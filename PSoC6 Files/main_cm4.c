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


#include "debugM4.h"
#include "controller.h"
#include "ipc.h"

#define DATALENGTH 6

volatile uint8 ffb CY_SECTION(".sharedsection");

volatile bool cts = true;

ipc_msg_t ipcMsgForCM0 = {              /* IPC structure to be sent to CM4  */
    .clientId = IPC_CM4_TO_CM0_CLIENT_ID,
    .userCode = 0,
    .intrMask = CY_IPC_CYPIPE_INTR_MASK,
    .val = {0,0,0,0,0,0}
};

void CM4_ReleaseCallback(void) {
    cts = true;
}

void config_buttons(void) {
    uint8_t command[] = {0x01, 0x4f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00};
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i < 9; i++) {
        SPI_Write(command[i]);
        Cy_SysLib_Delay(1);
    }
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
}

void enter_config(void) {
    uint8_t command[] = {0x01, 0x43, 0x00, 0x01, 0x00};
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i < 5; i++) {
        SPI_Write(command[i]);
        Cy_SysLib_Delay(1);
    }
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
}

void exit_config(void) {
    uint8_t command[] = {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i < 9; i++) {
        SPI_Write(command[i]);
        Cy_SysLib_Delay(1);
    }
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
}

void get_data(uint8_t * data) {
    uint8_t command[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[5];
    
    if (ffb) {
        command[3] = 0x01;
        command[4] = 0xFF;
    } else {
        command[3] = 0x00;
        command[4] = 0x00;
    }
    
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i<4;i++) {
        SPI_Transfer(&command[i], &buffer[i], 1);
        Cy_SysLib_Delay(1);
    }
    
    SPI_Transfer(&command[4], &buffer[4], 1);
    Cy_SysLib_Delay(1);
    SPI_Transfer(&command[5], &data[2], 1);
    Cy_SysLib_Delay(1);
    SPI_Transfer(&command[6], &data[3], 1);
    Cy_SysLib_Delay(1);
    SPI_Transfer(&command[7], &data[4], 1);
    Cy_SysLib_Delay(1);
    SPI_Transfer(&command[8], &data[5], 1);
    Cy_SysLib_Delay(1);
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
    data[0] = ~buffer[3];
    data[1] = ~buffer[4];
}

void activate_vibration(void) {
    uint8_t command[] = {0x01, 0x4D, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i < 9; i++) {
        SPI_Write(command[i]);

        Cy_SysLib_Delay(1);
    }
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
}

void test_vib(void) {
    uint8_t command[] = {0x01, 0x42, 0x00, 0x01, 0xff, 0, 0, 0, 0};
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i < 9; i++) {
        SPI_Write(command[i]);
        Cy_SysLib_Delay(1);
    }

    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(100);
    
    command[3] = 0;
    command[4] = 0;
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    for (int i = 0; i < 9; i++) {
        SPI_Write(command[i]);
        
        Cy_SysLib_Delay(1);
    }
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
}

void dualshock_mode() {
    uint8_t command[] = { 0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 0);
    Cy_SysLib_Delay(1);
    
    for (int i = 0; i < 9; i++) {
        SPI_Write(command[i]);
        Cy_SysLib_Delay(1);
    }
    Cy_GPIO_Write(Attention_0_PORT, Attention_0_NUM, 1);
    Cy_SysLib_Delay(1);
}

void rumble_off_int(void) {
    NVIC_ClearPendingIRQ(RumbleOffInt_cfg.intrSrc);
    ffb = 0;
}

int main(void) {
    uint8_t current_data[DATALENGTH] = {0,0,0,0,0,0};
    uint8_t last_data[DATALENGTH] = {0,0,0,0,0,0};
    
    uint8_t val_to_send;
    ffb = 0;
    
    Cy_SysInt_Init(&RumbleOffInt_cfg, &rumble_off_int);
    
    NVIC_ClearPendingIRQ(RumbleOffInt_cfg.intrSrc);
    NVIC_EnableIRQ(RumbleOffInt_cfg.intrSrc);
    __enable_irq();
    
    SPI_Start();
    
    Cy_SysLib_Delay(100);
    
    enter_config();
    dualshock_mode();
    activate_vibration();
    config_buttons();
    exit_config();
    
    test_vib();
  
    SPI_ClearRxFifo();
    
    for(;;) {
        get_data(current_data);
        
        if (memcmp(current_data, last_data, 6) != 0) {
            memcpy(last_data, current_data, DATALENGTH);
            while (!cts);
            memcpy(ipcMsgForCM0.val, current_data, DATALENGTH);
            Cy_IPC_Pipe_SendMessage(CY_IPC_EP_CYPIPE_CM0_ADDR, CY_IPC_EP_CYPIPE_CM4_ADDR, (uint32_t *) &ipcMsgForCM0, CM4_ReleaseCallback);
            cts = false;
        }
    }
}
/* [] END OF FILE */
