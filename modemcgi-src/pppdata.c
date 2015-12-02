#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"

int IOT_PPP_DATA(qentry_t *pReq) 
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "state") == 0)
		{
				FILE *fp;
				char buf[256] = {0, };
				fp = popen("/www/cgi-bin/getPPPData.sh", "r");
				while(fgets(buf, sizeof(buf), fp))
				{
						buf[strlen(buf)-1] = 0;
						
						qcgires_setcontenttype(req, "text/xml");
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", buf);
						printf("</data>");

				}
				pclose(fp);
		} 
		
		return 0;
}
