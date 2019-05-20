/** 
 * @file swtype.h
 * @brief ���Ͷ���
 * @author  Dou Hongchen 
 * @date 2007-11-19
 *
 */

#ifndef __SWTYPE_H__
#define __SWTYPE_H__


#ifndef __cplusplus

#ifndef bool
#define bool uint8_t
#endif	

#ifndef true
#define true 1
#endif	

#ifndef false
#define false 0
#endif  

#endif


#ifndef HANDLE
#define HANDLE void*
#endif

#ifndef HANLDE
#define HANLDE HANDLE
#endif


#ifndef SYSHANDLE
#define SYSHANDLE HANDLE
#endif

#ifndef LPVOID
#define LPVOID void*
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


#ifdef SW_OK
#undef SW_OK
#endif

#ifdef SW_ERROR
#undef SW_ERROR
#endif

#ifdef SW_STATUS
#undef SW_STATUS
#endif 

typedef unsigned int					SW_STATUS;  	/**<Sunniwell������������*/
#define SW_OK							0x00000000		/**<���سɹ�*/
#define SW_ERROR 						0xFFFFFFFF 		/**<һ�����*/									
#define SW_ERROR_INVALID_PARAMETER 		0x80000000 		/**<��Ч�Ĳ���*/
#define SW_ERROR_NOMEMORY		 		0x80000001 		/**<�ڴ治��*/
#define SW_ERROR_SYSERR			 		0x80000002 		/**<ϵͳ����*/
#define SW_ERROR_NOTINIT				0x80000003 		/**<û�г�ʼ��*/
#define SW_ERROR_NOTSUPPORT				0x80000004 		/**<��֧��*/
#define SW_ERROR_TIMEOUT				0x80000005 		/**<��ʱ*/

#ifndef NO_WAIT
#define NO_WAIT       0
#endif

#ifndef INFINITE
#define INFINITE -1
#endif

#ifndef WAIT_FOREVER
#define WAIT_FOREVER INFINITE
#endif



#ifndef SOCKET
#define SOCKET int
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif


#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

/**
 * @�����궨��
 */
typedef struct _point{
	
	int x;	/**<X����*/
	int y;	/**<Y����*/
}swpoint_t;

/**
 * @���ο���
 */
typedef struct _rect{
	
	int x;				/**<X����*/
	int y;				/**<Y����*/
	unsigned int width; /**<���*/
	unsigned int height;/**<�߶�*/ 
}swrect_t;

#endif
