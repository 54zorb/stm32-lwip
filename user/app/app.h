/**
  *****************************************************************************
  * @file    app.h
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   应用的头文件
  *****************************************************************************
  * @history
  *
  * 1. Date:2018-09-04
  *    Author:Zorb
  *    Modification:建立文件
  *
  *****************************************************************************
  */

#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* 应用的初始化 */
void App_init(void);

/* 应用程序 */
void App_process(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_H__ */

/******************************** END OF FILE ********************************/
