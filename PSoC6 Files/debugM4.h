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

#ifndef DEBUG_H
#define DEBUG_H
   
#ifdef DEBUG_ENABLED


    #include "stdio.h"    

    #define DEBUG_PRINT(...) {printf(__VA_ARGS__); \
                              printf("\r\n");}

#else
#define DEBUG_PRINT
#endif
#endif

/* [] END OF FILE */
