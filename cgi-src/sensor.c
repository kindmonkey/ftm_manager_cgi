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
		//sprintf(command, "/www/cgi-bin/sensor_script/get_fte_es7.sh %s", lpszMac);
        sprintf(command, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/search_node.sh %s", lpszMac);

		char	szBuf[1024];
		FILE *pPF = popen(command, "r");

		if (pPF != NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR_LIST>\n");
			//printf("<TEST>%s</TEST>\n", command);
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szMac[64];
				char	szID[64];
				char	szType[64];
				char	szName[64];
				char	szSN[64];
				char	szState[64];
				char	szValue[64];
				//char	szLastValue[64];
				//char	szLastTime[64];
				//char	szInterval[64];

				if (7 == sscanf(szBuf, "%s %s %s %s %s %s %s", szMac, szID, szType, szName, szSN, szState, szValue))//, szLastValue, szLastTime, szInterval))
				{
                    // 저장된 센서가 있는지 확인. 
                    char resCommand[1024];
                    sprintf(resCommand, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/get_added_sensor.sh %s %s | awk -f /home/kindmong/work/ftm_manager/cgi-bin/sensor_script/get_added_sensor.awk", szMac, szID);

                    char	szBuf[64];
                    FILE *fp = popen(resCommand, "r");
                    if (fp != NULL)
                    {
                        while(fgets(szBuf, sizeof(szBuf), fp) != 0)
                        {
                            char	szRes[64];
                            sscanf(szBuf, "%s", szRes);
                            //printf("<TEST>%s</TEST>\n", szBuf);
                            //if (1 == sscanf(szBuf, "%s", szRes))
                            if (strcmp(szRes, "empty") == 0)
                            {
                                printf("<SENSOR_ADDED>\n");
                                printf("<SENSOR>\n");
                                printf("<MAC>%s</MAC>\n", szMac);
                                printf("<ID>%s</ID>\n", szID);	
                                printf("<TYPE>%s</TYPE>\n", szType);	
                                printf("<NAME>%s</NAME>\n", szName);	
                                printf("<SN>%s</SN>\n", szSN);
                                printf("<STATE>%s</STATE>\n", szState);
                                printf("<VALUE>%s</VALUE>\n", szValue);
                                printf("</SENSOR>\n");
                                printf("</SENSOR_ADDED>\n");
                            }
                        }
                    }
                    pclose(fp);
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
		
        // MAC, ID 로 등록할 센서 검색 후 DB에 저장.
		char command[1024];
		sprintf(command, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/get_sensor_info.sh %s | awk '/%s/ && /%s/' | xargs -t /home/kindmong/work/ftm_manager/cgi-bin/sensor_script/push_sensor_list.sh", lpszMac, lpszMac, lpszID);

		FILE *fp = popen(command, "r");
		pclose(fp);

        // 센서가 제대로 저장되었는지 확인. 
		char resCommand[1024];
		sprintf(resCommand, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/get_added_sensor.sh %s %s", lpszMac, lpszID);
	
		char	szBuf[64];
		fp = popen(resCommand, "r");
		if (fp != NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR_ADDED>\n");
            printf("<TEST>%s %s</TEST>\n", lpszMac, lpszID);
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
	} else if (strcmp(lpszCmd, "sensinglist") == 0) {
        
        char	szBuf[1024];
        FILE *pPF = popen("/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/get_sensor_list.sh | awk -F'|' '{ print $1, $2, $3, $4, $5 }'", "r");

        if (pPF != NULL)
        {
            qcgires_setcontenttype(pReq, "text/xml");
            printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
            printf("<SENSOR_LIST>\n");
            //printf("<TEST>%s</TEST>\n", command);
            while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
            {
                char	szMac[64];
                char	szID[64];
                char	szType[64];
                char	szName[64];
                char    szFavorite[64];
                //char	szSN[64];
                //char	szState[64];
                //char	szValue[64];
                //char	szLastValue[64];
                //char	szLastTime[64];
                //char	szInterval[64];

                if (5 == sscanf(szBuf, "%s %s %s %s %s", szMac, szID, szType, szName, szFavorite))
                {
                    printf("<SENSOR_ADDED>\n");
                    printf("<SENSOR>\n");
                    printf("<MAC>%s</MAC>\n", szMac);
                    printf("<ID>%s</ID>\n", szID);	
                    printf("<TYPE>%s</TYPE>\n", szType);	
                    printf("<NAME>%s</NAME>\n", szName);
                    printf("<FAVORITE>%s</FAVORITE>\n", szFavorite);
                    //printf("<SN>%s</SN>\n", szSN);
                    //printf("<STATE>%s</STATE>\n", szState);
                    //printf("<VALUE>%s</VALUE>\n", szValue);
                    printf("</SENSOR>\n");
                    printf("</SENSOR_ADDED>\n");
                }
            }
            printf("</SENSOR_LIST>\n");
        }
        pclose(pPF);

    } else if (strcmp(lpszCmd, "modifylist") == 0) {
        char *lpszMac		= pReq->getstr(pReq, "mac", false);
		char *lpszID		= pReq->getstr(pReq, "id", false);
       	char *lpszChecked	= pReq->getstr(pReq, "checked", false);

		if ((lpszMac == NULL) || 
			(lpszID == NULL) ||
            (lpszChecked == NULL)) 
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR_ADD>\n");	
			printf("<RET>ERROR</RET>\n");
			printf("<MSG>Invalid Parameter!</MSG>\n");
			printf("</SENSOR_ADD>\n");	
		}
        
        // MAC, ID 로 삭제할 센서 검색.
        char command[1024];
        sprintf(command, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/modify_sensor_list.sh %s %s %s", lpszMac, lpszID, lpszChecked);

        FILE *fp = popen(command, "r");
        qcgires_setcontenttype(pReq, "text/xml");
        printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
        printf("<SENSOR_MODIFY>\n");
        printf("<RET>OK</RET>\n");
        printf("</SENSOR_MODIFY>\n");
        pclose(fp);

    } else if (strcmp(lpszCmd, "delete") == 0) {
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
        
        // MAC, ID 로 삭제할 센서 검색.
        char command[1024];
        sprintf(command, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/delete_sensor_list.sh %s %s", lpszMac, lpszID);

        FILE *fp = popen(command, "r");
        qcgires_setcontenttype(pReq, "text/xml");
        printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
        printf("<SENSOR_ADDED>\n");
        printf("<RET>OK</RET>\n");
        printf("</SENSOR_ADDED>\n");
        pclose(fp);

    } else if (strcmp(lpszCmd, "dashboard") == 0) {

        char *lpszMac		= pReq->getstr(pReq, "mac", false);
        char *lpszID        = pReq->getstr(pReq, "id", false);
				
		char command[2048];
        sprintf(command, "/home/kindmong/work/ftm_manager/cgi-bin/sensor_script/get_sensing_list.sh %s %s | awk -F'|' '{ print $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12 }'", lpszMac, lpszID);

		char	szBuf[2048];
		FILE *pPF = popen(command, "r");

		if (pPF != NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<SENSOR>\n");
			//printf("<TEST>%s</TEST>\n", command);
            int countFlag = 0;
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
                char    szIndex[64];
				char	szMac[64];
				char	szID[64];
				char	szType[64];
				char	szName[64];
				char	szSN[128];
				char	szState[64];
				char	szValue[64];
				char	szLastValue[64];
				char	szLastTime[64];
                char	szInterval[64];
                char    szTime[64];
                char    szTime2[64];

                if (13 == sscanf(szBuf, "%s %s %s %s %s %s %s %s %s %s %s %s %s", szIndex, szMac, szID, szType, szName, szSN, szState, szValue, szLastValue, szLastTime, szInterval, szTime, szTime2))
                {
                    //printf("<INDEX>\n");
                    if (countFlag == 0) {
                        //printf("<NUMBER>%s</NUMBER>\n", szIndex);
                        printf("<MAC>%s</MAC>\n", szMac);
                        printf("<ID>%s</ID>\n", szID);	
                        printf("<TYPE>%s</TYPE>\n", szType);	
                        printf("<NAME>%s</NAME>\n", szName);	
                        printf("<SN>%s</SN>\n", szSN);
                        printf("<VALUES>\n");
                    }
                    countFlag = 1;
                    printf("<INDEX>\n");
                    printf("<NUMBER>%s</NUMBER>\n", szIndex);
                    printf("<STATE>%s</STATE>\n", szState);
                    printf("<VALUE>%s</VALUE>\n", szValue);
                    printf("<LASTVALUE>%s</LASTVALUE>\n", szLastValue);
                    printf("<LASTTIME>%s</LASTTIME>\n", szLastTime);
                    printf("<INTERVAL>%s</INTERVAL>\n", szInterval);
                    printf("<TIME>%s %s</TIME>\n", szTime, szTime2);
                    printf("</INDEX>\n");
                }
			}
            printf("</VALUES>\n");
			printf("</SENSOR>\n");
		}
		pclose(pPF);

	}

    else
	{
		return	-1;	
	}

	return	0;
}
