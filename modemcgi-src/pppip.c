#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"


int IOT_PPP_IP(qentry_t *pReq)
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
				fp = popen("/www/cgi-bin/scripts/modem_ip.sh", "r");
				while(fgets(resBuf, sizeof(resBuf), fp))
				{
						resBuf[strlen(resBuf)-1] = 0;
					
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", resBuf);
						printf("</data>");

				}
				pclose(fp);

		} 
		return 0;
}
