#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xml.h>
#include "qdecoder.h"
 
int FTMC_SENSOR(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "getlist") == 0)
	{
		char *lpszMac		= pReq->getstr(pReq, "mac", false);

				
		char command[1024];
		sprintf(command, "/www/cgi-bin/sensor_script/get_humidity_info.sh %s | awk -f /www/cgi-bin/sensor_script/get_humidity_info.awk", lpszMac);

		char	szBuf[1024];
		FILE *pPF = popen(command, "r");
		
		if (pPF != NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR_LIST>\n");
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szMac[64];
				char	szID[64];
				char	szType[64];
				char	szName[64];
				char	szSN[64];
				char	szState[64];
				char	szValue[64];
				char	szLastValue[64];
				char	szLastTime[64];
				char	szInterval[64];

				if (10 == sscanf(szBuf, "%s %s %s %s %s %s %s %s %s %s", szMac, szID, szType, szName, szSN, szState, szValue, szLastValue, szLastTime, szInterval))
				{
					printf("<SENSOR>\n");
					printf("<MAC>%s</MAC>", szMac);
					printf("<ID>%s</ID>\n", szID);	
					printf("<TYPE>%s</TYPE>\n", szType);	
					printf("<NAME>%s</NAME>\n", szName);	
					printf("<SN>%s</SN>\n", szSN);
					printf("<STATE>%s</STATE>\n", szState);
					printf("<VALUE>%s</VALUE>\n", szValue);
					printf("<LASTVALUE>%s</LASTVALUE>\n", szLastValue);
					printf("<LASTTIME>%s</LASTTIME>\n", szLastTime);
					printf("<INTERVAL>%s</INTERVAL>\n", szInterval);
					printf("</SENSOR>\n");
				}
			}
			printf("</SENSOR_LIST>\n");
		}
		pclose(pPF);

	}
	else if(strcmp(lpszCmd, "set") == 0)
	{
		char *lpszMac		= pReq->getstr(pReq, "mac", false);
		char *lpszID		= pReq->getstr(pReq, "id", false);

		if ((lpszMac == NULL) || 
			(lpszID == NULL)) 
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR_ADD>\n");	
			printf("<RET>ERROR</RET>\n");
			printf("<MSG>Invalid Parameter!</MSG>\n");
			printf("</SENSOR_ADD>\n");	
		}
		
		char command[1024];
		sprintf(command, "/www/cgi-bin/sensor_script/get_humidity.sh %s | awk '/%s/ && /%s/' | xargs -t /www/cgi-bin/sensor_script/pull_sensor_list.sh", lpszMac, lpszMac, lpszID);

		FILE *fp = popen(command, "r");
		pclose(fp);

		char resCommand[1024];
		sprintf(resCommand, "/www/cgi-bin/sensor_script/get_added_sensor.sh %s %s", lpszMac, lpszID);
	
		char	szBuf[64];
		fp = popen(resCommand, "r");
		if (fp != NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR_ADDED>\n");
			while(fgets(szBuf, sizeof(szBuf), fp) != 0)
			{
				char	szID[64];

				if (1 == sscanf(szBuf, "%s", szID))
				{
					if (szID != NULL)
					{
						printf("<RET>OK</RET>\n");
					} else 
					{
						printf("<RET>ERROR</RET>\n");
					}
				}
			}
			printf("</SENSOR_ADDED>\n");
		}
		pclose(fp);
	}
	else
	{
		return	-1;	
	}

	return	0;
}
