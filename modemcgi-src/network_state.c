#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"
//#define MODEM "/dev/ttyS1"
//#define BUFSIZE 255
//#define BAUDRATE B115200
//#define FALSE 0
//#define TRUE 1

//int open_port(char *port);

int IOT_NETWORK_STATE(qentry_t *pReq) 
{
		//int STOP=FALSE;		
		//int res;
		//int serialFD = open_port(MODEM);
		//char numbuf[BUFSIZE];
		//memset(buf,0,BUFSIZE);

		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "state") == 0)
		{
				FILE *fp;
				char buf[1024] = {0, };

				qcgires_setcontenttype(req, "text/xml");
				//fp = popen("echo AT#RFSTS > /dev/ttyS1; sleep 0.1", "r");
				//if (fp != NULL)
				//{
				//		pclose(fp);
				//}
				fp = popen("/www/cgi-bin/scripts/network_status.sh", "r");
				while(fgets(buf, sizeof(buf), fp))
				{
						buf[strlen(buf)-1] = 0;
						
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", buf);
						printf("</data>");

				}
				pclose(fp);
				return 0;
				/*
				char buf2[256] = {0, };	
				//fp = popen("cat /etc/pNum.log | awk '/CNUM:/' | awk 'END {print}'", "r");
				fp = popen ("mmd -p /dev/ttyS1 -c at+cnum | awk '/CNUM/'", "r");
				while(fgets(buf2, sizeof(buf2), fp))
				{
						buf2[strlen(buf2)-1] = 0;
						//printf("network_state : %s", buf);
						
						//qcgires_setcontenttype(req, "text/xml");
						//printf("<data>\n");
						//printf("<res>OK</res>\n");
						if (strcmp(buf2, "") != 0) {
							printf("<num>%s</num>\n", buf2);
						} else {
							printf("<num>none</num>\n");
						}
						printf("</data>");

				}
				pclose(fp);
				*/
				/*
				int STOP=FALSE;		
				int res;
				int serialFD = open_port(MODEM);
				char numbuf[BUFSIZE];

				write(serialFD, "AT+CNUM\r" , strlen("AT+CNUM\r"));
				while (STOP==FALSE) {

						res = read(serialFD, numbuf, BUFSIZE);
						numbuf[res]=0;
						//printf("%s:%d\n", buf, res);
						
						if (strstr(numbuf, "OK") != NULL) {
								STOP=TRUE;
								printf("<num>%s</num>\n", numbuf);
								printf("</data>");
						}
						else if (strstr(numbuf, "ERROR") != NULL) {
								STOP=TRUE;
								printf("<num>%s</num>\n", numbuf);
								printf("</data>\n");
						}
						
				}
				close(serialFD);
				*/

		} 
		
		//close(serialFD);
		return 0;
}
