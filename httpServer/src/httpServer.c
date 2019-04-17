/*****************************************************************
 ** 文件名 ： httpserver.c
 ** 2015-2017 高新兴科技集团股份有限公司 Co.Ltd
 
 ** 创建人: 骆军城
 ** 日  期: 2018-08-13
 ** 描  述: httpserver模块
 ** 版  本: V1.0

 ** 修改人:
 ** 日  期:
 ** 修改描述:
 ** 版  本: 
******************************************************************/

/*****************************************************************
* 包含头文件
******************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/queue.h>
#include <event.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/buffer_compat.h>
#include <event2/util.h>
#include <event2/event_compat.h>
#include <event2/buffer.h>
#include "cJSON.h"
#include "httpServer.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************
* 宏定义(仅在当前C文件使用的宏定义写在当前C文件中，否则需写在H文件中)
******************************************************************/


/*****************************************************************
* 结构定义(仅在当前C文件使用的结构体写在当前C文件中，否则需写在H文件中)
******************************************************************/


/*****************************************************************
* 全局变量定义
******************************************************************/
int gnEventInitialFlag = 0;

/*****************************************************************
* 静态变量定义
******************************************************************/

/*****************************************************************
* 外部变量声明（如果全局变量没有在其它的H文件声明，引用时需在此处声明，
*如果已在其它H文件声明，则只需包含此H文件即可）
******************************************************************/


/*************************************************
** Function：httpServer_CommonDel
** Description：通用处理回调函数
** Input：无
** Output：无

** Return：-1-失败	0-成功
** Author：骆军城
** Date：2018-11-27

** Modification History：
** Author：
** Date：
** Description：
*************************************************/
void httpServer_CommonDel(struct evhttp_request *req, void *arg) 
{
	int nRet = 0;
	char nlengthBuffer[16]={0};
	char respondBuffer[128]={0};
	stHttpServer *pstHttpServer = (stHttpServer *)arg;

	/*获取POST方法的数据*/
	size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
	char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
	
	printf("%s get post_data, len =%ld\n",__FUNCTION__, post_size);
	printf("RCV:\n%s\n",post_data);

	int nSize = ((post_size<=pstHttpServer->httpd_RcvBufSize)?post_size:(pstHttpServer->httpd_RcvBufSize));
	memset(pstHttpServer->http_RcvBuffer,0,pstHttpServer->httpd_RcvBufSize);
	memcpy(pstHttpServer->http_RcvBuffer, post_data, nSize);

	/*如果用户有注册处理函数则将数据交给用户处理*/
	if(pstHttpServer->defaultDelFunc != NULL)
	{
		nRet = pstHttpServer->defaultDelFunc(req,pstHttpServer,NULL);	
		/*产生回应信息*/
		if(pstHttpServer->respondDelFunc !=NULL){
			nRet = pstHttpServer->respondDelFunc(req,respondBuffer,&nRet);
		}
	}

	//HTTP header
	evutil_snprintf(nlengthBuffer, sizeof(nlengthBuffer)-1, "%lu", strlen(respondBuffer));
	//evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Content-Length",nlengthBuffer);
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf;
	buf = evbuffer_new();
	evbuffer_add_printf(buf, "%s", respondBuffer);
	
	//将封装好的evbuffer 发送给客户端
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	evbuffer_free(buf);
}


/*************************************************
** Function：httpServer_KeepAliveDel
** Description：心跳处理函数
** Input：无
** Output：无

** Return：-1-失败	0-成功
** Author：骆军城
** Date：2018-11-27

** Modification History：
** Author：
** Date：
** Description：
*************************************************/
void httpServer_KeepAliveDel(struct evhttp_request *req, void *arg) 
{
	int nRet = 0;
	char nlengthBuffer[16]={0};
	char respondBuffer[128]={0};
	stHttpServer *pstHttpServer = (stHttpServer *)arg;

	/*获取POST方法的数据*/
	size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
	char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
	
	printf("%s get post_data, len =%ld\n",__FUNCTION__, post_size);
	printf("RCV:\n%s\n",post_data);
	
	int nSize = ((post_size<=pstHttpServer->httpd_RcvBufSize)?post_size:(pstHttpServer->httpd_RcvBufSize));
	memset(pstHttpServer->http_RcvBuffer,0,pstHttpServer->httpd_RcvBufSize);
	memcpy(pstHttpServer->http_RcvBuffer, post_data, nSize);

	/*如果用户有注册处理函数则将数据交给用户处理*/
	if(pstHttpServer->keepAliveDelFunc != NULL)
	{
		nRet = pstHttpServer->keepAliveDelFunc(req,pstHttpServer,NULL);	
		/*产生回应信息*/
		if(pstHttpServer->respondDelFunc !=NULL){
			nRet = pstHttpServer->respondDelFunc(req,respondBuffer,&nRet);
		}
	}

	//HTTP header
	evutil_snprintf(nlengthBuffer, sizeof(nlengthBuffer)-1, "%lu", strlen(respondBuffer));
	//evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Content-Length",nlengthBuffer);
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf;
	buf = evbuffer_new();
	evbuffer_add_printf(buf, "%s", respondBuffer);
	
	//将封装好的evbuffer 发送给客户端
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	evbuffer_free(buf);
}


/*************************************************
** Function：httpServer_FaceInfoDel
** Description：人脸信息处理函数
** Input：无
** Output：无

** Return：-1-失败	0-成功
** Author：骆军城
** Date：2018-11-27

** Modification History：
** Author：
** Date：
** Description：
*************************************************/
void httpServer_FaceInfoDel(struct evhttp_request *req, void *arg) 
{
	int nRet = 0;
	char nlengthBuffer[16]={0};
	char respondBuffer[128]={0};
	stHttpServer *pstHttpServer = (stHttpServer *)arg;

	/*获取POST方法的数据*/
	size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
	char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
	
	printf("%s get post_data, len =%ld\n",__FUNCTION__, post_size);
	printf("RCV:\n%s\n",post_data);
	
	int nSize = ((post_size<=pstHttpServer->httpd_RcvBufSize)?post_size:(pstHttpServer->httpd_RcvBufSize));
	memset(pstHttpServer->http_RcvBuffer,0,pstHttpServer->httpd_RcvBufSize);
	memcpy(pstHttpServer->http_RcvBuffer, post_data, nSize);

	/*如果用户有注册处理函数则将数据交给用户处理*/
	if(pstHttpServer->faceInfoDelFuc != NULL)
	{
		nRet = pstHttpServer->faceInfoDelFuc(req,pstHttpServer,NULL);	
		/*产生回应信息*/
		if(pstHttpServer->respondDelFunc !=NULL){
			nRet = pstHttpServer->respondDelFunc(req,respondBuffer,&nRet);
		}
	}

	//HTTP header
	evutil_snprintf(nlengthBuffer, sizeof(nlengthBuffer)-1, "%lu", strlen(respondBuffer));
	//evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Content-Length",nlengthBuffer);
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf;
	buf = evbuffer_new();
	evbuffer_add_printf(buf, "%s", respondBuffer);
	
	//将封装好的evbuffer 发送给客户端
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	evbuffer_free(buf);
}



/*************************************************
** Function：httpServer_Create
** Description：创建一个http服务器
** Input:无
** Output：

** Return：-1-失败	0-成功
** Author：骆军城
** Date：2018-11-27

** Modification History：
** Author：
** Date：
** Description：
*************************************************/
int httpServer_Create(stHttpServer *pstHttpServer)
{
	//初始化event_init
	event_init();

	printf("httpServer_ListenIP:%s\n",pstHttpServer->httpd_ListenIP);
	printf("httpServer_ListenPort:%d\n",pstHttpServer->httpd_ListenPort);
	printf("httpServer_FaceInfoMatchStr:%s\n",pstHttpServer->httpd_FaceInfoMatchStr);
	printf("httpServer_KeepAliveMatchStr:%s\n",pstHttpServer->httpd_KeepAliveMatchStr);

	pstHttpServer->httpd = evhttp_start(pstHttpServer->httpd_ListenIP, pstHttpServer->httpd_ListenPort);

	evhttp_set_timeout(pstHttpServer->httpd, pstHttpServer->httpd_timeout);
	//特定的URI指定callback
	evhttp_set_cb(pstHttpServer->httpd, pstHttpServer->httpd_FaceInfoMatchStr, httpServer_FaceInfoDel, (void*)pstHttpServer);
	evhttp_set_cb(pstHttpServer->httpd, pstHttpServer->httpd_KeepAliveMatchStr, httpServer_KeepAliveDel, (void*)pstHttpServer);

	/*通用处理回调函数指定*/
	evhttp_set_gencb(pstHttpServer->httpd,httpServer_CommonDel, (void*)pstHttpServer);

	//循环处理events
	event_dispatch();

	evhttp_free(pstHttpServer->httpd);
	return 0;
}

/*************************************************
** Function：httpServer_Remove
** Description：httpServer_Remove
** Input:无
** Output：

** Return：-1-失败	0-成功
** Author：骆军城
** Date：2018-11-27

** Modification History：
** Author：
** Date：
** Description：
*************************************************/
int httpServer_Remove(stHttpServer *pstHttpServer)
{
	return 0;
}


#ifdef __cplusplus
}
#endif


