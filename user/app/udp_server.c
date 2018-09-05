/**
  *****************************************************************************
  * @file    udp_server.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   udp服务端的实现
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
#include "udp.h"

/* 定义端口号 */
#define UDP_REMOTE_PORT    8881 /* 远端端口 */
#define UDP_LOCAL_PORT     8880 /* 本地端口 */

/******************************************************************************
 * 描述  : 接收回调函数
 * 参数  : -
 * 返回  : 无
******************************************************************************/
static void udp_server_receive_callback(void *arg, struct udp_pcb *upcb,
    struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t i;
    
    /* 数据回传 */
//    udp_sendto(upcb, p, addr, port);
    
    /* 打印接收到的数据 */
    printf("get msg from %d:%d:%d:%d port:%d:\r\n",
        *((uint8_t *)&addr->addr), *((uint8_t *)&addr->addr + 1),
        *((uint8_t *)&addr->addr + 2), *((uint8_t *)&addr->addr + 3), port);
    
    if (p != NULL)
    {
        struct pbuf *ptmp = p;
        
        while(ptmp != NULL)
        {
            for (i = 0; i < p->len; i++)
            {
                printf("%c", *((char *)p->payload + i));
            }
            
            ptmp = p->next;
        }
        
        
        printf("\r\n");
    }
    
    /* 释放缓冲区数据 */
    pbuf_free(p);
}

/******************************************************************************
 * 描述  : 初始化UDP服务器端
 * 参数  : 无
 * 返回  : 无
******************************************************************************/
void udp_server_init(void)
{
    struct udp_pcb *upcb;
    err_t err;

    /* 创建udp控制块 */
    upcb = udp_new();

    if (upcb)
    {
        /* 绑定端口接收，接收对象为所有ip地址 */
        err = udp_bind(upcb, IP_ADDR_ANY, UDP_LOCAL_PORT);

        if(err == ERR_OK)
        {
            /* 注册接收回调函数 */
            udp_recv(upcb, udp_server_receive_callback, NULL);
        }
        else
        {
            /* 删除控制块 */
            udp_remove(upcb);
            
            printf("can not bind pcb\r\n");
        }
    }
}

/******************************** END OF FILE ********************************/
