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

/* [] END OF FILE */
#ifndef IPC_DEF_H
#define IPC_DEF_H	
    
    #include <stdint.h>
    
    
    #define IPC_CM0_TO_CM4_CLIENT_ID        0
    #define IPC_CM4_TO_CM0_CLIENT_ID        1
    
    typedef struct __attribute__((packed, aligned(4)))
        {
            uint8_t     clientId;
            uint8_t     userCode;
            uint16_t    intrMask;
            uint8_t    val[6];
        } ipc_msg_t;
    
#endif
        