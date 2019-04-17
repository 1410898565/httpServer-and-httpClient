/*****************************************************************
 ** 文件名 ： http.c
 ** 2015-2017 高新兴科技集团股份有限公司 Co.Ltd
 
 ** 创建人: 骆军城
 ** 日  期: 2019-04-10
 ** 描  述: http客户端
 ** 版  本: V1.0

 ** 修改人:
 ** 日  期:
 ** 修改描述:
 ** 版  本: 
******************************************************************/
#ifdef __cplusplus
	extern "C" {
#endif
	

/*****************************************************************
* 包含头文件
******************************************************************/
#include <string.h>
#include "curl.h"
#include "httpClient.h"

/*****************************************************************
* 宏定义(仅在当前C文件使用的宏定义写在当前C文件中，否则需写在H文件中)
******************************************************************/

/*****************************************************************
* 结构定义(仅在当前C文件使用的结构体写在当前C文件中，否则需写在H文件中)
******************************************************************/

/*****************************************************************
* 全局变量定义
******************************************************************/


/*****************************************************************
* 静态变量定义
******************************************************************/

/*****************************************************************
* 外部变量声明（如果全局变量没有在其它的H文件声明，引用时需在此处声明，
*如果已在其它H文件声明，则只需包含此H文件即可）
******************************************************************/
																						
/*****************************************************************
* 函数原型声明
******************************************************************/

/*****************************************************************
* 函数定义
*注意，编写函数需首先定义所有的局部变量等，不允许在
*函数的中间出现新的变量的定义。
******************************************************************/

/*************************************************
** Function：httpClient_writeBack
** Description：http回调数据接收函数
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
size_t httpClient_writeBack(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int writeLength = 0;
	int written  = size*nmemb;
	stHttpUserInfo *pSt = (stHttpUserInfo*)stream;	
	
	if(1 == pSt->ucFlag) //此处记录头部
	{
		if(pSt->pRcvBuffer != NULL && (written <= (pSt->nRcvBufLen - pSt->nRcvSize)))
		{
			writeLength = (written <= (pSt->nRcvBufLen - pSt->nRcvSize))?(written):(pSt->nRcvBufLen - pSt->nRcvSize);
			memcpy((char*)(pSt->pRcvBuffer) +pSt->nRcvSize, ptr, writeLength);
			pSt->nRcvSize += writeLength;
			//printf("%d line,nWRLength=%d written=%d nUsedLength=%d writeLength=%d\n",__LINE__,pSt->nRcvBufLen, written,pSt->nRcvSize,writeLength);
		}
		return writeLength;
	}
	else if(0 == pSt->ucFlag)//此处记录body内容
	{
		if(pSt->pRcvBuffer != NULL && (written <= (pSt->nRcvBufLen - pSt->nRcvSize)))
		{
			writeLength = (written <= (pSt->nRcvBufLen - pSt->nRcvSize))?(written):(pSt->nRcvBufLen - pSt->nRcvSize);
			memcpy((char*)(pSt->pRcvBuffer) +pSt->nRcvSize, ptr, writeLength);
			pSt->nRcvSize += writeLength;
			//printf("%d line,nWRLength=%d written=%d nUsedLength=%d writeLength=%d\n",__LINE__,pSt->nRcvBufLen, written,pSt->nRcvSize,writeLength);
		}
		return writeLength;
	}
	return 0;
}


/*************************************************
** Function：httpClient_httpPost
** Description：httpPost
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
int httpClient_httpPost(stHttpClientInfo *pstUserArg,long *pRespCode)
{
	int nRet = 0;
	CURL *curl = NULL;
	CURLcode code = 0;
	long response_code = -1;
	struct curl_slist *chunk = NULL;

	*pRespCode = 0;

	if(strlen(pstUserArg->chUrl) < 14){
		printf("%s func Invalid parameter,chUrl=%s at (%d) line\n",__FUNCTION__,pstUserArg->chUrl,__LINE__);
		return -1;
	}

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, pstUserArg->chUrl);
		switch(pstUserArg->stHttpUserHead.nOperation)
		{
			case HTTP_POST_ONE_SHOT_PIC_MEM:
				//chunk = curl_slist_append(chunk, "Content-Type: image/jpeg;");
				chunk = curl_slist_append(chunk, "Content-Type: application/json;");
				break;
			break;
			default:
				break;
		}
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);		//分开接收head和body时，需要设置这个为0L
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);	//注意，毫秒超时一定要设置这个  
		curl_easy_setopt(curl, CURLOPT_POST, 1L); 

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpClient_writeBack);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pstUserArg->stHttpUserBody); 	/*接收body部分*/

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,httpClient_writeBack);
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &pstUserArg->stHttpUserHead);	/*接收头部分*/
		
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10*1000);	//设置延时30s
		switch(pstUserArg->stHttpUserHead.nOperation)
		{
			case HTTP_POST_ONE_SHOT_PIC_MEM:  //一次性发
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pstUserArg->stHttpUserBody.pSendBuffer);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pstUserArg->stHttpUserBody.nSedSize);
				break;
			default:
				break;
		}
		code = curl_easy_perform(curl);
		if(code != CURLE_OK)
		{
			nRet = -1;
			printf("%s func curl_easy_perform failed:%s at (%d) line\n",__FUNCTION__,curl_easy_strerror(code),__LINE__);
		} 
		else
		{
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			if(200 != response_code)
			{
				nRet = -1;
				printf("%s func curl_easy_getinfo RESPONSE_CODE: %d at (%d) line\n",__FUNCTION__,response_code,__LINE__);
			}
		}
		curl_slist_free_all(chunk);
		curl_easy_cleanup(curl);

		*pRespCode = response_code;

		return nRet;
	}

	return -1;
}

/*************************************************
** Function：httpClient_httpGet
** Description：httpget
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
int httpClient_httpGet(stHttpClientInfo *pstUserArg,long *pRespCode)
{
	int nRet		= 0;
	CURL *curl		= NULL;
	CURLcode code	= 0;
	long response_code = -1;

	*pRespCode = 0;

	if(strlen(pstUserArg->chUrl) < 14){
		printf("%s func Invalid parameter,chUrl=%s at (%d) line\n",__FUNCTION__,pstUserArg->chUrl,__LINE__);
		return -1;
	}
	
	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);							/*分开接收head和body时，需要设置这个为0L*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);						/*注意，毫秒超时一定要设置这个*/
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);		
		
		curl_easy_setopt(curl, CURLOPT_URL, pstUserArg->chUrl);
		curl_easy_setopt(curl, CURLOPT_USERNAME, pstUserArg->chUserName);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, pstUserArg->chUserPassWord);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpClient_writeBack);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pstUserArg->stHttpUserBody); 	/*接收body部分*/

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,httpClient_writeBack);
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &pstUserArg->stHttpUserHead);	/*接收头部分*/
		
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10*1000);			/*设置延时10s*/		
		code = curl_easy_perform(curl);
		if(code != CURLE_OK)
		{
			nRet = -1;
			printf("%s func curl_easy_perform failed:%s at (%d) line\n",__FUNCTION__,curl_easy_strerror(code),__LINE__);
		} 
		else
		{
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			if(200 != response_code)
			{
				nRet = -1;				
				printf("%s func curl_easy_getinfo RESPONSE_CODE: %d at (%d) line\n",__FUNCTION__,response_code,__LINE__);
			}
		}
		
		curl_easy_cleanup(curl);
		*pRespCode = response_code;

		return nRet;
	}
	return -1;
}



#ifdef __cplusplus
}
#endif

