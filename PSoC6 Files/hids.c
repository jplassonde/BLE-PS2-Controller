/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"
#include "hids.h"
#include "debug.h"

uint8 ffb CY_SECTION(".sharedsection");

void hids_callback(uint32_t event, void * eventParam) {
    
    switch (event) { 
        case CY_BLE_EVT_HIDSS_NOTIFICATION_ENABLED: 
            DEBUG_PRINT("Notif enabled");
        break;
        
        case CY_BLE_EVT_HIDSS_NOTIFICATION_DISABLED:
            DEBUG_PRINT("Notif disabled");
        break;
        
        case CY_BLE_EVT_HIDSS_REPORT_MODE_ENTER:
            DEBUG_PRINT("REPORT MODE ENTER");
        break;
            
        case CY_BLE_EVT_HIDSS_SUSPEND:
            DEBUG_PRINT("HID SUSPEND");
        break;
            
        case CY_BLE_EVT_HIDSS_EXIT_SUSPEND:
            DEBUG_PRINT("EXIT SUSPEND");
        break;
         
        case CY_BLE_EVT_HIDSS_REPORT_WRITE_CHAR: {
            cy_stc_ble_hids_char_value_t sentStruct = *(cy_stc_ble_hids_char_value_t*)eventParam;

            switch (sentStruct.value->val[0]) {
                case 1:
                   DEBUG_PRINT("Device ID: intId: %x, extId: %x, fbegin: %x", sentStruct.value->val[1], sentStruct.value->val[2], sentStruct.value->val[3]);
                break;
                case 2:
                    if (sentStruct.value->val[2] != 0) {
                        ffb = 1;
                        RumbleCounter_SetPeriod(50);
                        RumbleCounter_Start();
                    }
                    break;
                case 3:
                    DEBUG_PRINT("Get Eff Status");
                case 4:
                    DEBUG_PRINT("Get FFB State");
                    break;
                case 5:
                    DEBUG_PRINT("Send FFB Command: %02x%02x - devId: %x", sentStruct.value->val[1], sentStruct.value->val[2], sentStruct.value->val[3]);
                    break;
                case 6:
                    DEBUG_PRINT("Set Gain: %02x%02x%02x", sentStruct.value->val[1], sentStruct.value->val[2], sentStruct.value->val[3]);
                    break;
                case 7:
                    DEBUG_PRINT("StartEffect");
                    break;
                case 8:
                    DEBUG_PRINT("StopEffect");
                    break;
                default:
                    DEBUG_PRINT("Unhandled DIE command: %02x", sentStruct.value->val[0]);
            }
        }
        break;
            
        default:
            DEBUG_PRINT("HID UNKNOWN");
            break; 
    }
}

/* [] END OF FILE */
