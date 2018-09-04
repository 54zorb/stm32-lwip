/**
  *****************************************************************************
  * @file    app.h
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   Ӧ�õ�ͷ�ļ�
  *****************************************************************************
  * @history
  *
  * 1. Date:2018-09-04
  *    Author:Zorb
  *    Modification:�����ļ�
  *
  *****************************************************************************
  */

#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* Ӧ�õĳ�ʼ�� */
void App_init(void);

/* Ӧ�ó��� */
void App_process(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_H__ */

/******************************** END OF FILE ********************************/
