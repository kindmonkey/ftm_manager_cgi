#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"


int IOT_APN(qentry_t *pReq)
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
				//fp = popen("echo AT+CGDCONT? > /dev/ttyS1; sleep 0.1", "r");
				//if (fp != NULL)
				//{
				//		pclose(fp);
				//}
				fp = popen("/www/cgi-bin/scripts/apn.sh", "r");
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

				char *cid = req->getstrf(req, false, "cid");
				char *pdp_type = req->getstrf(req, false, "pdp_type");
				char *apn = req->getstrf(req, false, "apn");
				//char *pdp_addr = req->getstrf(req, false, "pdp_addr");
				//char *d_comp = req->getstrf(req, false, "d_comp");
				//char *h_comp = req->getstrf(req, false, "h_comp");
			
				char command[100];
				sprintf(command, "echo 'at+cgdcont=%s,\"%s\",\"%s\"' > /dev/ttyACM0; sleep 0.1", 
						cid, pdp_type, apn);

				fp = popen(command, "r");
				while(fgets(resBuf, sizeof(resBuf), fp))
				{
						resBuf[strlen(resBuf)-1] = 0;

						qcgires_setcontenttype(req, "text/xml");
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", resBuf);
						printf("</data>");

				}
				pclose(fp);

				//fp = popen("killall pppd", "r");
				fp = popen("/www/cgi-bin/scripts/modem_detach.sh", "r");
				while(fgets(resBuf, sizeof(resBuf), fp))
				{
						resBuf[strlen(resBuf)-1] = 0;
				}
				pclose(fp);
				/*
				if (fp != NULL)
				{
						pclose(fp);
				}
				*/

				return 0;
		}
		return 0;
}
