#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"


int FTMC_APIKEY(qentry_t *pReq)
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		FILE *fp;
		char resBuf[256];

		if (strcmp(value, "state") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/get_apikey.sh", "r");
				while(fgets(resBuf, sizeof(resBuf), fp))
				{
						resBuf[strlen(resBuf)-1] = 0;
					
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", resBuf);
						printf("</data>");

				}
				pclose(fp);
				return 0;

		} else if (strcmp(value, "set") == 0) {

				char *apikey = req->getstrf(req, false, "api");
			
				char command[100];
				sprintf(command, "APIKEY=%s /opt/thingplus.sh restart", apikey);
				
				qcgires_setcontenttype(req, "text/xml");
				fp = popen(command, "r");
				pclose(fp);

				qcgires_setcontenttype(req, "text/xml");
				printf("<data>\n");
				printf("<res>OK</res>\n");
				printf("</data>");

				return 0;

		} else if (strcmp(value, "status") == 0) {

				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/opt/thingplus.sh status", "r");
				while(fgets(resBuf, sizeof(resBuf), fp))
				{
						resBuf[strlen(resBuf)-1] = 0;
					
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", resBuf);
						printf("</data>");

				}
				pclose(fp);
				return 0;

		} else if (strcmp(value, "cloud") == 0) {

				char *status = req->getstrf(req, false, "status");
			
				char command[100];
				sprintf(command, "/opt/thingplus.sh %s", status);
				
				qcgires_setcontenttype(req, "text/xml");
				fp = popen(command, "r");
				pclose(fp);

				qcgires_setcontenttype(req, "text/xml");
				printf("<data>\n");
				printf("<res>OK</res>\n");
				printf("</data>");

				return 0;
		}

		return 0;
}
