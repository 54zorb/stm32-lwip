/**
  *****************************************************************************
  * @file    ethernet_test.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   以太网帧数据发送与接收测试的实现
  *****************************************************************************
  * @history
  *
  * 1. Date:2018-09-04
  *    Author:Zorb
  *    Modification:建立文件
  *
  *****************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "lwip.h"

/******************************************************************************
 * 描述  : 以太网帧发送测试1
 * 参数  : 无
 * 返回  : 无
******************************************************************************/
void ethernet_sendtest1(void)
{
    uint8_t frame_data[] =
    {
        /* 以太网帧格式 */
        0x50,0xFA,0x84,0x15,0x3C,0x3C,                            /* 远端MAC */
        0x0,0x80,0xE1,0x0,0x0,0x0,                                /* 本地MAC */
        0x8,0x0,                                                  /* ip类型 */
        0x45,0x0,0x0,0x26/*l*/,0x0,0x0,0x0,0x0,0xFF,0x11,0x0,0x0, /* UDP报头 */
        0xC0,0xA8,0x2,0x8,                                        /* 本地IP */
        0xC0,0xA8,0x2,0xC2,                                       /* 远端IP */
        0x22,0xB0,                                                /* 本地端口 */
        0x22,0xB1,                                                /* 远端端口 */
        0x0,0x12,                                                 /* UDP长度 */
        0x0,0x0,                                                  /* UDP校验和 */
        0x68,0x65,0x6C,0x6C,0x6F,0x20,0x7A,0x6F,0x72,0x62         /* 数据 */
    };
        
    struct pbuf *p;
    
    /* 分配缓冲区空间 */
    p = pbuf_alloc(PBUF_TRANSPORT, 0x26 + 14, PBUF_POOL);
    
    if (p != NULL)
    {
        /* 填充缓冲区数据 */
        pbuf_take(p, frame_data, 0x26 + 14);

        /* 把数据直接通过底层发送 */
        gnetif.linkoutput(&gnetif, p);

        /* 释放缓冲区空间 */
        pbuf_free(p);
    }
}

/******************************************************************************
 * 描述  : 以太网帧发送测试2
 * 参数  : 无
 * 返回  : 无
******************************************************************************/
void ethernet_sendtest2(void)
{
    uint8_t dstAddr[6] = {0x50,0xFA,0x84,0x15,0x3C,0x3C};         /* 远端MAC */
    
    uint8_t frame_data[] =
    {
        /* UDP帧格式 */
        0x45,0x0,0x0,0x26/*l*/,0x0,0x0,0x0,0x0,0xFF,0x11,0x0,0x0, /* UDP报头 */
        0xC0,0xA8,0x2,0x8,                                        /* 本地IP */
        0xC0,0xA8,0x2,0xC2,                                       /* 远端IP */
        0x22,0xB0,                                                /* 本地端口 */
        0x22,0xB1,                                                /* 远端端口 */
        0x0,0x12,                                                 /* UDP长度 */
        0x0,0x0,                                                  /* UDP校验和 */
        0x68,0x65,0x6C,0x6C,0x6F,0x20,0x7A,0x6F,0x72,0x62         /* 数据 */
    };
        
    struct pbuf *p;
    
    /* 分配缓冲区空间 */
    p = pbuf_alloc(PBUF_TRANSPORT, 0x26, PBUF_POOL);
    
    if (p != NULL)
    {
        /* 填充缓冲区数据 */
        pbuf_take(p, frame_data, 0x26);
        
        /* 把数据进行以太网封装，再通过底层发送 */
        ethernet_output(&gnetif, p, (const struct eth_addr*)gnetif.hwaddr,
            (const struct eth_addr*)dstAddr, ETHTYPE_IP);

        /* 释放缓冲区空间 */
        pbuf_free(p);
    }
}

/******************************** END OF FILE ********************************/
