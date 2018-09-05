/**
  *****************************************************************************
  * @file    tcp_server.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   tcp����˵�ʵ��
  *****************************************************************************
  * @history
  *
  * 1. Date:2018-09-04
  *    Author:Zorb
  *    Modification:�����ļ�
  *
  *****************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "lwip.h"
#include "tcp.h"
#include "string.h"

/* ����˿ں� */
#define TCP_REMOTE_PORT    8881 /* Զ�˶˿� */
#define TCP_LOCAL_PORT     8880 /* ���ض˿� */

/******************************************************************************
 * ����  : ���ջص�����
 * ����  : -
 * ����  : -
******************************************************************************/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
    uint32_t i;
    
    /* ���ݻش� */
    //tcp_write(tpcb, p->payload, p->len, 1);
    
    if (p != NULL)
    {
        struct pbuf *ptmp = p;
        
        /* ��ӡ���յ������� */
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
        
        /* �ͷŻ��������� */
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
 * ����  : �ͻ��˽���ص�����
 * ����  : -
 * ����  : -
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
    
    /* ע����ջص����� */
    tcp_recv(newpcb, tcp_server_recv);

    return ERR_OK;
}

/******************************************************************************
 * ����  : ����tcp������
 * ����  : ��
 * ����  : ��
******************************************************************************/
void tcp_server_init(void)
{
    struct tcp_pcb *tpcb;

    /* ����tcp���ƿ� */
    tpcb = tcp_new();

    /* �󶨶˿ڽ��գ����ն���Ϊ����ip��ַ */
    tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);

    /* ���� */
    tpcb = tcp_listen(tpcb);

    /* ע�����ص����� */
    tcp_accept(tpcb, tcp_server_accept);
    
    printf("tcp server listening\r\n");
    printf("tcp server ip:%d:%d:%d:%d prot:%d\r\n",
        *((uint8_t *)&ipaddr.addr),
        *((uint8_t *)&ipaddr.addr + 1),
        *((uint8_t *)&ipaddr.addr + 2),
        *((uint8_t *)&ipaddr.addr + 3),
        tpcb->local_port);
}

/******************************** END OF FILE ********************************/
