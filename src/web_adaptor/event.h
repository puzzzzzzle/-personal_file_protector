#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    SEVENT_NULL,

    SEVENT_BIND_FAILED, //wparam: 0-->http, 1-->ftp; lparam: port

    SEVENT_FTP_CONNECT, //identifier-->server���, wparam-->ip, lparam-->port
    SEVENT_FTP_LOGIN,   //identifier-->session���, wparam: 0-->succeed, 1-->failed; lparam-->username
    SEVENT_FTP_QUIT,    //identifier-->session���
    SEVENT_FTP_KILL,    //TODO
    SEVENT_FTP_GET,     //identifier-->session���, wparam-->file, lparam: 0-->start, 1-->complete, 2-->failed
    SEVENT_FTP_PUT,     //ͬ��
    SEVENT_FTP_MKD,     //identifier-->session���, wparam-->dir, lparam:0-->succeed, 1-->failed
    SEVENT_FTP_RMD,     //identifier-->session���, wparam-->dir, lparam:0-->succeed, 1-->failed 
    SEVENT_FTP_DEL,     //identifier-->session���, wparam-->file, lparam:0-->succeed, 1-->failed  
    SEVENT_FTP_RENAME,  ///dentifier-->session���, wparam-->to file, lparam:0-->succeed, 1-->failed  

    SEVENT_HTTP_GET,    //identifier-->session���, wparam-->path, lparam: 0-->start, 1-->complete, 2-->failed
    SEVENT_HTTP_POST,   //identifier-->session���, wparam-->path(lparamΪ1ʱ��Ч), lparam: 0-->start, 1-->complete, 2-->failed

    SEVENT_MAX

} e_sevent;


typedef void (*cb_event_t)(void *context, e_sevent type, void *identifier, unsigned long wparam, unsigned long lparam);

//�ϲ��������ӿ�ע���¼��ص�
void set_event_callback(cb_event_t cbfun, void *context);

//server�ڲ���������ӿ����¼�, �¼��ص��в�Ӧ�к�ʱ�Ĳ���
//TODO ���¼������ڲ�����, �ɵ����߳�������
void send_event(e_sevent type, void *identifier, unsigned long wparam, unsigned long lparam);


#ifdef __cplusplus
}
#endif

#endif // __EVENT_H__

