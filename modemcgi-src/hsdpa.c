#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"
#define MODEM "/dev/ttyS1"
#define BUFSIZE 255
#define BAUDRATE B115200
#define FALSE 0
#define TRUE 1

int open_port(char *port);

int IOT_HSDPA(qentry_t *pReq) 
{
		int STOP=FALSE;		
		int res;
		int serialFD = open_port(MODEM);
		char buf[BUFSIZE];
		//memset(buf,0,BUFSIZE);

		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "state") == 0)
		{
				write(serialFD, "AT#HSDPA?\r" , strlen("AT#HSDPA?\r"));
				while (STOP==FALSE) {

						res = read(serialFD,buf,255);
						buf[res]=0;
						//printf("%s:%d\n", buf, res);
						if (strstr(buf, "OK") != NULL) {
								STOP=TRUE;
								qcgires_setcontenttype(req, "text/xml");
								printf("<data>\n");
								printf("<res>OK</res>\n");
								printf("<text>%s</text>\n", buf);
								printf("</data>");
						}
						else if (strstr(buf, "ERROR") != NULL) {
								STOP=TRUE;
								qcgires_setcontenttype(req, "text/xml");
								printf("<data>\n");
								printf("<res>ERROR</res>\n");
								printf("<text>%s</text>\n", buf);
								printf("</data>\n");
						}
						
				}

		} else if (strcmp(value, "set") == 0) {

				char *mode = req->getstrf(req, false, "mode");
				char *category = req->getstrf(req, false, "category");
				
				char command[100];
				sprintf(command, "AT#HSDPA=%s,%s\r", mode, category);
				write(serialFD, command, strlen(command));
				while (STOP==FALSE) {

						res = read(serialFD,buf,255);
						buf[res]=0;
						//printf("%s:%d\n", buf, res);
						if (strstr(buf, "OK") != NULL) {
								STOP=TRUE;
								qcgires_setcontenttype(req, "text/xml");
								printf("<data>\n");
								printf("<res>OK</res>\n");
								printf("<command>%s</command>\n", command);
								printf("<text>%s</text>\n", buf);
								printf("</data>");
						}
						else if (strstr(buf, "ERROR") != NULL) {
								STOP=TRUE;
								qcgires_setcontenttype(req, "text/xml");
								printf("<data>\n");
								printf("<command>%s</command>\n", command);
								printf("<res>ERROR</res>\n");
								printf("<text>%s</text>\n", buf);
								printf("</data>\n");
						}

				}

		}
		close(serialFD);
		return 0;
}
