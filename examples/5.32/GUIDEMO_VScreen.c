/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
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
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              STMicroelectronics International NV
Licensed SEGGER software: emWin
License number:           GUI-00429
License model:            Buyout SRC [Buyout Source Code License, signed November 29th 2012]
Licensed product:         -
Licensed platform:        STMs ARM Cortex-M based 32 BIT CPUs
Licensed number of seats: -
----------------------------------------------------------------------
File        : GUIDEMO_VScreen.c
Purpose     : Virtual screen demo
----------------------------------------------------------------------
*/

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_VSCREEN)

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _Loop
*/
static int _Loop(int y, int d, int n, int Delay) {
  do {
    y += d;
    GUI_SetOrg(0, y);
    GUI_Delay(Delay);
    if (GUIDEMO_CheckCancel()) {
      return 1;
    }
  } while (--n);
  return 0;
}

/*********************************************************************
*
*       _DemoVScreen
*/
static void _DemoVScreen(void) {
  GUI_RECT Rect;
  unsigned j;
  int      xSize;
  int      ySize;
  int      n;
  int      aDelay[] = { 20, 5 };

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  GUI_SetFont(&GUI_FontComic24B_ASCII);
  GUI_SetTextMode(GUI_TM_TRANS);
  Rect.x0 = 0;
  Rect.y0 = 0;
  Rect.x1 = xSize - 1;
  Rect.y1 = ySize - 1;
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRect("Default screen", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
  GUI_Delay(250);
  Rect.y0 += ySize;
  Rect.y1 += ySize;
  GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 0xFFFFFF, 0x000000);
  Rect.y1 -= ySize / 3;
  GUI_SetColor(GUI_DARKGREEN);
  GUI_DispStringInRect("Here is the virtual screen", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
  n = ySize / 5;
  for (j = 0; j < GUI_COUNTOF(aDelay); j++) {
    if (_Loop(0, 5, n, aDelay[j]) == 1) {
      return;
    }
    GUI_SetOrg(0, ySize);
    GUI_Delay(250);
    if (_Loop(ySize, -5, n, aDelay[j]) == 1) {
      return;
    }
    GUI_SetOrg(0, 0);
    GUI_Delay(250);
    if (GUIDEMO_CheckCancel()) {
      return;
    }
  }
  for (j = 0; j < 5; j++) {
    GUI_SetOrg(0, ySize);
    GUI_Delay(200);
    GUI_SetOrg(0, 0);
    GUI_Delay(200);
    if (GUIDEMO_CheckCancel()) {
      return;
    }
  }
  GUIDEMO_Delay(1000);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_VScreen
*/
void GUIDEMO_VScreen(void) {
  int vySize;
  int ySize;

  ySize  = LCD_GetYSize();
  vySize = LCD_GetVYSize();
  if (vySize < (ySize * 2)) {
    GUIDEMO_ConfigureDemo("Virtual Screen", "Works only with a virtual screen with at least twice the ySize of the display.\nDemo will be skipped...", GUIDEMO_SHOW_CURSOR | GUIDEMO_SHOW_INFO | GUIDEMO_SHOW_CONTROL);
    return;
  }
  GUIDEMO_ConfigureDemo("Virtual Screen", "Demonstrates how to use\nvirtual screens", GUIDEMO_SHOW_CURSOR | GUIDEMO_SHOW_INFO | GUIDEMO_SHOW_CONTROL);
  GUIDEMO_DrawBk();
  _DemoVScreen();
  GUI_SetOrg(0, 0);
}

#else

void GUIDEMO_VScreen_C(void);
void GUIDEMO_VScreen_C(void) {}

#endif  // SHOW_GUIDEMO_VSCREEN

/*************************** End of file ****************************/
