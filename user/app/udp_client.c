/**
  *****************************************************************************
  * @file    udp_client.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   udp�ͻ��˵�ʵ��
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
#include "udp.h"
#include "string.h"

/* ����˿ں� */
#define UDP_REMOTE_PORT    8881 /* Զ�˶˿� */
#define UDP_LOCAL_PORT     8880 /* ���ض˿� */

/* udp���ƿ� */
static struct udp_pcb *upcb;

/******************************************************************************
 * ����  : ���ջص�����
 * ����  : -
 * ����  : ��
******************************************************************************/
static void udp_receive_callback(void *arg, struct udp_pcb *upcb,
    struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t i;
    
    /* ���ݻش� */
//    udp_send(upcb, p);
//    udp_sendto(upcb, p, addr, port);
    
    /* ��ӡ���յ������� */
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
    
    /* �ͷŻ��������� */
    pbuf_free(p);
}

/******************************************************************************
 * ����  : ����udp����
 * ����  : (in)pData �������ݵ�ָ��
 * ����  : ��
******************************************************************************/
void udp_client_send(char *pData)
{
    struct pbuf *p;
    
    /* ���仺�����ռ� */
    p = pbuf_alloc(PBUF_TRANSPORT, strlen(pData), PBUF_POOL);
    
    if (p != NULL)
    {
        /* ��仺�������� */
        pbuf_take(p, pData, strlen(pData));

        /* ����udp���� */
        udp_send(upcb, p);

        /* �ͷŻ������ռ� */
        pbuf_free(p);
    }
}

/******************************************************************************
 * ����  : ����udp�ͻ���
 * ����  : ��
 * ����  : ��
******************************************************************************/
void udp_client_init(void)
{
    ip_addr_t serverIP;
    ip_addr_t clientIP;
    err_t err;

    IP4_ADDR(&serverIP, 192, 168, 2, 194);
    IP4_ADDR(&clientIP, 192, 168, 2, 66);

    /* ����udp���ƿ� */
    upcb = udp_new();

    if (upcb!=NULL)
    {
        /* ���ñ��ض˿� */
        upcb->local_port = UDP_LOCAL_PORT;
        
        /* ���÷�����IP�Ͷ˿� */
        err= udp_connect(upcb, &serverIP, UDP_REMOTE_PORT);

        if (err == ERR_OK)
        {
            /* ע����ջص����� */
            udp_recv(upcb, udp_receive_callback, NULL);
            
            /* ����udp���� */
            udp_client_send("udp client connected");
        }
    }
}

/******************************** END OF FILE ********************************/
