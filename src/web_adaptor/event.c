#include <stdio.h>
#include "event.h"


static cb_event_t m_cbfun = NULL;
static void *m_context = NULL;


//�ϲ��������ӿ�ע���¼��ص�
void set_event_callback(cb_event_t cbfun, void *context)
{
    m_cbfun = cbfun;
    m_context = context;
}


//server�ڲ���������ӿ����¼�
void send_event(e_sevent type, void *identifier, unsigned long wparam, unsigned long lparam)
{
    if (m_cbfun == NULL)
        return;
    m_cbfun(m_context, type, identifier, wparam, lparam);
}

