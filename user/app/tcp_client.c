/**
  *****************************************************************************
  * @file    tcp_client.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   tcp客户端的实现
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
#include "tcp.h"
#include "string.h"

/* 定义端口号 */
#define TCP_REMOTE_PORT    8881 /* 远端端口 */
#define TCP_LOCAL_PORT     8880 /* 本地端口 */

/******************************************************************************
 * 描述  : 数据接收回调函数
 * 参数  : -
 * 返回  : -
******************************************************************************/
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
    uint32_t i;
    
    /* 数据回传 */
    //tcp_write(tpcb, p->payload, p->len, 1);
    
    if (p != NULL)
    {
        struct pbuf *ptmp = p;
        
        /* 打印接收到的数据 */
        printf("get msg from %d:%d:%d:%d port:%d:\r\n",
            *((uint8_t *)&tpcb->remote_ip.addr),
            *((uint8_t *)&tpcb->remote_ip.addr + 1),
            *((uint8_t *)&tpcb->remote_ip.addr + 2),
            *((uint8_t *)&tpcb->remote_ip.addr + 3),
            tpcb->remote_port);
        
        while(ptmp != NULL)
        {
            for (i = 0; i < p->len; i++)
            {
                printf("%c", *((char *)p->payload + i));
            }
            
            ptmp = p->next;
        }
        
        printf("\r\n");
        
        tcp_recved(tpcb, p->tot_len);
        
        /* 释放缓冲区数据 */
        pbuf_free(p);
    }
    else if (err == ERR_OK)
    {
        printf("tcp client closed\r\n");
        
        tcp_recved(tpcb, p->tot_len);
        
        return tcp_close(tpcb);
    }

    return ERR_OK;
}

/******************************************************************************
 * 描述  : 连接服务器回调函数
 * 参数  : -
 * 返回  : -
******************************************************************************/
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    printf("tcp client connected\r\n");
    
    tcp_write(tpcb, "tcp client connected", strlen("tcp client connected"), 0);

    /* 注册接收回调函数 */
    tcp_recv(tpcb, tcp_client_recv);

    return ERR_OK;
}

/******************************************************************************
 * 描述  : 创建tcp客户端
 * 参数  : 无
 * 返回  : 无
******************************************************************************/
void tcp_client_init(void)
{
    struct tcp_pcb *tpcb;
    ip_addr_t serverIp;

    /* 服务器IP */
    IP4_ADDR(&serverIp, 192, 168, 2, 194);

    /* 创建tcp控制块 */
    tpcb = tcp_new();
    
    if (tpcb != NULL)
    {
        err_t err;
        
        /* 绑定本地端号和IP地址 */
        err = tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);

        if (err == ERR_OK)
        {
            /* 连接服务器 */
            tcp_connect(tpcb, &serverIp, TCP_REMOTE_PORT, tcp_client_connected);
        }
        else
        {
            memp_free(MEMP_TCP_PCB, tpcb);
            
            printf("can not bind pcb\r\n");
        }
    }
}

/******************************** END OF FILE ********************************/
