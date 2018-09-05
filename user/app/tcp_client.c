/**
  *****************************************************************************
  * @file    tcp_client.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   tcp�ͻ��˵�ʵ��
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
 * ����  : ���ݽ��ջص�����
 * ����  : -
 * ����  : -
******************************************************************************/
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
    uint32_t i;
    
    /* ���ݻش� */
    //tcp_write(tpcb, p->payload, p->len, 1);
    
    if (p != NULL)
    {
        /* ��ӡ���յ������� */
        printf("get msg from %d:%d:%d:%d port:%d:\r\n",
            *((uint8_t *)&tpcb->remote_ip.addr),
            *((uint8_t *)&tpcb->remote_ip.addr + 1),
            *((uint8_t *)&tpcb->remote_ip.addr + 2),
            *((uint8_t *)&tpcb->remote_ip.addr + 3),
            tpcb->remote_port);
        
        for (i = 0; i < p->len; i++)
        {
            printf("%c", *((char *)p->payload + i));
        }
        
        printf("\r\n");
        
        /* �ͷŻ��������� */
        pbuf_free(p);
    }
    else if (err == ERR_OK)
    {
        printf("tcp client closed\r\n");
        
        return tcp_close(tpcb);
    }

    return ERR_OK;
}

/******************************************************************************
 * ����  : ���ӷ������ص�����
 * ����  : -
 * ����  : -
******************************************************************************/
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    printf("tcp client connected\r\n");
    
    tcp_write(tpcb, "tcp client connected", strlen("tcp client connected"), 0);

    /* ע����ջص����� */
    tcp_recv(tpcb, tcp_client_recv);

    return ERR_OK;
}

/******************************************************************************
 * ����  : ����tcp�ͻ���
 * ����  : ��
 * ����  : ��
******************************************************************************/
void tcp_client_init(void)
{
    struct tcp_pcb *tpcb;
    ip_addr_t serverIp;

    /* ������IP */
    IP4_ADDR(&serverIp, 192, 168, 2, 194);

    /* ����tcp���ƿ� */
    tpcb = tcp_new();

    /* �󶨱��ض˺ź�IP��ַ */
    tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);

    if (tpcb != NULL)
    {
        /* ���ӷ����� */
        tcp_connect(tpcb, &serverIp, TCP_REMOTE_PORT, tcp_client_connected);
    }
}

/******************************** END OF FILE ********************************/
