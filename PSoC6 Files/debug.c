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

#ifdef DEBUG_ENABLED
    
int _write(int file, char *ptr, int len) {
    int nChars = 0;
    if (0 != file) {} // get rid of compiler warning
    
    nChars = UART_PutArray(ptr, len);
       
    return (nChars);
}
    
/*
void debug_hid(uint32_t event, void * eventParam) {

}

void debug_scps(uint32_t event, void * eventParam) {
    
}

void debug_bas(uint32_t event, void * eventParam) {

}

void debug_ble(uint32_t event, void * eventParam) {

}
*/
#endif
/* [] END OF FILE */
