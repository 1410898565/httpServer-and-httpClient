/*****************************************************************
 ** 文件名 ： httpserver.h
 ** 2015-2017 高新兴科技集团股份有限公司 Co.Ltd
 
 ** 创建人: 骆军城
 ** 日  期: 2019-04-13
 ** 描  述: httpserver模块
 ** 版  本: V1.0

 ** 修改人:
 ** 日  期:
 ** 修改描述:
 ** 版  本: 
******************************************************************/

#ifndef __HTTPSERVER_H
#define __HTTPSERVER_H	 
/*****************************************************************
* 头文件
******************************************************************/


/*****************************************************************
* 宏定义
******************************************************************/
#define HTTP_RCV_BUFFER_SIZE (4*1024*1024)

#define MYHTTPD_SIGNATURE		"GxxHttpd v0.1"

/*****************************************************************
* 结构定义(仅在当前C文件使用的结构体写在当前C文件中，否则需写在H文件中)
******************************************************************/
typedef int (*fnHttpCallback)(void *pst,void *pArg0,void *pArg1);


typedef struct _stHttpServerInfo
{
	int httpd_ListenPort;
	int httpd_timeout;
	char httpd_ListenIP[32];

	/*http回调函数*/
	fnHttpCallback faceInfoDelFuc;		/*人脸信息处理回调函数*/
	fnHttpCallback keepAliveDelFunc; 	/*心跳处理回调函数*/
	fnHttpCallback defaultDelFunc;		/*通用处理回调函数*/	
	fnHttpCallback respondDelFunc;		/*处理结果响应回调函数*/

	/*用以指定特定处理回调函数的匹配符*/
	char httpd_KeepAliveMatchStr[32];// 如"/faceinfo"
	char httpd_FaceInfoMatchStr[32];	// 如"/keepalive"
	
	int httpd_RcvBufSize;
	char http_RcvBuffer[HTTP_RCV_BUFFER_SIZE];

	
	struct evhttp *httpd;	//一个http server
	void *pUserStArg;		//指向user结构体
}stHttpServer;



/*****************************************************************
* 函数原型声明
* 函数定义
*注意，编写函数需首先定义所有的局部变量等，不允许在
*函数的中间出现新的变量的定义。
******************************************************************/
int httpServer_Create(stHttpServer *pstHttpServer);
int httpServer_Remove(stHttpServer *pstHttpServer);


#endif



