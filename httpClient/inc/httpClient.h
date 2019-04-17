/*****************************************************************
 ** 文件名 : http.h
 ** 2015-2017 高新兴科技集团股份有限公司 Co.Ltd
 
 ** 创建人: 陈贤禧
 ** 日  期: 2016-11-xx
 ** 描  述: http应用程序头文件
 ** 版  本: V1.0

 ** 修改人:
 ** 日  期:
 ** 修改描述:
 ** 版  本: 
******************************************************************/

#ifndef __HTTPCLIENT_H
#define __HTTPCLIENT_H	 

#ifdef __cplusplus 
extern "C" { 
#endif

/*****************************************************************
* 包含头文件(如非特殊需要，H文件不建议包含其它H文件)
******************************************************************/


/*****************************************************************
* 宏定义
******************************************************************/
#define HTTP_STRING							"http://"
#define HTTP_POST_ONE_SHOT_PIC_MEM			0

#define HTTP_BODY 						0  //http内容
#define HTTP_HEAD						1  //http头


/*****************************************************************
* 结构、枚举定义
******************************************************************/
typedef struct _stHttpUserInfo
{
	int nOperation;				//操作长度
	int ucFlag;					//用以区分头部还是内容部分
	int nSedSize;				//发送数据大小	
	int nRcvBufLen;				//缓冲区长度
	int nRcvSize;				//接收body数据大小
	
	char *pRcvBuffer;			//指向用户用来存储数据的buf
	char *pSendBuffer;			//指向用户发送数据的buf
}stHttpUserInfo;



typedef struct _stHttpClientInfo
{
	int nClientPort;			//http服务器监听端口号
	char chClientIP[32];		//http服务器IP
	char chClientPath[64];		//http服务器文件路径
	char chUserName[32];		//http服务器登录用户
	char chUserPassWord[32];	//http服务器登录密码
	char chUrl[128];			//http服务器url

	stHttpUserInfo stHttpUserHead;
	stHttpUserInfo stHttpUserBody;
}stHttpClientInfo;


/*****************************************************************
* 全局变量声明
******************************************************************/

/*****************************************************************
* 函数原型声明
******************************************************************/
int httpClient_httpPost(stHttpClientInfo *pstUserArg,long *pRespCode);
int httpClient_httpGet(stHttpClientInfo *pstUserArg,long *pRespCode);


#ifdef __cplusplus
}
#endif

#endif

