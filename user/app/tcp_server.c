/**
  *****************************************************************************
  * @file    tcp_server.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   tcp服务端的实现
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
 * 描述  : 接收回调函数
 * 参数  : -
 * 返回  : -
******************************************************************************/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
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
 * 描述  : 客户端接入回调函数
 * 参数  : -
 * 返回  : -
******************************************************************************/
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    printf("tcp client connected\r\n");
    
    printf("ip %d:%d:%d:%d port:%d\r\n",
        *((uint8_t *)&newpcb->remote_ip.addr),
        *((uint8_t *)&newpcb->remote_ip.addr + 1),
        *((uint8_t *)&newpcb->remote_ip.addr + 2),
        *((uint8_t *)&newpcb->remote_ip.addr + 3),
        newpcb->remote_port);
    
    tcp_write(newpcb, "tcp client connected", strlen("tcp client connected"), 0);
    
    /* 注册接收回调函数 */
    tcp_recv(newpcb, tcp_server_recv);

    return ERR_OK;
}

/******************************************************************************
 * 描述  : 创建tcp服务器
 * 参数  : 无
 * 返回  : 无
******************************************************************************/
void tcp_server_init(void)
{
    struct tcp_pcb *tpcb;

    /* 创建tcp控制块 */
    tpcb = tcp_new();

    if (tpcb != NULL)
    {
        err_t err;
        
        /* 绑定端口接收，接收对象为所有ip地址 */
        err = tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);

        if (err == ERR_OK)
        {
            /* 监听 */
            tpcb = tcp_listen(tpcb);

            /* 注册接入回调函数 */
            tcp_accept(tpcb, tcp_server_accept);
            
            printf("tcp server listening\r\n");
            printf("tcp server ip:%d:%d:%d:%d prot:%d\r\n",
                *((uint8_t *)&ipaddr.addr),
                *((uint8_t *)&ipaddr.addr + 1),
                *((uint8_t *)&ipaddr.addr + 2),
                *((uint8_t *)&ipaddr.addr + 3),
                tpcb->local_port);
        }
        else
        {
            memp_free(MEMP_TCP_PCB, tpcb);
            
            printf("can not bind pcb\r\n");
        }

    }
}

/******************************** END OF FILE ********************************/
