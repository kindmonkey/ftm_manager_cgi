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


int IOT_MODULE(qentry_t *pReq)
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		FILE *fp;
		char buf[256] = {0, };

		if (strcmp(value, "state") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
//				fp = popen("echo AT*VERINFO > /dev/ttyACM0; sleep 0.1", "r");
//				if (fp != NULL)
//				{
//						pclose(fp);
//				}
				fp = popen("/www/cgi-bin/scripts/modem_ver.sh", "r");
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
		}
		
		if (strcmp(value, "hw_state") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
//				fp = popen("echo AT*VERINFO > /dev/ttyACM0; sleep 0.1", "r");
//				if (fp != NULL)
//				{
//						pclose(fp);
//				}
				fp = popen("/www/cgi-bin/scripts/modem_hw_ver.sh", "r");
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
		}

		return 0;
}
