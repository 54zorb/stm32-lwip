/**
  *****************************************************************************
  * @file    udp_server.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   udp����˵�ʵ��
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

/* ����˿ں� */
#define UDP_REMOTE_PORT    8881 /* Զ�˶˿� */
#define UDP_LOCAL_PORT     8880 /* ���ض˿� */

/******************************************************************************
 * ����  : ���ջص�����
 * ����  : -
 * ����  : ��
******************************************************************************/
static void udp_server_receive_callback(void *arg, struct udp_pcb *upcb,
    struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t i;
    
    /* ���ݻش� */
//    udp_sendto(upcb, p, addr, port);
    
    /* ��ӡ���յ������� */
    printf("get msg from %d:%d:%d:%d port:%d:\r\n",
        *((uint8_t *)&addr->addr), *((uint8_t *)&addr->addr + 1),
        *((uint8_t *)&addr->addr + 2), *((uint8_t *)&addr->addr + 3), port);
    
    if (p != NULL)
    {
        for (i = 0; i < p->len; i++)
        {
            printf("%c", *((char *)p->payload + i));
        }
        
        printf("\r\n");
    }
    
    /* �ͷŻ��������� */
    pbuf_free(p);
}

/******************************************************************************
 * ����  : ��ʼ��UDP��������
 * ����  : ��
 * ����  : ��
******************************************************************************/
void udp_server_init(void)
{
    struct udp_pcb *upcb;
    err_t err;

    /* ����udp���ƿ� */
    upcb = udp_new();

    if (upcb)
    {
        /* �󶨶˿ڽ��գ����ն���Ϊ����ip��ַ */
        err = udp_bind(upcb, IP_ADDR_ANY, UDP_LOCAL_PORT);

        if(err == ERR_OK)
        {
            /* ע����ջص����� */
            udp_recv(upcb, udp_server_receive_callback, NULL);
        }
        else
        {
            /* ɾ�����ƿ� */
            udp_remove(upcb);
        }
    }
}

/******************************** END OF FILE ********************************/
