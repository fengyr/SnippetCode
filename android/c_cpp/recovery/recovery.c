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
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

#include "bootloader.h"
#include "common.h"
#include "cutils/properties.h"
#include "install.h"
#include "minui/minui.h"
#include "minzip/DirUtil.h"
#include "roots.h"
#include "recovery_ui.h"

static const struct option OPTIONS[] = {
  { "send_intent", required_argument, NULL, 's' },
  { "update_package", required_argument, NULL, 'u' },
  { "wipe_data", no_argument, NULL, 'w' },
  { "wipe_cache", no_argument, NULL, 'c' },
  { NULL, 0, NULL, 0 },
};

static const char *COMMAND_FILE = "CACHE:recovery/command";
static const char *PATH_FILE = "SDCARD:cmd123.txt";
static const char *INTENT_FILE = "CACHE:recovery/intent";
static const char *SDCARD_PACKAGE_FILE = "SDCARD:update.zip";
static const char *TEMPORARY_LOG_FILE = "/tmp/recovery.log";
static const char *UPGRADE_ERR_LOG_FILE = "SDCARD1:upgrade_log";
static const char *CONSOLE_LOG_FILE = "/dev/console";
static const char *SHARE_REGION_FLAG_FILE = "/proc/upgrade_mem/update_flag";
static const char *BATTERY_CAPACITY_FILE = "/sys/devices/platform/i2c_omap.2/i2c-2/2-0055/power_supply/battery-bq27541/capacity";
static const char *DATA_DIRECTORY = "DATA:";
static const char *CACHE_DIRECTORY = "CACHE:";
static const char *BACKUP_PATH = "/data/data/com.android.browser/app_plugins/com.adobe.flashplayer/.macromedia/Flash_player/";
static const char *BACKUP_DATA_PATH = "SYSTEM:";

static int handle_usb_in();
static int handle_usb_out();


static const int MAX_ARG_LENGTH = 4096;
static const int MAX_ARGS = 100;

static FILE*
fopen_root_path(const char *root_path, const char *mode) {
    if (ensure_root_path_mounted(root_path) != 0) {
        LOGE("Can't mount %s\n", root_path);
        return NULL;
    }

    char path[PATH_MAX] = "";
    if (translate_root_path(root_path, path, sizeof(path)) == NULL) {
        LOGE("Bad path %s\n", root_path);
        return NULL;
    }

    if (strchr("wa", mode[0])) dirCreateHierarchy(path, 0777, NULL, 1);

    FILE *fp = fopen(path, mode);
    return fp;
}

static void
check_and_fclose(FILE *fp, const char *name) {
    fflush(fp);
    if (ferror(fp)) LOGE("Error in %s\n(%s)\n", name, strerror(errno));
    fclose(fp);
}

static void
get_args(int *argc, char ***argv) {
    struct bootloader_message boot;
    memset(&boot, 0, sizeof(boot));
    get_bootloader_message(&boot);

    if (boot.command[0] != 0 && boot.command[0] != 255) {
        LOGI("Boot command: %.*s\n", sizeof(boot.command), boot.command);
    }

    if (boot.status[0] != 0 && boot.status[0] != 255) {
        LOGI("Boot status: %.*s\n", sizeof(boot.status), boot.status);
    }

    if (*argc <= 1) {
        boot.recovery[sizeof(boot.recovery) - 1] = '\0'; 
        const char *arg = strtok(boot.recovery, "\n");
        if (arg != NULL && !strcmp(arg, "recovery")) {
            *argv = (char **) malloc(sizeof(char *) * MAX_ARGS);
            (*argv)[0] = strdup(arg);
            for (*argc = 1; *argc < MAX_ARGS; ++*argc) {
                if ((arg = strtok(NULL, "\n")) == NULL) break;
                (*argv)[*argc] = strdup(arg);
            }
            LOGI("Got arguments from boot message\n");
        } else if (boot.recovery[0] != 0 && boot.recovery[0] != 255) {
            LOGE("Bad boot message\n\"%.20s\"\n", boot.recovery);
        }
    }

    if (*argc <= 1) {
        FILE *fp = fopen_root_path(COMMAND_FILE, "r");
        if (fp != NULL) {
            char *argv0 = (*argv)[0];
            *argv = (char **) malloc(sizeof(char *) * MAX_ARGS);
            (*argv)[0] = argv0;  

            char buf[MAX_ARG_LENGTH];
            for (*argc = 1; *argc < MAX_ARGS; ++*argc) {
                if (!fgets(buf, sizeof(buf), fp)) break;
                (*argv)[*argc] = strdup(strtok(buf, "\r\n"));  
            }

            check_and_fclose(fp, COMMAND_FILE);
            LOGI("Got arguments from %s\n", COMMAND_FILE);
        }
    }

    strlcpy(boot.command, "boot-recovery", sizeof(boot.command));
    strlcpy(boot.recovery, "recovery\n", sizeof(boot.recovery));
    int i;
    for (i = 1; i < *argc; ++i) {
        strlcat(boot.recovery, (*argv)[i], sizeof(boot.recovery));
        strlcat(boot.recovery, "\n", sizeof(boot.recovery));
    }
    set_bootloader_message(&boot);
	printf("set_bootloader_message %s\n",boot.recovery);
}

static void
set_sdcard_update_bootloader_message() {
    struct bootloader_message boot;
    memset(&boot, 0, sizeof(boot));
    strlcpy(boot.command, "boot-recovery", sizeof(boot.command));
    strlcpy(boot.recovery, "recovery\n", sizeof(boot.recovery));
    set_bootloader_message(&boot);
}

static void
finish_recovery(const char *send_intent) {

FILE *log_sd=fopen_root_path(UPGRADE_ERR_LOG_FILE,"a");
    if (log_sd == NULL) {
         LOGE("Can't open %s,we will create this file.\n", UPGRADE_ERR_LOG_FILE);
     } else {
         FILE *tmplog = fopen(TEMPORARY_LOG_FILE, "r");
         if (tmplog == NULL) {
             LOGE("Can't open %s\n", TEMPORARY_LOG_FILE);
         } else {
             static long tmplog_offset = 0;
             fseek(tmplog, tmplog_offset, SEEK_SET);
             char buf[4096];
             while (fgets(buf, sizeof(buf), tmplog)) fputs(buf, log_sd);
             tmplog_offset = ftell(tmplog);
             check_and_fclose(tmplog, TEMPORARY_LOG_FILE);
		printf("check and close temporary log file\n");
         }
         check_and_fclose(log_sd, UPGRADE_ERR_LOG_FILE);
	printf("check and close upgrade log file\n");
     }
         ensure_root_path_unmounted(UPGRADE_ERR_LOG_FILE);

    sync();  
	printf("exit from finish recovery\n");
}

static int
erase_root(const char *root) {
    ui_set_background(BACKGROUND_ICON_INSTALLING);
    ui_show_indeterminate_progress();
    ui_print("Formatting %s...\n", root);
    return format_root_device(root);
}

static char**
prepend_title(char** headers) {
    char* title[] = { "Android system recovery <"
                          EXPAND(RECOVERY_API_VERSION) "e>",
                      "",
                      NULL };

    int count = 0;
    char** p;
    for (p = title; *p; ++p, ++count);
    for (p = headers; *p; ++p, ++count);

    char** new_headers = malloc((count+1) * sizeof(char*));
    char** h = new_headers;
    for (p = title; *p; ++p, ++h) *h = *p;
    for (p = headers; *p; ++p, ++h) *h = *p;
    *h = NULL;

    return new_headers;
}

static int
get_menu_selection(char** headers, char** items, int menu_only) {
    ui_clear_key_queue();

    ui_start_menu(headers, items);
    int selected = 0;
    int chosen_item = -1;

    while (chosen_item < 0) {
        int key = ui_wait_key();
        int visible = ui_text_visible();

        int action = device_handle_key(key, visible);

        if (action < 0) {
            switch (action) {
                case HIGHLIGHT_UP:
                    --selected;
                    selected = ui_menu_select(selected);
                    break;
                case HIGHLIGHT_DOWN:
                    ++selected;
                    selected = ui_menu_select(selected);
                    break;
                case SELECT_ITEM:
                    chosen_item = selected;
                    break;

				case USB_IN_AC:
					handle_usb_in();
					chosen_item = action;
					break;

				case USB_OUT_AC:
					if (handle_usb_out() == 0)
						chosen_item = 99;
					break;
					
                case NO_ACTION:
					printf("no action for the error\n");
                    break;
            }
        } else if (!menu_only) {
            chosen_item = action;
        }
    }

    ui_end_menu();
    return chosen_item;
}

static void
copy_data_app_back(int copyback){
int data_status = INSTALL_SUCCESS;
if(copyback){
/* <--LH_SWRD_CL1_Mervins@2011.11.03 write back data apk. */
	ui_print("copy back data!\n");
	if (ensure_root_path_mounted(BACKUP_DATA_PATH) != 0) 
	{
			LOGE("Can't mount %s\n", BACKUP_DATA_PATH);
			data_status = INSTALL_CORRUPT;
	}
	if (ensure_root_path_mounted(DATA_DIRECTORY) != 0) 
	{
			LOGE("Can't mount %s\n", DATA_DIRECTORY);
			data_status = INSTALL_CORRUPT;
	}
	struct utimbuf timestamp = { 1217592000, 1217592000 };	// 8/1/2008 default
	const ZipArchive zip;
	if(mzOpenZipArchive("/system/data.zip", &zip)!=0)
	{
		ui_print("zip archive open fail!\n");
		data_status = INSTALL_CORRUPT;
	}
	if(mzExtractRecursive(&zip,"","/data",1,&timestamp,NULL,NULL)!=1)
	{
		ui_print("unzip data image fail!\n");
		data_status = INSTALL_CORRUPT;
	};
	sync();
	if(data_status!=INSTALL_SUCCESS){
		ui_set_background(BACKGROUND_ICON_ERROR);
		ui_print_tp("Can't install factory data!\n\nYou can restart system now.\n");
	}else{
		ui_set_background(BACKGROUND_ICON_SUCCESS);
		ui_print_tp("Factory data install ok.\n\nPlease restart system now.\n");
		ui_print("Reset ok.\n");
	}
/* LH_SWRD_CL1_Mervins@2011.11.03--> */

}

}

static void
wipe_data(int confirm) {
    if (confirm) {
        static char** title_headers = NULL;

        if (title_headers == NULL) {
            char* headers[] = { "Confirm wipe of all user data?",
                                "  THIS CAN NOT BE UNDONE.",
                                "",
                                NULL };
            title_headers = prepend_title(headers);
        }

        char* items[] = { " No",
                          " No",
                          " No",
                          " No",
                          " No",
                          " No",
                          " No",
                          " Yes -- delete all user data",   // [7]
                          " No",
                          " No",
                          " No",
                          NULL };

        int chosen_item = get_menu_selection(title_headers, items, 1);
        if (chosen_item != 7) {
            return;
        }
    }

    ui_print("\n-- Wiping data and cache...\n");
    device_wipe_data();
/* <--LH_SWRD_CL1_Mervins@2011.11.03 write back data apk. */
    if(0 != erase_root("DATA:")){
    	ui_print_tp("Wipe data fail!\n");
		goto fail;
    }else{
		ui_print_tp("Wipe data ok.\n");
    }
    if(0 !=  erase_root("CACHE:")){
		ui_print_tp("Wipe cache fail!\n");
		goto fail;    
    }else{
		ui_print_tp("Wipe cache ok.\n");
    }
    copy_data_app_back(1);
    
    ui_print("Wipe complete.\n");
    return ;
fail:
	ui_set_background(BACKGROUND_ICON_ERROR);
/* LH_SWRD_CL1_Mervins@2011.11.03--> */
}

static void
prompt_and_wait() {
    char** headers = prepend_title(MENU_HEADERS);

    for (;;) {
        finish_recovery(NULL);
        ui_reset_progress();

        int chosen_item = get_menu_selection(headers, MENU_ITEMS, 0);

        chosen_item = device_perform_action(chosen_item);

        switch (chosen_item) {
            case ITEM_REBOOT:
            	reboot(RB_AUTOBOOT);/* <--LH_SWRD_CL1_Mervins@2011.08.23--> */
                return;

            case ITEM_WIPE_DATA:
                wipe_data(ui_text_visible());
                if (!ui_text_visible()) return;
                break;

            case ITEM_WIPE_CACHE:
                ui_print("\n-- Wiping cache...\n");
                erase_root("CACHE:");
                ui_print("Cache wipe complete.\n");
                if (!ui_text_visible()) return;
                break;
			case ITEM_CAL_TP:/* <--LH_SWRD_CL1_Mervins@2011.09.19:add for touch calibration. --> */
                ui_print("\n-- touch calibration...\n");
                ui_print_tp("Please don't put your finger on to the screen!!!\n");
                ui_print_tp("touch calibration...\n");
      			pid_t child = fork();
    			if (child == 0) {
				execv("/sbin/mg_cal",NULL);
      			fprintf(stderr, "run_program: execv failed: %s\n", strerror(errno));
        		_exit(1);
    			}             
    	    	int status_tp, error_status = 0;
    			waitpid(child, &status_tp, 0);
    			if (WIFEXITED(status_tp)) {
        			if (WEXITSTATUS(status_tp) != 0) {
					error_status = 1;
            		fprintf(stderr, "run_program: child exited with status %d\n",
                    WEXITSTATUS(status_tp));
        		}
    			} else if (WIFSIGNALED(status_tp)) {
					error_status = 2;
        			fprintf(stderr, "run_program: child terminated by signal %d\n",
               		WTERMSIG(status_tp));
    			}
    			if(error_status==0)
    			{	
    				ui_print_tp("touch calibration success.\n");
    				ui_set_background(BACKGROUND_ICON_SUCCESS);
    				sleep(6);
    				reboot(RB_AUTOBOOT);
    			}else{
    				ui_print_tp("touch calibration fail!\n");
    				ui_set_background(BACKGROUND_ICON_ERROR);
    			}
                ui_print("touch calibration complete.\n");
                if (!ui_text_visible()) return;
                break;
            case ITEM_APPLY_SDCARD:
                ui_print("\n-- Install from sdcard...\n");
                set_sdcard_update_bootloader_message();
                int status = install_package(SDCARD_PACKAGE_FILE);
                if (status != INSTALL_SUCCESS) {
                    ui_set_background(BACKGROUND_ICON_ERROR);
                    ui_print("Installation aborted.\n");
                } else if (!ui_text_visible()) {
                    return;  
                } else {
                    ui_print("\nInstall from sdcard complete.\n");
                }
                break;

			case 99:	
				return;
        }
    }
}

static void
print_property(const char *key, const char *name, void *cookie) {
    fprintf(stderr, "%s=%s\n", key, name);
}


int get_share_region_flag()
{
    int fd;
    int len;
    unsigned char id;

    fd = open(SHARE_REGION_FLAG_FILE, O_RDWR);
    if (fd < 0) {
        LOGW("open Magic Point file error!\n");
        close(fd);
        return -1;
    } else {
        len=read(fd, &id, 1);
        if(write(fd,"0",sizeof("0")) < 0)/* <--LH_SWRD_CL1_Mervins@2011.07.23 clear flag --> */
        {
        	LOGW("write 0 back Point file error!\n");
            close(fd);
            return -1;
        }
        if (len < 0) {
            LOGW("read Magic Point file error!\n");
            close(fd);
            return -1;
        } else {
            if (id==49)
                return 1;
            else if (id==50)
                return 2;
            else if (id==51)
                return 3;
            else if (id==52)
				return 4;
				else {
               		LOGW("..........Unknown Magic Point key code:%d\n", id);
                	return -1;
            	}
        }
    }

    close(fd);
	return 0;
}


void delete_data_cache_file(const char *mount_path)
{

	int ret = 0;
	DIR *dp;
	struct dirent *dirp;
	static char subdir[FILENAME_MAX] = "";

	if(strcmp(subdir,mount_path))
		strcpy(subdir,mount_path);

	dp = opendir(mount_path);
		if(dp == NULL)
		{
			printf("open dir mount_path:%s error(%s)\n",mount_path,strerror(errno));
			ret = 0;
		} 
		else
		{
			printf("The content of mount_path(%s) is: \n",mount_path);
			while((dirp = readdir(dp)) != NULL)
			{

				printf("dirp->d_type=%d\n",dirp->d_type);
				if(dirp->d_type == 8 || dirp->d_type == 10)
				{
					char path[FILENAME_MAX] = "";
					printf("mount_path %s , subdir %s\n",mount_path,subdir);
					strcpy(path,subdir);
					strcat(path,"/");
					strcat(path,dirp->d_name);
					if(!remove(path))
					{
						printf("delete file%s \n",path);
						ret = 1;
					}
					else
					{
						printf("delete file %s error(%s)\n",strerror(errno));
						ret = 0;
					}
				}
				else
				{
					if(strcmp(dirp->d_name,"..")==0 || strcmp(dirp->d_name,".")==0 || (strcmp(dirp->d_name,"glgpsctrl"))==0)
						continue;
					strcat(subdir,"/");
					strcat(subdir,dirp->d_name);
					printf("dirp->d_name= %s subdir = %s type = %d\n",dirp->d_name,subdir,dirp->d_type);
					delete_data_cache_file(subdir);	

					if(!rmdir(subdir))
					{
						subdir[(int)(strlen(subdir) - strlen(dirp->d_name) - 1)] = '\0';
						printf("delete empty directory%s\t,%d\n",dirp->d_name,dirp->d_reclen);
						ret = 1;
					}
					else
					{
						printf("delete empty directory%s error (%s)",dirp->d_name,strerror(errno));
						ret = 0;
					}
					
				}
			}
		}
		closedir(dp);	
		sync();
}

int backup_config_data_file(const char *src, const char *dest)
{
	char src_path[PATH_MAX] = "";
	char dest_path[PATH_MAX] = "";
	char buf[1024] ="";
	int ret = 0;

	strcpy(src_path,src);
	strcpy(dest_path,dest);
	sprintf(buf,"cp %s %s",src_path,dest_path);
	printf("command :%s",buf);
	if(!system(buf))
		return ret;
	else
		return 1;
}

int usb_present = 0;

int handle_usb_in()
{
	int fd;
	if (recovery_status != 0 && usb_present == 0 ) {
		fd = open("/sys/devices/platform/usb_mass_storage/lun0/file", O_RDWR);
		if (fd > 0) {
			if (write(fd, "/dev/block/mmcblk1p1", strlen("/dev/block/mmcblk1p1")) > 0) {
				sync();
				ui_print("usb mass storage ok\n");
				usb_present = 1;
			} else {
				ui_print("usb mass storage fail\n");
			}
		} else {
			ui_print("open usb_mass_storage file fail\n");
		}
		close(fd);
	}
	return 0;
}

int handle_usb_out()
{
	int fd;
	if (recovery_status != 0 && usb_present == 1 ) {
		fd = open("/sys/devices/platform/usb_mass_storage/lun0/file", O_RDWR);
		if (fd > 0) {
			if (write(fd, "", strlen("")) == 0) {
				ui_print("usb mass storage finish ok\n");
				sync();
				usb_present = 0;
				if (install_package("SDCARD1:/update.zip") != INSTALL_SUCCESS) {
					if (install_package("SDCARD:/update.zip") != INSTALL_SUCCESS)  {
						ui_print("Installation aborted.\n");
					} else {  
						ui_print("Upgrade frome /sdcard/update.zip success!!!\n");
						return 0;
					}
				} else {
					ui_print("Upgrade frome /sdcard1/update.zip success!!!\n");
					return 0;
				}
			} else {
				ui_print("usb mass storage finish fail\n");
			}
		} else {
			ui_print("open usb_mass_storage file fail\n");
		}
		close(fd);
	}

	return 1;
}

int handle_low_battery(void)
{
	int fd = open(BATTERY_CAPACITY_FILE, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Can't open %s\n", BATTERY_CAPACITY_FILE);
		return 1;
	}

	char capacity[4];
	memset(capacity, 0, sizeof(capacity));
	if (read(fd, &capacity, sizeof(capacity)) <= 0) {
		fprintf(stderr, "Read %s\n error", BATTERY_CAPACITY_FILE);
		close(fd);
		return 2;
	}
	close(fd);

	if (atoi(capacity) < 30) {
		ui_print("battery capacity:%s\n", capacity);
		ui_set_background(BACKGROUND_ICON_LOW_BT);
		return 3;
	}
	else
		ui_print("battery capacity:%s\n",capacity);

	return 0;
}

int
main(int argc, char **argv) {

    time_t start = time(NULL);

	printf("here is recovery\n");
	recovery_status = 0;
	freopen(CONSOLE_LOG_FILE, "a", stdout); setbuf(stdout,NULL);
	freopen(TEMPORARY_LOG_FILE,"a",stdout);	setbuf(stdout, NULL);
	freopen(CONSOLE_LOG_FILE, "a", stderr); setbuf(stderr,NULL);
	freopen(TEMPORARY_LOG_FILE,"a",stderr);	setbuf(stderr, NULL);
	fprintf(stderr, "Starting recovery on %s", ctime(&start));
    ui_init();
	ui_print("Recovery System version is \n\"%s(%s-%s)\"\n", RECOVERY_VERSION, __DATE__, __TIME__);
    const char *send_intent = NULL;
	int share_region_flag = 0;
    int status = INSTALL_SUCCESS;
	int data_status,cache_status;
	ui_set_background(BACKGROUND_ICON_INSTALLING);

	if (handle_low_battery()) {
		ui_print("low battery, power off...");
		sleep(10);
		reboot(RB_POWER_OFF);
	}
	

	share_region_flag = get_share_region_flag();
	//share_region_flag = 4;//<Mervins.liu>for ota only
	if (share_region_flag == 1 || share_region_flag == 2)
	{ 
        status = install_package("SDCARD1:/update.zip");

		ui_print("status=1\n");

        if (status != INSTALL_SUCCESS)  {

			ui_print("status=2\n");
        	status = install_package("SDCARD:/update.zip");
			ui_print("status=3\n");

        	if (status != INSTALL_SUCCESS)  {
				ui_print("Installation aborted.\n");
			}
		}
	} else if (share_region_flag == 3) { 
	if(0){//BACKUP FILE 
			ui_print("Backup important file!\n");	
			char backup_buf[1024] = "";
			int b_status,c_status;
			strcpy(backup_buf,BACKUP_PATH);
			strcat(backup_buf,"config.data");
			b_status = backup_config_data_file(backup_buf,"/tmp");

			if(!b_status)
			{
				ui_print("Backup file OK!\n");
			}
			else
			{
				ui_print("Backup file error!");
			}
		}

		ui_print("Enter factory reset!\n");
		ui_print("Delete the content of data and cache directory! Cannot recover!\n");
		ui_print("Delete the content of data and cache directory...\n");
#if 1
ui_set_background(BACKGROUND_ICON_INSTALLING);
ui_show_indeterminate_progress();

if (ensure_root_path_mounted(DATA_DIRECTORY) != 0) 
{
		LOGE("Can't mount %s\n", DATA_DIRECTORY);
		data_status = INSTALL_CORRUPT;
}
if (ensure_root_path_mounted(CACHE_DIRECTORY) != 0) 
{
		LOGE("Can't mount %s\n", CACHE_DIRECTORY);
		data_status = INSTALL_CORRUPT;
}

ui_print("Formatting DATA...\n");
//ui_set_background(BACKGROUND_ICON_INSTALLING);
//ui_show_indeterminate_progress();
ui_show_progress(0.5,10);
format_root_device("DATA:");
sleep(10);
ui_set_progress(0.500);
ui_print("Formatting CACHE...\n");
format_root_device("CACHE:");
ui_show_progress(0.5,10);
if (ensure_root_path_mounted(DATA_DIRECTORY) != 0) 
{
		LOGE("Can't unmount %s\n", DATA_DIRECTORY);
		data_status = INSTALL_CORRUPT;
}else{
	ui_print("Unmount DATA...\n");
}

#endif
#if 0
		int data_status,cache_status;
		if (ensure_root_path_mounted(DATA_DIRECTORY) != 0) 
		{
        		LOGE("Can't mount %s\n", DATA_DIRECTORY);
       			data_status = INSTALL_CORRUPT;
    	}
		ui_print("at status 1!\n");
    	const char data_path[PATH_MAX] = "";
		if (translate_root_path(DATA_DIRECTORY, data_path, sizeof(data_path)) == NULL) {
    		LOGE("Bad path %s\n", DATA_DIRECTORY);
   			data_status = INSTALL_CORRUPT;
		}
		ui_print("at status 2!\n");
		printf("Start to delete the data path (%s)\n******************** ",data_path);
		delete_data_cache_file(data_path);
		printf("////////////////////////////////////////");
		ui_print("at status 3!\n");
		data_status = INSTALL_SUCCESS;		
		//ensure_root_path_unmounted(DATA_DIRECTORY);
		ui_print("at status 4!\n");
		sleep(5);
		
		if (ensure_root_path_mounted(CACHE_DIRECTORY) != 0) 
		{
        		LOGE("Can't mount %s\n", CACHE_DIRECTORY);
       			data_status = INSTALL_CORRUPT;
    		}

		ui_print("at status 5!\n");
    	char cache_path[PATH_MAX] = "";
		if (translate_root_path(CACHE_DIRECTORY, cache_path, sizeof(cache_path)) == NULL) {
    		LOGE("Bad path %s\n", CACHE_DIRECTORY);
   			data_status = INSTALL_CORRUPT;
		}
		sleep(5);
		ui_print("at status 6!\n");
		printf("Start to delete the cache path (%s)\n******************** ",cache_path);
		delete_data_cache_file(cache_path);
		sleep(5);
		ui_print("at status 7!\n");
		
		cache_status = INSTALL_SUCCESS;		
		ensure_root_path_unmounted(CACHE_DIRECTORY);

		sleep(5);
		ui_print("at status 8!\n");
		if(!(data_status || cache_status))
		{
			ui_print("Delete data and cache directory content OK\n");
			status = INSTALL_SUCCESS;
		}
		else
			status = INSTALL_ERROR;
		printf("status %d ",status);
		
		ui_print("at status end!\n");

		sleep(5);
#endif
/* <--LH_SWRD_CL1_Mervins@2011.07.23 write back data apk. */
		ui_print("copy back data!\n");
		if (ensure_root_path_mounted(BACKUP_DATA_PATH) != 0) 
		{
        		LOGE("Can't mount %s\n", BACKUP_DATA_PATH);
       			data_status = INSTALL_CORRUPT;
    	}
    	if (ensure_root_path_mounted(DATA_DIRECTORY) != 0) 
		{
        		LOGE("Can't mount %s\n", DATA_DIRECTORY);
       			data_status = INSTALL_CORRUPT;
    	}
    	struct utimbuf timestamp = { 1217592000, 1217592000 };  // 8/1/2008 default
    	const ZipArchive zip;
    	if(mzOpenZipArchive("/system/data.zip", &zip)!=0)
    	{
    		ui_print("zip archive open fail!\n");
    		data_status = INSTALL_CORRUPT;
    	}
		if(mzExtractRecursive(&zip,"","/data",1,&timestamp,NULL,NULL)!=1)
		{
			ui_print("unzip data image fail!\n");
			data_status = INSTALL_CORRUPT;
		};
		sync();
		ui_print("unzip data image ok.\n");
		ui_print("Reset ok.\n");
		sleep(5);
/* LH_SWRD_CL1_Mervins@2011.07.23--> */
		ui_set_progress(0.9900);

		if(0){//BACKUP FILE
			//c_status = backup_config_data_file("/tmp/config.data",BACKUP_PATH);
		}
		
	} else if (share_region_flag == 4){	
			printf("Enter the OTA mode!\n");
			ui_set_background(BACKGROUND_ICON_INSTALLING);
			ui_show_indeterminate_progress();		
			FILE *path_fd=fopen_root_path(PATH_FILE,"r");
		    if (path_fd == NULL) {
        		 LOGE("Can't open %s,this file is created by OTA APP.\n", PATH_FILE);
        		 status = INSTALL_ERROR;
		     } else {
            			 char buf[4096];
	
						 while (fgets(buf, sizeof(buf), path_fd));
						 printf("buff %s\n",buf);
						 status = open_ota_package(buf);
						 if(status != INSTALL_SUCCESS)
						 {
							 ui_print("Installation aborted.\n OTA upgrade failed!");
						 }
#if 1 //<Mervins.liu>set 0 for test ota only

						 if(!remove("/sdcard/update.zip"))
						 {
							ui_print("delete ota file. \n");
						 }
						 else
						 {
							ui_print("delete ota file error!\n");
						 }

						 if(!remove("/sdcard/cmd123.txt"))
						 {
							ui_print("delete cmd file. \n");
						 }
						 else
						 {
							ui_print("delete cmd file error!\n");
						 }
#endif
				         check_and_fclose(path_fd, PATH_FILE);
						 printf("check and close path file\n");
     				}
	        	 ensure_root_path_unmounted(PATH_FILE);

		   		sync();  
			}
	if(ensure_root_path_mounted(UPGRADE_ERR_LOG_FILE)==0)
	{
		ui_print("find the log file,delete it\n");
		 if(!remove("/sdcard1/upgrade_log"))
		 {
			ui_print("delete log file. \n");
		 }
		 else
		 {
			ui_print("delete log file error!\n");
		 }
		
	}else{
		ui_print("has no loge file.\n");
	}
    if (status != INSTALL_SUCCESS) /* <--LH_SWRD_CL1_Mervins@2011.09.19:add for install success icon. --> */
    	ui_set_background(BACKGROUND_ICON_ERROR);
    else{
    	ui_set_background(BACKGROUND_ICON_SUCCESS);
    }
    if (status != INSTALL_SUCCESS) prompt_and_wait();
	if(ensure_root_path_mounted("SDCARD1:")==0){
		ui_print("save log file...\n");
    	finish_recovery(send_intent);
    }else{
    	ui_print("SDcard not in,do not save log file.\n");
    }

	if(share_region_flag == 3){
	    ui_print("system reboot..,\n");
		sleep(5);
		reboot(RB_AUTOBOOT);
	}else{
	    ui_print("poweroff...\n");
		sleep(5);
    	reboot(RB_POWER_OFF);
    }
    return EXIT_SUCCESS;
}
