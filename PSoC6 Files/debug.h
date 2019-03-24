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


#ifndef DEBUGM0_H
#define DEBUGM0_H
    


#define DEBUG_ENABLED

   
#ifdef DEBUG_ENABLED


    #include "stdio.h"    

    #define DEBUG_PRINT(...) {printf(__VA_ARGS__); \
                              printf("\r\n");}
#define DEBUG_BLE
#else
#define DEBUG_PRINT
#define DEBUG_BLE
#endif
#endif

/* [] END OF FILE */
