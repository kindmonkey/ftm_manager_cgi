#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"

int open_port(char *port);

int IOT_INIT(qentry_t *pReq) 
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "init") == 0)
		{
				FILE *fp;

				fp = popen("rm -r /overlay/", "r");
				pclose(fp);
				qcgires_setcontenttype(req, "text/xml");
				printf("<data>\n");
				printf("<res>OK</res>\n");
				printf("</data>");
				return 0;
//				char buf[64] = {0, };

				/*				
				fp = popen("./init.sh", "r");
				if (fp == NULL)
				{
						perror("error");
				} else {
						pclose(fp);
				}
					
				qcgires_setcontenttype(req, "text/html");
				printf("Please try again in a few minutes.");
	*/
	/*			qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/init.sh", "r");
				while(fgets(buf, sizeof(buf), fp))
				{
						buf[strlen(buf)-1] = 0;
						
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("</data>");

				}
				pclose(fp);
				return 0;
	*/		
				/*
				while(fgets(buf, sizeof(buf), fp) != NULL)
				{
						printf("%s", buf);
				}
				*/

				/*
				fp = popen("sync;sync; reboot", "r");
				if (fp != NULL)
				{
					pclose(fp);
				}
				*/
				//req->free(req);

		} 
		return 0;
}
