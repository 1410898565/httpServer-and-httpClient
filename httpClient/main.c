#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "httpClient.h"

#define HTTP_RCVBUF_SIZE_MAX			(3*1024*1024)

#define HTTP_SERVER_IP					"127.0.0.1"
#define HTTP_SERVER_PORT				6666
#define HTTP_SERVER_PATH				"/"


#define POSTDATA "{\"username\":\"gailun\",\"password\":\"123123\",\"driver\":\"yes\"}"


char gHttpRcvHeadBuffer[16*1024]={0};
char gHttpRcvBodyBuffer[HTTP_RCVBUF_SIZE_MAX]={0};
char gHttpSendBuffer[HTTP_RCVBUF_SIZE_MAX]={0};



int main()
{
	int nRet = -1;
	long lTmp = -1;
	stHttpClientInfo stHttpClient;

	memcpy(stHttpClient.chClientIP,HTTP_SERVER_IP,strlen(HTTP_SERVER_IP));	
	memcpy(stHttpClient.chClientPath,HTTP_SERVER_PATH,strlen(HTTP_SERVER_PATH));
	stHttpClient.nClientPort = HTTP_SERVER_PORT;
	stHttpClient.stHttpUserHead.nOperation 	= HTTP_POST_ONE_SHOT_PIC_MEM;	
	stHttpClient.stHttpUserHead.ucFlag 		= HTTP_HEAD;
	stHttpClient.stHttpUserHead.nRcvBufLen 	= sizeof(gHttpRcvHeadBuffer);
	stHttpClient.stHttpUserHead.pRcvBuffer	= gHttpRcvHeadBuffer;
	stHttpClient.stHttpUserHead.pSendBuffer = NULL;
	stHttpClient.stHttpUserHead.nRcvSize	= 0;
	
	stHttpClient.stHttpUserBody.nOperation	= HTTP_POST_ONE_SHOT_PIC_MEM;
	stHttpClient.stHttpUserBody.ucFlag 		= HTTP_BODY;
	stHttpClient.stHttpUserBody.nRcvBufLen	= sizeof(gHttpRcvBodyBuffer);
	stHttpClient.stHttpUserBody.pRcvBuffer 	= gHttpRcvBodyBuffer;
	stHttpClient.stHttpUserBody.pSendBuffer = gHttpSendBuffer;
	stHttpClient.stHttpUserBody.nRcvSize	= 0;
	stHttpClient.stHttpUserBody.nSedSize	= strlen(POSTDATA);
	memcpy(stHttpClient.stHttpUserBody.pSendBuffer,POSTDATA,strlen(POSTDATA));
	sprintf(stHttpClient.chUrl,"%s:%d/%s",stHttpClient.chClientIP,stHttpClient.nClientPort,stHttpClient.chClientPath);

	printf("->%s\n",stHttpClient.chUrl);
	
	/*ÉÏ´«Í¼Æ¬*/
	nRet = httpClient_httpPost(&stHttpClient,&lTmp);
	if(nRet < 0)
	{
		printf("[picUpload] ====> %s func http_Post failed,at (%d) lines\n",__FUNCTION__,__LINE__);
		return -1;
	}
	printf("\nRcv Head:\n%s\n",stHttpClient.stHttpUserHead.pRcvBuffer);
	printf("\nRcv Body:\n%s\n",stHttpClient.stHttpUserBody.pRcvBuffer);

	return 0;
}







