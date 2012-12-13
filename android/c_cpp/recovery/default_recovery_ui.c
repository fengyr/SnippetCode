/*
 * Copyright (C) 2009 The Android Open Source Project
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

#include <linux/input.h>

#include "recovery_ui.h"
#include "common.h"

#if 1
char* MENU_HEADERS[] = { "CL1 system recovery utility",
                         "",
                         NULL };

/* <--LH_SWRD_CL1_Mervins@2011.08.23--> */
/*
char* MENU_ITEMS[] = { "reboot system now",
                       "Update form SD card",
                       "Factory reset",
                       "wipe cache partition",
                       NULL };*/
char* MENU_ITEMS[] = { "reboot system now",
                       "wipe data(factory reset)",
                       "wipe cache",
                       /*"update touch",*/
                       "touch calibration",/* <--LH_SWRD_CL1_Mervins@2011.09.19:add for touch calibration. --> */
                       NULL };
#endif

int device_recovery_start() {
    return 0;
}

int device_toggle_display(volatile char* key_pressed, int key_code) {
    return key_code == KEY_POWER;
}

int device_reboot_now(volatile char* key_pressed, int key_code) {
    return 0;
}

int device_handle_key(int key_code, int visible) {
    if (visible) {
        switch (key_code) {
            case KEY_DOWN:
            case KEY_VOLUMEDOWN:
                return HIGHLIGHT_DOWN;

            case KEY_UP:
            case KEY_VOLUMEUP:
                return HIGHLIGHT_UP;

            case KEY_ENTER:
                return SELECT_ITEM;

			case USB_IN:
				return USB_IN_AC;	
			case USB_OUT:
				return USB_OUT_AC;	
        }
    }

    return NO_ACTION;
}

int device_perform_action(int which) {
    return which;
}

int device_wipe_data() {
    return 0;
}
