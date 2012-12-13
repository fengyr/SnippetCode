ifneq ($(TARGET_SIMULATOR),true)
ifeq ($(TARGET_ARCH),arm)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

commands_recovery_local_path := $(LOCAL_PATH)

LOCAL_C_INCLUDES:= \
	external/e2fsprogs/lib \
	external/e2fsprogs/misc \
	external/e2fsprogs \
	external/e2fsprogs/e2fsck


LOCAL_SRC_FILES := \
    recovery.c \
    bootloader.c \
    install.c \
    roots.c \
    ui.c \
    verifier.c \
	../../bw-fw-2.3.4/mydroid/external/e2fsprogs/misc/util.c \
	../../bw-fw-2.3.4/mydroid/external/e2fsprogs/misc/default_profile.c \
	fatformat.c \
	mke2fs.c 


LOCAL_SRC_FILES += test_roots.c

LOCAL_MODULE := recovery

LOCAL_FORCE_STATIC_EXECUTABLE := true

RECOVERY_API_VERSION := 3
RECOVERY_VERSION := "RECOVERY_V1.0"
LOCAL_CFLAGS += \
	-DRECOVERY_API_VERSION=$(RECOVERY_API_VERSION) \
	-DRECOVERY_VERSION='$(RECOVERY_VERSION)'
LOCAL_CFLAGS += -O2 -static -W -Wall \
		-DHAVE_UNISTD_H \
		-DHAVE_ERRNO_H \
		-DHAVE_NETINET_IN_H \
		-DHAVE_SYS_IOCTL_H \
		-DHAVE_SYS_MMAN_H \
		-DHAVE_SYS_MOUNT_H \
		-DHAVE_SYS_PRCTL_H \
		-DHAVE_SYS_RESOURCE_H \
		-DHAVE_SYS_SELECT_H \
		-DHAVE_SYS_STAT_H \
		-DHAVE_SYS_TYPES_H \
		-DHAVE_STDLIB_H \
		-DHAVE_STRCASECMP \
		-DHAVE_STRDUP \
		-DHAVE_MMAP \
		-DHAVE_UTIME_H \
		-DHAVE_GETPAGESIZE \
		-DHAVE_LSEEK64 \
		-DHAVE_LSEEK64_PROTOTYPE \
		-DHAVE_EXT2_IOCTLS \
		-DHAVE_LINUX_FD_H \
		-DHAVE_TYPE_SSIZE_T \
		-DHAVE_GETOPT_H

# This binary is in the recovery ramdisk, which is otherwise a copy of root.
# It gets copied there in config/Makefile.  LOCAL_MODULE_TAGS suppresses
# a (redundant) copy of the binary in /system/bin for user builds.
# TODO: Build the ramdisk image in a more principled way.

LOCAL_MODULE_TAGS := eng

LOCAL_STATIC_LIBRARIES :=
ifeq ($(TARGET_RECOVERY_UI_LIB),)
  LOCAL_SRC_FILES += default_recovery_ui.c
else
  LOCAL_STATIC_LIBRARIES += $(TARGET_RECOVERY_UI_LIB)
endif
LOCAL_STATIC_LIBRARIES += libminzip libunz libmtdutils libmincrypt
LOCAL_STATIC_LIBRARIES += libminui libpixelflinger_static libpng libcutils
LOCAL_STATIC_LIBRARIES += libext2fs libext2_blkid libext2_uuid libext2_profile libext2_com_err libext2_e2p 
LOCAL_STATIC_LIBRARIES += libstdc++ libc

include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_SRC_FILES := verifier_test.c verifier.c

LOCAL_MODULE := verifier_test

LOCAL_FORCE_STATIC_EXECUTABLE := true

LOCAL_MODULE_TAGS := tests

LOCAL_STATIC_LIBRARIES := libmincrypt libcutils libstdc++ libc

include $(BUILD_EXECUTABLE)


include $(commands_recovery_local_path)/minui/Android.mk
include $(commands_recovery_local_path)/minzip/Android.mk
include $(commands_recovery_local_path)/mtdutils/Android.mk
include $(commands_recovery_local_path)/tools/Android.mk
include $(commands_recovery_local_path)/edify/Android.mk
include $(commands_recovery_local_path)/updater/Android.mk
include $(commands_recovery_local_path)/applypatch/Android.mk
commands_recovery_local_path :=

endif   # TARGET_ARCH == arm
endif    # !TARGET_SIMULATOR

