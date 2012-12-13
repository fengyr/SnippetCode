define my-dir
$(strip \
		  $(eval md_file_ := $$(lastword $$(MAKEFILE_LIST))) \
		    $(if $(filter $(CLEAR_VARS),$(md_file_)), \
				    $(error LOCAL_PATH must be set before including $$(CLEAR_VARS)) \
					   , \
					       $(patsubst %/,%,$(dir $(md_file_))) \
						      ) \
			 )
endef


LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

PRODUCT := $(shell echo "$(TARGET_PRODUCT)" | cut -c 5-) 
PRODUCT := $(strip $(PRODUCT))

TARGET_OUT := $(ANDROID_PRODUCT_OUT)
TARGET_ROOT_OUT := $(TARGET_OUT)/root
TARGET_RECOVERY_OUT := $(TARGET_OUT)/recovery
TARGET_RECOVERY_ROOT_OUT := $(TARGET_RECOVERY_OUT)/root
INSTALLED_RECOVERYIMAGE_TARGET := $(TARGET_OUT)/recovery.img
INSTALLED_BUILD_PROP_TARGET := $(TARGET_OUT)/system/build.prop
MKBOOTFS := $(ANDROID_BUILD_TOP)/out/host/linux-x86/bin/mkbootfs
MKBOOTIMG := $(ANDROID_BUILD_TOP)/out/host/linux-x86/bin/mkbootimg
MINIGZIP := $(ANDROID_BUILD_TOP)/out/host/linux-x86/bin/minigzip

recovery_initrc := $(LOCAL_PATH)/etc/init.rc
recovery_kernel := $(INSTALLED_KERNEL_TARGET) # same as a non-recovery system
recovery_ramdisk := $(TARGET_OUT)/ramdisk-recovery.img
recovery_ramdisk_uboot := $(TARGET_OUT)/recovery_mmc.img
recovery_build_prop := $(INSTALLED_BUILD_PROP_TARGET)
recovery_binary := $(TARGET_OUT)/system/bin/recovery
recovery_resources_common := $(LOCAL_PATH)/res
#recovery_resources_private := $(strip $(wildcard $(ANDROID_BUILD_TOP)/vendor/sec/$(PRODUCT)/recovery/res))
#recovery_resource_deps := $(shell find $(recovery_resources_common) \
		  $(recovery_resources_private) -type f)

ifeq ($(recovery_resources_private),)
  $(info No private recovery resources for TARGET_DEVICE $(TARGET_DEVICE))
endif

# Keys authorized to sign OTA packages this build will accept.  The
# build always uses test-keys for this; release packaging tools will
# substitute other keys for this one.
OTA_PUBLIC_KEYS := $(ANDROID_BUILD_TOP)/build/target/product/security/testkey.x509.pem

# Generate a file containing the keys that will be read by the
# recovery binary.
RECOVERY_INSTALL_OTA_KEYS := $(TARGET_OUT)/obj/PACKAGING/ota_keys_intermediates/keys
DUMPKEY_JAR := $(ANDROID_BUILD_TOP)/out/host/linux-x86/framework/dumpkey.jar
$(RECOVERY_INSTALL_OTA_KEYS): PRIVATE_OTA_PUBLIC_KEYS := $(OTA_PUBLIC_KEYS)
$(RECOVERY_INSTALL_OTA_KEYS): $(OTA_PUBLIC_KEYS) $(DUMPKEY_JAR)
	@echo "DumpPublicKey: $@ <= $(PRIVATE_OTA_PUBLIC_KEYS)"
	@rm -rf $@
	@mkdir -p $(dir $@)
	java -jar $(DUMPKEY_JAR) $(PRIVATE_OTA_PUBLIC_KEYS) > $@

recoveryimage: $(MKBOOTFS) $(MKBOOTIMG) $(MINIGZIP) \
	$(INSTALLED_RAMDISK_TARGET) \
		$(INSTALLED_BOOTIMAGE_TARGET) \
		$(recovery_binary) \
		$(recovery_initrc) $(recovery_kernel) \
		$(INSTALLED_2NDBOOTLOADER_TARGET) \
		$(recovery_build_prop) $(recovery_resource_deps) \
		$(RECOVERY_INSTALL_OTA_KEYS)
	@echo ----- Making recovery image ------
	rm -rf $(TARGET_RECOVERY_OUT)
	mkdir -p $(TARGET_RECOVERY_OUT)
	mkdir -p $(TARGET_RECOVERY_ROOT_OUT)
	mkdir -p $(TARGET_RECOVERY_ROOT_OUT)/etc
	mkdir -p $(TARGET_RECOVERY_ROOT_OUT)/tmp
	@echo Copying baseline ramdisk...
	cp -R $(TARGET_ROOT_OUT) $(TARGET_RECOVERY_OUT)
	@echo Modifying ramdisk contents...
	cp -f $(recovery_initrc) $(TARGET_RECOVERY_ROOT_OUT)/
	cp -f $(recovery_binary) $(TARGET_RECOVERY_ROOT_OUT)/sbin/
	cp -rf $(recovery_resources_common) $(TARGET_RECOVERY_ROOT_OUT)/
	cp -f $(recovery_resources_common)/images/mg_cal $(TARGET_RECOVERY_ROOT_OUT)/sbin
#	$(foreach item,$(recovery_resources_private), \
	  cp -rf $(item) $(TARGET_RECOVERY_ROOT_OUT)/)
	cp $(RECOVERY_INSTALL_OTA_KEYS) $(TARGET_RECOVERY_ROOT_OUT)/res/keys
	cat $(INSTALLED_DEFAULT_PROP_TARGET) $(recovery_build_prop) \
	        > $(TARGET_RECOVERY_ROOT_OUT)/default.prop
	$(MKBOOTFS) $(TARGET_RECOVERY_ROOT_OUT) | $(MINIGZIP) > $(recovery_ramdisk)
	mkimage -A arm -O linux -T ramdisk -C none -a 0x30800000 -n "recovery" -d $(recovery_ramdisk) $(recovery_ramdisk_uboot)
	@echo make recovery success!!!

