/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "install.h"
#include "mincrypt/rsa.h"
#include "minui/minui.h"
#include "minzip/SysUtil.h"
#include "minzip/Zip.h"
#include "mtdutils/mounts.h"
#include "mtdutils/mtdutils.h"
#include "roots.h"
#include "verifier.h"

#define ASSUMED_UPDATE_BINARY_NAME  "META-INF/com/google/android/update-binary"
#define HW_VERSION_ENTRY_ZIP "version"
#define HW_VERSION_FILE_PROC "/proc/hw_version"
#define PUBLIC_KEYS_FILE "/res/keys"

// If the package contains an update binary, extract it and run it.
static int
try_update_binary(const char *path, ZipArchive *zip) {
    const ZipEntry* binary_entry =
            mzFindZipEntry(zip, ASSUMED_UPDATE_BINARY_NAME);
    if (binary_entry == NULL) {
        return INSTALL_CORRUPT;
    }

    char* binary = "/tmp/update_binary";
    unlink(binary);
    int fd = creat(binary, 0755);
    if (fd < 0) {
        LOGE("Can't make %s\n", binary);
        return 1;
    }
    bool ok = mzExtractZipEntryToFile(zip, binary_entry, fd);
    close(fd);

    if (!ok) {
        LOGE("Can't copy %s\n", ASSUMED_UPDATE_BINARY_NAME);
        return 1;
    }

    int pipefd[2];
    pipe(pipefd);

    // When executing the update binary contained in the package, the
    // arguments passed are:
    //
    //   - the version number for this interface
    //
    //   - an fd to which the program can write in order to update the
    //     progress bar.  The program can write single-line commands:
    //
    //        progress <frac> <secs>
    //            fill up the next <frac> part of of the progress bar
    //            over <secs> seconds.  If <secs> is zero, use
    //            set_progress commands to manually control the
    //            progress of this segment of the bar
    //
    //        set_progress <frac>
    //            <frac> should be between 0.0 and 1.0; sets the
    //            progress bar within the segment defined by the most
    //            recent progress command.
    //
    //        firmware <"hboot"|"radio"> <filename>
    //            arrange to install the contents of <filename> in the
    //            given partition on reboot.
    //
    //            (API v2: <filename> may start with "PACKAGE:" to
    //            indicate taking a file from the OTA package.)
    //
    //            (API v3: this command no longer exists.)
    //
    //        ui_print <string>
    //            display <string> on the screen.
    //
    //   - the name of the package zip file.
    //

    char** args = malloc(sizeof(char*) * 5);
    args[0] = binary;
    args[1] = EXPAND(RECOVERY_API_VERSION);   // defined in Android.mk
    args[2] = malloc(10);
    sprintf(args[2], "%d", pipefd[1]);
    args[3] = (char*)path;
    args[4] = NULL;
	
	
	ui_print("path=");
	ui_print(path);

    pid_t pid = fork();
    if (pid == 0) {
        close(pipefd[0]);
        execv(binary, args);
        fprintf(stderr, "E:Can't run %s (%s)\n", binary, strerror(errno));
        _exit(-1);
    }
    close(pipefd[1]);

    char buffer[1024];
    FILE* from_child = fdopen(pipefd[0], "r");
    while (fgets(buffer, sizeof(buffer), from_child) != NULL) {
        char* command = strtok(buffer, " \n");
        if (command == NULL) {
            continue;
        } else if (strcmp(command, "progress") == 0) {
            char* fraction_s = strtok(NULL, " \n");
            char* seconds_s = strtok(NULL, " \n");

            float fraction = strtof(fraction_s, NULL);
            int seconds = strtol(seconds_s, NULL, 10);

            ui_show_progress(fraction * (1-VERIFICATION_PROGRESS_FRACTION),
                             seconds);
        } else if (strcmp(command, "set_progress") == 0) {
            char* fraction_s = strtok(NULL, " \n");
            float fraction = strtof(fraction_s, NULL);
            ui_set_progress(fraction);
        } else if (strcmp(command, "ui_print") == 0) {
            char* str = strtok(NULL, "\n");
            if (str) {
                ui_print(str);
            } else {
                ui_print("\n");
            }
        } else {
            LOGE("unknown command [%s]\n", command);
        }
    }
    fclose(from_child);

    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        LOGE("Error in %s\n(Status %d)\n", path, WEXITSTATUS(status));
        return INSTALL_ERROR;
    }

    return INSTALL_SUCCESS;
}

static int
handle_update_package(const char *path, ZipArchive *zip)
{
    // Update should take the rest of the progress bar.
    ui_print("Installing update...\n");

    int result = try_update_binary(path, zip);
    register_package_root(NULL, NULL);  // Unregister package root
    return result;
}

// Reads a file containing one or more public keys as produced by
// DumpPublicKey:  this is an RSAPublicKey struct as it would appear
// as a C source literal, eg:
//
//  "{64,0xc926ad21,{1795090719,...,-695002876},{-857949815,...,1175080310}}"
//
// (Note that the braces and commas in this example are actual
// characters the parser expects to find in the file; the ellipses
// indicate more numbers omitted from this example.)
//
// The file may contain multiple keys in this format, separated by
// commas.  The last key must not be followed by a comma.
//
// Returns NULL if the file failed to parse, or if it contain zero keys.
static RSAPublicKey*
load_keys(const char* filename, int* numKeys) {
    RSAPublicKey* out = NULL;
    *numKeys = 0;

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        LOGE("opening %s: %s\n", filename, strerror(errno));
        goto exit;
    }

    int i;
    bool done = false;
    while (!done) {
        ++*numKeys;
        out = realloc(out, *numKeys * sizeof(RSAPublicKey));
        RSAPublicKey* key = out + (*numKeys - 1);
        if (fscanf(f, " { %i , 0x%x , { %u",
                   &(key->len), &(key->n0inv), &(key->n[0])) != 3) {
            goto exit;
        }
        if (key->len != RSANUMWORDS) {
            LOGE("key length (%d) does not match expected size\n", key->len);
            goto exit;
        }
        for (i = 1; i < key->len; ++i) {
            if (fscanf(f, " , %u", &(key->n[i])) != 1) goto exit;
        }
        if (fscanf(f, " } , { %u", &(key->rr[0])) != 1) goto exit;
        for (i = 1; i < key->len; ++i) {
            if (fscanf(f, " , %u", &(key->rr[i])) != 1) goto exit;
        }
        fscanf(f, " } } ");

        switch (fgetc(f)) {
            case ',':
                break;

            case EOF:
                done = true;
                break;

            default:
                LOGE("unexpected character between keys\n");
                goto exit;
        }
    }

    fclose(f);
    return out;

exit:
    if (f) fclose(f);
    free(out);
    *numKeys = 0;
    return NULL;
}

static int get_hw_version_zip(ZipArchive *zip, char* item_name, char* zip_hw_version)
{
	int ret;
	int fd;
    char* hw_version = "/tmp/hw_version";
    const ZipEntry* hw_version_entry;

   	hw_version_entry = mzFindZipEntry(zip, HW_VERSION_ENTRY_ZIP);
    if (hw_version_entry == NULL) {
        return 1;
    }

    fd = creat(hw_version, 0755);
    if (fd < 0) {
        LOGE("Can't make %s\n", hw_version);
        return 2;
    }


    if (!mzExtractZipEntryToFile(zip, hw_version_entry, fd)) {
        LOGE("Can't copy %s, error is %s\n", HW_VERSION_ENTRY_ZIP, strerror(errno));
        return 4;
    }
	close(fd);

	fd = open(hw_version, O_RDONLY);
	ret = read(fd, zip_hw_version, 256);
	ui_print("read %d\n", ret);
	if (ret <= 0) {
		LOGE("Can't read from %s\n", hw_version);
		return 3;
	}
    close(fd);

	ui_print("zip_hw_version=%s\n", zip_hw_version);
	return 0;
}

static int get_hw_version_proc(char* proc_hw_version)
{
	int ret;
	FILE* fp = fopen(HW_VERSION_FILE_PROC, "r");
	if (fp == NULL)
		return 0;	
	
	ret = fread(proc_hw_version, 1, 256, fp);
	ui_print("fread %d\n", ret);
	if (ret <= 0) {
		LOGE("Can't read from %s\n", HW_VERSION_FILE_PROC);
		fclose(fp);
		return 1;
	}
	fclose(fp);

	proc_hw_version[strlen(proc_hw_version) - 1] = '\0';

	ui_print("proc_hw_version=%s\n", proc_hw_version);
	
	return 0;
}

static int check_hw_version(ZipArchive *zip)
{
	char buff_proc[256];
	char buff_zip[256];
	char buff_item[256];
	
	memset(buff_zip, 0, sizeof(buff_zip));
	memset(buff_item, 0, sizeof(buff_item));
	memset(buff_proc, 0, sizeof(buff_proc));
	if (get_hw_version_proc(buff_proc) || get_hw_version_zip(zip, buff_item, buff_zip)) 
		return 1;
   	
	return !strstr(buff_zip, buff_proc);
}

int
install_package(const char *root_path)
{
	recovery_status = 0;
	int status = INSTALL_SUCCESS;
    ui_set_background(BACKGROUND_ICON_INSTALLING);
    ui_print("Finding update package...\n");
    ui_show_indeterminate_progress();

    if (ensure_root_path_mounted(root_path) != 0) {
        ui_print("Can't mount %s\n", root_path);
        status = INSTALL_CORRUPT;
		goto exit;
    }

    char path[PATH_MAX] = "";
    if (translate_root_path(root_path, path, sizeof(path)) == NULL) {
        ui_print("Bad path %s\n", root_path);
        status = INSTALL_CORRUPT;
		goto exit;
    }
	printf("package name = '%s'\t path_len= %d\n",path,strlen(path));
    ui_print("Opening update package...\n");
#if 0
    int numKeys;
    RSAPublicKey* loadedKeys = load_keys(PUBLIC_KEYS_FILE, &numKeys);
    if (loadedKeys == NULL) {
        LOGE("Failed to load keys\n");
        return INSTALL_CORRUPT;
    }
    LOGI("%d key(s) loaded from %s\n", numKeys, PUBLIC_KEYS_FILE);

    // Give verification half the progress bar...
    ui_print("Verifying update package...\n");
    ui_show_progress(
            VERIFICATION_PROGRESS_FRACTION,
            VERIFICATION_PROGRESS_TIME);

    int err;
    err = verify_file(path, loadedKeys, numKeys);
    free(loadedKeys);
    LOGI("verify_file returned %d\n", err);
    if (err != VERIFY_SUCCESS) {
        LOGE("signature verification failed\n");
        return INSTALL_CORRUPT;
    }
#endif

    /* Try to open the package.
     */
    int err;
    ZipArchive zip;
    err = mzOpenZipArchive(path, &zip);
    if (err != 0) {
        ui_print("Can't open %s\n(%s)\n", path, err != -1 ? strerror(err) : "bad");
        status = INSTALL_CORRUPT;
		goto exit;
    }

	/*
	 * Verify hw version 
	 */
	/*
	if (check_hw_version(&zip)) {
		ui_print("HW version doesn't match!!!");
		status = INSTALL_CORRUPT;
    	mzCloseZipArchive(&zip);
		goto exit;
	}
*/
    /* Verify and install the contents of the package.
     */
	ui_print("start update package file\n");
    status = handle_update_package(path, &zip);
	ui_print("install_success!!!\n");
    mzCloseZipArchive(&zip);

exit:
	ensure_root_path_unmounted(root_path);
	if (status != INSTALL_SUCCESS)
		recovery_status = 1;
	else
		recovery_status = 0;

    return status;
}

int open_ota_package(const char *root_path)
{
	int status = INSTALL_SUCCESS;
	int recovery_status = 0;
	ui_print("Opening OTA upgrade package...\n");

	char *c = root_path;
	const char *p;
	int len = strlen(root_path);
	int i;

	c = c + len-2;

	for(i=1;i<len-1;i++)
	{
		--c;
	}
	c[len-1] = '\0';
	p = c;

    int err;
    ZipArchive zip;
    err = mzOpenZipArchive(p, &zip);
    if (err != 0) {
        ui_print("Can't open '%s'\n(%s)\n", p, err != -1 ? strerror(err) : "bad");
        status = INSTALL_CORRUPT;
		goto exit;
    }
	
	ui_print("start update package file\n");
    status = handle_update_package(p, &zip);
	ui_print("install_success!!!\n");
    mzCloseZipArchive(&zip);

exit:
	if (status != INSTALL_SUCCESS)
		recovery_status = 1;
	else
		recovery_status = 0;

    return status;

}
