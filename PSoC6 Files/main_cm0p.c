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
#include "main_ble.h"

int _write(int file, char *ptr, int len) {
    int nChars = 0;

    /* Suppress the compiler warning about an unused variable. */
    if (0 != file) {}
            
    nChars = Cy_SCB_UART_PutArray(UART_HW, ptr, len);
       
    return (nChars);
}

int main(void)
{
    uint8_t buffer[21];
    
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    UART_Start();
    ble_main();
    
   
    
}

/* [] END OF FILE */
