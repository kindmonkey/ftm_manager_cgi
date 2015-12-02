#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xml.h>
#include "qdecoder.h"

int FTMC_Network(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "status") == 0)
	{
		char	szBuf[1024];
		FILE *pPF = popen("/www/cgi-bin/status_network.sh", "r");
		if (pPF != NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<STATUS_NETWORK>\n");
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				int		nIndex;
				char	szType[64];
				char	szIFName[64];
				char	szIPAddr[64];
				char	szNetmask[64];
				char	szGateway[64];
				printf("<CMT>%s</CMT>\n", szBuf);
				if (1 == sscanf(szBuf, "%s", szType))
				{
					if (strcmp(szType, "eth") == 0)
					{
						if (6 == sscanf(szBuf, "%s %d %s %s %s %s", szType, &nIndex, szIFName, szIPAddr, szNetmask, szGateway))
						{
							printf("<ETH>\n");
							printf("<INDEX>%d</INDEX>\n", nIndex);	
							printf("<IFNAME>%s</IFNAME>\n", szIFName);	
							printf("<IPADDR>%s</IPADDR>\n", szIPAddr);	
							printf("<NETMASK>%s</NETMASK>\n", szNetmask);	
							printf("<MACADDR>%s</MACADDR>\n", szGateway);	
							printf("</ETH>\n");
						}
					}
					else if (strcmp(szType, "p2p") == 0)
					{
						if (6 == sscanf(szBuf, "%s %d %s %s %s %s", szType, &nIndex, szIFName, szIPAddr, szNetmask, szGateway))
						{
							printf("<PTP>\n");
							printf("<INDEX>%d</INDEX>\n", nIndex);	
							printf("<IFNAME>%s</IFNAME>\n", szIFName);	
							printf("<IPADDR>%s</IPADDR>\n", szIPAddr);	
							printf("<NETMASK>%s</NETMASK>\n", szNetmask);	
							printf("<PEER>%s</PEER>\n", szGateway);	
							printf("</PTP>\n");
						}
					}
					else if (strcmp(szType, "dns") == 0)
					{
						if (4 == sscanf(szBuf, "%s %d %s %s", szType, &nIndex, szIFName, szIPAddr))
						{
							printf("<DNS>\n");
							printf("<INDEX>%d</INDEX>\n", nIndex);	
							printf("<NAME>%s</NAME>\n", szIFName);	
							printf("<IPADDR>%s</IPADDR>\n", szIPAddr);	
							printf("</DNS>\n");
						}
					}
				}
			}
			printf("</STATUS_NETWORK>\n");
			pclose(pPF);
		}
	}
	else if (strcmp(lpszCmd, "set") == 0)
	{

		FILE *fp = fopen("/tmp/network", "w");
		if (fp == NULL)
		{
			printf("<RET>ERROR</RET>\n");
			return	-1;
		}

//		fprintf(fp, "auto lo\n");	
		fprintf(fp, "config interface loopback\n");
		fprintf(fp, "\toption ifname\tlo\n");
		fprintf(fp, "\toption proto\tstatic\n");
		fprintf(fp, "\toption ipaddr\t127.0.0.1\n");
		fprintf(fp, "\toption netmask\t255.0.0.0\n");
		fprintf(fp, "\n");

		fprintf(fp, "config interface wan\n");
		fprintf(fp, "\toption ifname\teth0\n");
		fprintf(fp, "\toption proto\tdhcp\n");
		fprintf(fp, "\n");

		fprintf(fp, "config interface wwan\n");
		fprintf(fp, "\toption ifname\tusb0\n");
		fprintf(fp, "\toption proto\tdhcp\n");
		fprintf(fp, "\n");


		int i  = 0;
		while(1)
		{
			char *lpszIFName = pReq->getstrf(pReq, false, "ifname%d", i);

			if (lpszIFName == NULL)
			{
				break;
			}

			char *lpszIPAddr = pReq->getstrf(pReq, false, "ipaddr%d", i);
			char *lpszNetmask = pReq->getstrf(pReq, false, "netmask%d", i);

			if (strcmp(lpszIFName, "br-lan") == 0)
			{
					fprintf(fp, "config interface lan\n");
					fprintf(fp, "\toption ifname\teth2\n");
					fprintf(fp, "\toption type\tbridge\n");
					fprintf(fp, "\toption proto\tstatic\n");
					fprintf(fp, "\toption ipaddr\t%s\n", lpszIPAddr);
					fprintf(fp, "\toption netmask\t%s\n", lpszNetmask);
					fprintf(fp, "\n");
			}
			/*
			if (strcmp(lpszIFName, "eth0") == 0)
			{
					fprintf(fp, "config interface wan\n");
					fprintf(fp, "\toption ifname\teth0\n");
					fprintf(fp, "\toption proto\tdhcp\n");
					fprintf(fp, "\n");
			}
			if (strcmp(lpszIFName, "usb0") == 0)
			{
					fprintf(fp, "config interface wwan\n");
					fprintf(fp, "\toption ifname\tusb0\n");
					fprintf(fp, "\toption proto\tdhcp\n");
					fprintf(fp, "\n");
			}
			*/
			i++;
		}
		fclose(fp);
		
		/*
		char *lpszDNS0 = pReq->getstr(pReq, "dns0", false);
		char *lpszDNS1 = pReq->getstr(pReq, "dns1", false);
		fp = fopen("/tmp/resolv.conf", "w");
		if (lpszDNS0 != NULL)
		{
			fprintf(fp, "nameserver %s\n", lpszDNS0);
		}

		if (lpszDNS1 != NULL)
		{
			fprintf(fp, "nameserver %s\n", lpszDNS1);
		}
		fclose(fp);
		*/
		fp = popen("cp -f /tmp/network /etc/config/network;sync;sync", "r");
		pclose(fp);

		/*
		fp = popen("cp -f /tmp/resolv.conf /etc/resolv.conf;sync;sync", "r");
		pclose(fp);
		*/

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<CONFIG_NETWORK>\n");
		printf("<RET>OK</RET>\n");
		printf("</CONFIG_NETWORK>\n");		
	}	
	else
	{
		return	-1;	
	}

	return	0;
}
