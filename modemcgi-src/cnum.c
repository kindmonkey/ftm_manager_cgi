#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"

int IOT_CNUM(qentry_t *pReq) 
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "state") == 0)
		{
				FILE *fp;
				char buf2[256] = {0, };	
				
				qcgires_setcontenttype(req, "text/xml");
				//fp = popen ("mmd -p /dev/ttyS1 -c at+cnum | awk '/CNUM/'", "r");
				fp = popen("echo AT+CNUM > /dev/ttyS1; sleep 0.1", "r");
				if (fp != NULL)
				{
					pclose(fp);
				}
				fp = popen("/www/cgi-bin/scripts/cnum.sh", "r");
				while(fgets(buf2, sizeof(buf2), fp))
				{
						buf2[strlen(buf2)-1] = 0;
						
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<num>%s</num>\n",buf2);
						printf("</data>\n");
						//pclose(fp);
						//return 0;
				}
				pclose(fp);
				return 0;
				//printf("<data>\n");
				//printf("<res>ERROR</res>\n");
				//printf("<num>none</num>\n");
				//printf("</data>\n");
				//pclose(fp);
		} 
		return 0;
}
