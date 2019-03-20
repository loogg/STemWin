/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.44 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *                      http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"

rt_device_t lcd_device = RT_NULL;


/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void)
{
    if(!lcd_device)
    {
        lcd_device = rt_device_find("lcd");
        if (!lcd_device)
            LOG_E("find %s failed!", "lcd");
        rt_device_open(lcd_device, RT_DEVICE_FLAG_RDWR);
    }
    
    struct rt_device_graphic_info info;
    rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
    
    const LCD_API_COLOR_CONV *color;
    switch(info.pixel_format)
    {
        case RTGRAPHIC_PIXEL_FORMAT_MONO:
            color = GUICC_1;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB565:
            color = GUICC_M565;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB888:
            color = GUICC_M888;
            break;
        default:
            color = GUICC_M565;
            break;
    }
    
    GUI_DEVICE_CreateAndLink(&GUIDRV_Template_API, color, 0, 0);
    //
    // Display driver configuration, required for Lin-driver
    //
    LCD_SetSizeEx(0, info.width, info.height);
    LCD_SetVSizeEx(0, info.width, info.height);

}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *pData)
{
    int r;
    (void)LayerIndex;
    (void)pData;

    switch (Cmd)
    {
    case LCD_X_INITCONTROLLER:
    {
        //
        // Called during the initialization process in order to set up the
        // display controller and put it into operation. If the display
        // controller is not initialized by any external routine this needs
        // to be adapted by the customer...
        //
        // ...
        return 0;
    }
    default:
        r = -1;
    }
    return r;
}

/*************************** End of file ****************************/
