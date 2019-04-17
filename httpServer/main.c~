#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "cJSON.h"
#include "httpServer.h"

#define HTTP_RESPOND_ERROR			"Error"
#define HTTP_RESPOND_OK				"OK"
#define HTTP_LISTEN_IP				"127.0.0.1"
#define HTTP_LISTEN_PORT			6666
#define HTTP_CONNECT_TIMEOUT			20
#define HTTP_CURL_FACEDEL_PATH			"face"
#define HTTP_CURL_HEARTDEL_PATH			"heart"


int http_CommonCallfunc(void *pst,void *pArg0,void *pArg1)
{
	printf("%s been called,at %d line\n",__FUNCTION__,__LINE__);
	return 0;
}

int http_FaceCallfunc(void *pst,void *pArg0,void *pArg1)
{
	printf("%s been called,at %d line\n",__FUNCTION__,__LINE__);
	return 0;
}

int http_KeepAliveCallfunc(void *pst,void *pArg0,void *pArg1)
{
	printf("%s been called,at %d line\n",__FUNCTION__,__LINE__);
	return 0;
}


int http_RespondCodeCreate(void *pst,void *pArg0,void *pArg1)
{
	int nOption = -1;
	char repStatus[32]={0};

	if(pArg0 ==NULL || pArg1 == NULL){
		printf("%s func input para error at (%d) line\n",__FUNCTION__,__LINE__);
		return -1;
	}

	nOption = *((int*)pArg1);

	if(nOption>=0)
		memcpy(repStatus,HTTP_RESPOND_OK,strlen(HTTP_RESPOND_OK));
	else
		memcpy(repStatus,HTTP_RESPOND_ERROR,strlen(HTTP_RESPOND_ERROR));

	//给前段回复一个响应结果
	cJSON*response_root = cJSON_CreateObject();
	cJSON_AddStringToObject(response_root, "Result", repStatus);
	cJSON_AddNumberToObject(response_root, "ErrorCode", nOption);
	char *response_str = cJSON_Print(response_root);
	/*拷贝数据*/
	memcpy((char*)pArg0,response_str,strlen(response_str));

	cJSON_Delete(response_root);
	free(response_str);
	return 0;
}

int main()
{

	stHttpServer stHttpServerInfo;
	memset(&stHttpServerInfo,0,sizeof(stHttpServerInfo));
	stHttpServerInfo.httpd_ListenPort	= 6666;
	stHttpServerInfo.httpd_timeout		= 20;
	memcpy(stHttpServerInfo.httpd_ListenIP,HTTP_LISTEN_IP,strlen(HTTP_LISTEN_IP));
	stHttpServerInfo.defaultDelFunc		= http_CommonCallfunc;
	stHttpServerInfo.faceInfoDelFuc		= http_FaceCallfunc;	
	stHttpServerInfo.keepAliveDelFunc	= http_KeepAliveCallfunc;	
	stHttpServerInfo.respondDelFunc		= http_RespondCodeCreate;
	memcpy(stHttpServerInfo.httpd_KeepAliveMatchStr,HTTP_CURL_HEARTDEL_PATH,strlen(HTTP_CURL_HEARTDEL_PATH));
	memcpy(stHttpServerInfo.httpd_FaceInfoMatchStr,HTTP_CURL_FACEDEL_PATH,strlen(HTTP_CURL_FACEDEL_PATH));
	stHttpServerInfo.httpd_RcvBufSize = HTTP_RCV_BUFFER_SIZE;

	/*开启http server 会自动阻塞然后进行循环处理*/
	httpServer_Create(&stHttpServerInfo);

	return 0;
}







