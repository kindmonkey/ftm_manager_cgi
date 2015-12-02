#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "qdecoder.h"

int FTMC_WIFI(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "status") == 0)
	{
		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<wifi>\n");

		char	szBuf[1024];
		FILE *pPF = popen("/www/cgi-bin/status_hostapd.sh", "r");
		if (pPF != NULL)
		{
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szTag[64];	
				char	szData0[64];
				//char	szData1[64];
				
				switch(sscanf(szBuf, "%s %s", szTag, szData0))//, szData1))
				{
				case	2:
					{
						if (strcmp(szTag, "status") == 0)
						{
							printf("<status>%s</status>\n", szData0);
						}
						else if (strcmp(szTag, "essid") == 0)
						{
							printf("<essid>%s</essid>\n", szData0);					
						}
						else if (strcmp(szTag, "key") == 0)
						{
							printf("<key>%s</key>\n", szData0);					
						}
					}
					break;	
				}
			}
			pclose(pPF);
		}
		printf("</wifi>\n");
	}
	
	else if(strcmp(lpszCmd, "set") == 0)
	{
		/*
		char *lpszEnable	= pReq->getstr(pReq, "enable", false);
		char *lpszInterface = pReq->getstr(pReq, "if", false);
		char *lpszStatic	= pReq->getstr(pReq, "static", false);
		char *lpszStart		= pReq->getstr(pReq, "start", false);
		char *lpszEnd		= pReq->getstr(pReq, "end", false);
		char *lpszRouter	= pReq->getstr(pReq, "router", false);
		char *lpszTime		= pReq->getstr(pReq, "time", false);
		*/
		char *lpszSSID		= pReq->getstr(pReq, "wifi_ssid", false);
		char *lpszPASSWD	= pReq->getstr(pReq, "wpa_passphrase", false);
		char *lpszSTATUS	= pReq->getstr(pReq, "status", false);

		if ((lpszSSID == NULL) ||
			(lpszPASSWD == NULL) ||
			(lpszSTATUS == NULL))
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<WIFI_CONFIG>\n");	
			printf("<RET>ERROR</RET>\n");
			printf("<MSG>Invalid Parameter!</MSG>\n");
			printf("</WIFI_CONFIG>\n");	
		}

		FILE *fp = fopen("/tmp/config.sh", "w");
		if (fp == NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<WIFI_CONFIG>\n");	
			printf("<RET>SYSTEM_ERROR</RET>\n");
			printf("<MSG>Can't create file\n[/tmp/hostapd.conf]</MSG>\n");
			printf("</WIFI_CONFIG>\n");	
			return	-1;	
		}

		fprintf(fp, "iwconfig wlan1 essid %s\n", lpszSSID);
		fprintf(fp, "iwconfig wlan1 key s:%s\n", lpszPASSWD);
		fclose(fp);
		
		fp = popen("chmod 755 /tmp/config.sh", "r");
		pclose(fp);

		fp = popen("cp -f /tmp/config.sh /etc/wifi_config.sh;sync;sync", "r");
		pclose(fp);

		if (strcmp(lpszSTATUS, "true") == 0)
		{
			fp = popen("echo enabled > /etc/service/wifi", "r");
		} else {
			fp = popen("echo disabled > /etc/service/wifi", "r");
		}
		pclose(fp);

		char buf2[256] = {0, };
		fp = popen("/www/cgi-bin/restart_ap.sh", "r");
		while(fgets(buf2, sizeof(buf2), fp))
		{
				buf2[strlen(buf2)-1] = 0;
				if (strcmp(buf2, "done") == 0)
				{
						break;
				}
		}
		pclose(fp);

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<WIFI_CONFIG>\n");
		printf("<RET>OK</RET>\n");
		printf("</WIFI_CONFIG>\n");
	}
	else
	{
		return	-1;	
	}
	
	return	0;
}
