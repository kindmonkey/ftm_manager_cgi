#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"

int FTMC_SSL(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "status") == 0)
	{
		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<SSL>\n");

		char	szBuf[1024];
		FILE *pPF = popen("/www/cgi-bin/status_ssl.sh", "r");

		if (pPF != NULL)
		{
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szTag[64];	
				char	szData[64];
				if (2 == sscanf(szBuf, "%s %s", szTag, szData)) {

				if (strcmp(szTag, "USER") == 0)
				{
					printf("<USER>%s</USER>\n", szData);
				}
				else if (strcmp(szTag, "PASSWD") == 0)
				{
					printf("<PASSWD>%s</PASSWD>\n", szData);
				}
				else if (strcmp(szTag, "SERVER_IP") == 0)
				{
					printf("<SERVER_IP>%s</SERVER_IP>\n", szData);
				}
				else if (strcmp(szTag, "SERVER_PORT") == 0)
				{
					printf("<SERVER_PORT>%s</SERVER_PORT>\n", szData);
				}
				else if (strcmp(szTag, "INTERVAL") == 0)
				{
					printf("<INTERVAL>%s</INTERVAL>\n", szData);
				}
				}
			}

			pclose(pPF);
		}

		printf("</SSL>\n");
	}
	else if(strcmp(lpszCmd, "set") == 0)
	{
		char *lpszUser		= pReq->getstr(pReq, "user", false);
		char *lpszPasswd	= pReq->getstr(pReq, "passwd", false);
		char *lpszIP		= pReq->getstr(pReq, "ip", false);
		char *lpszPort		= pReq->getstr(pReq, "port", false);
		char *lpszInterval	= pReq->getstr(pReq, "interval", false);

		if ((lpszUser == NULL) || 
			(lpszPasswd == NULL) || 
			(lpszIP == NULL) || 
			(lpszPort == NULL) || 
			(lpszInterval == NULL))
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SSL_CONFIG>\n");	
			printf("<RET>ERROR</RET>\n");
			printf("<MSG>Invalid Parameter!</MSG>\n");
			printf("</SSL_CONFIG>\n");	
		}

		FILE *fp = fopen("/tmp/system.conf", "w");
		if (fp == NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SSL_CONFIG>\n");	
			printf("<RET>SYSTEM_ERROR</RET>\n");
			printf("<MSG>Can't create file\n</MSG>\n");
			printf("</SSL_CONFIG>\n");	
			return	-1;	
		}

		fprintf(fp, "[SSL]\n");
		fprintf(fp, "USER=%s\n", lpszUser);
		fprintf(fp, "PASSWD=%s\n", lpszPasswd);
		fprintf(fp, "SERVER_IP=%s\n", lpszIP);
		fprintf(fp, "SERVER_PORT=%s\n", lpszPort);
		fprintf(fp, "\n");

		fprintf(fp, "[KEEP_ALIVE]\n");
		fprintf(fp, "INTERVAL=%s\n", lpszInterval);	

		fclose(fp);

		fp = popen("cp -f /tmp/system.conf /etc/ssl_config/system.conf;sync;sync", "r");
		pclose(fp);

		fp = popen("/etc/init.d/ssl restart", "r");
		pclose(fp);

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<SSL_CONFIG>\n");
		printf("<RET>OK</RET>\n");
		printf("</SSL_CONFIG>\n");
	}
	else
	{
		return	-1;	
	}

	return	0;
}
