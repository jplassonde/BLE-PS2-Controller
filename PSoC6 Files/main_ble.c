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
#include "debug.h"
#include "bond.h"
#include "hids.h"
#include "bas.h"
#include "scan_service.h"
#include "ipc.h"
#include "stdio.h"

void ble_callback(uint32_t event, void * eventParam);    

cy_stc_ble_conn_handle_t appConnHandle;

void CM0_MessageCallback(uint32_t *msg) {
    uint8_t array[6];
 
    cy_en_ble_api_result_t apiResult;
    ipc_msg_t *ipcMsgFromCM4;
    if (msg != NULL && Cy_BLE_GetConnectionState(appConnHandle) == CY_BLE_CONN_STATE_CONNECTED) {
        cy_en_ble_api_result_t result;
        ipcMsgFromCM4 = (ipc_msg_t *) msg;
        memcpy(array, ipcMsgFromCM4->val, 6);
        result = Cy_BLE_HIDSS_SendNotification(appConnHandle, CY_BLE_HUMAN_INTERFACE_DEVICE_SERVICE_INDEX, 
                                      CY_BLE_HUMAN_INTERFACE_DEVICE_REPORTIN, 6, array);
        if (result != CY_BLE_SUCCESS) {
            char buffer[20];
            snprintf(buffer, 20, "Error code: %d\n\r", result);
            UART_PutString(buffer);
        }
    }
}

void ble_main(void) {
    Cy_BLE_HIDS_RegisterAttrCallback(hids_callback);
    Cy_BLE_BAS_RegisterAttrCallback(battery_service_callback);
    Cy_BLE_SCPS_RegisterAttrCallback(scan_service_callback);
    Cy_BLE_Start(ble_callback);
    Cy_IPC_Pipe_RegisterCallback(CY_IPC_EP_CYPIPE_ADDR, CM0_MessageCallback, IPC_CM4_TO_CM0_CLIENT_ID);    
    
    while (1) {
        Cy_BLE_ProcessEvents();
    }
}

void ble_callback(uint32_t event, void * eventParam) {
    
    static cy_stc_ble_gap_sec_key_info_t key_config = {
            .localKeysFlag    = CY_BLE_GAP_SMP_INIT_ENC_KEY_DIST | 
                                CY_BLE_GAP_SMP_INIT_IRK_KEY_DIST | 
                                CY_BLE_GAP_SMP_INIT_CSRK_KEY_DIST,
            .exchangeKeysFlag = CY_BLE_GAP_SMP_INIT_ENC_KEY_DIST | 
                                CY_BLE_GAP_SMP_INIT_IRK_KEY_DIST | 
                                CY_BLE_GAP_SMP_INIT_CSRK_KEY_DIST |
                                CY_BLE_GAP_SMP_RESP_ENC_KEY_DIST |
                                CY_BLE_GAP_SMP_RESP_IRK_KEY_DIST |
                                CY_BLE_GAP_SMP_RESP_CSRK_KEY_DIST,
        };
    
    switch (event) {
        
        case CY_BLE_EVT_STACK_ON:
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
            Cy_BLE_GAP_GenerateKeys(&key_config);
            DEBUG_PRINT("Advertisement started, key generated.");
            break;
            
        case CY_BLE_EVT_GAP_AUTH_REQ:
            cy_ble_configPtr->authInfo[CY_BLE_SECURITY_CONFIGURATION_0_INDEX].bdHandle = ((cy_stc_ble_gap_auth_info_t *)eventParam)->bdHandle;
            
            if (Cy_BLE_GAPP_AuthReqReply(&cy_ble_configPtr->authInfo[CY_BLE_SECURITY_CONFIGURATION_0_INDEX]) != CY_BLE_SUCCESS) {
                DEBUG_PRINT("AuthReqReply failed");
            } else {
                DEBUG_PRINT("AuthReqReply success");
            }
            break;
            
        case CY_BLE_EVT_GAP_DEVICE_CONNECTED:
            if(App_IsDeviceInBondList((*(cy_stc_ble_gap_connected_param_t *)eventParam).bdHandle) == 0u) {
               key_config.SecKeyParam.bdHandle = (*(cy_stc_ble_gap_connected_param_t *)eventParam).bdHandle;
               if (Cy_BLE_GAP_SetSecurityKeys(&key_config) != CY_BLE_SUCCESS) {
                    DEBUG_PRINT("Security key error");
                }
            }
            DEBUG_PRINT("Device Connected.");
            break;
        
        case CY_BLE_EVT_GAP_KEYS_GEN_COMPLETE:
            key_config.SecKeyParam = (*(cy_stc_ble_gap_sec_key_param_t *)eventParam);
            Cy_BLE_GAP_SetIdAddress(&cy_ble_deviceAddress);
            DEBUG_PRINT("Key generation is complete.");
            break;
        
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
            DEBUG_PRINT("Device disconnected.");
            break;
        
        case CY_BLE_EVT_GATT_CONNECT_IND:
            appConnHandle = *(cy_stc_ble_conn_handle_t *)eventParam;
            DEBUG_PRINT("GATT Connect Ind.");
            break;
            
        case CY_BLE_EVT_GATTS_XCNHG_MTU_REQ: {
                cy_stc_ble_gatt_xchg_mtu_param_t mtu = {
                    .connHandle = ((cy_stc_ble_gatt_xchg_mtu_param_t *)eventParam)->connHandle };
                Cy_BLE_GATT_GetMtuSize(&mtu);
            }   
            break;

        case CY_BLE_EVT_SET_DEVICE_ADDR_COMPLETE:
            Cy_BLE_GAP_GetBdAddress();
            break;    
        
        case CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ:
            {
                cy_stc_ble_gatts_char_val_read_req_t param = *(cy_stc_ble_gatts_char_val_read_req_t*)eventParam;
                DEBUG_PRINT("Read, attr handle: %x Error: %x", param.attrHandle, param.gattErrorCode);
                
            }
            break;
        default:
            DEBUG_PRINT("Event: 0x%x", event);
            break;
    }
    
}


/* [] END OF FILE */
