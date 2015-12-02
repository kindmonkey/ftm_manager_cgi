#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"

int IOT_USIM(qentry_t *pReq) 
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		FILE *fp;
		char buf[64] = {0, };

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "state") == 0)
		{
				//FILE *fp;
				//char buf[256] = {0, };

				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/usim.sh", "r");
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
	
		if (strcmp(value, "state_status") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/is_usim_status.sh", "r");
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

		if (strcmp(value, "state_nwcause") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
			//	fp = popen("/www/cgi-bin/scripts/sgact.sh", "r");
				//fp = popen("cat /tmp/nwcause", "r");
				fp = popen("/www/cgi-bin/scripts/is_limited.sh", "r");
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

		// usim socket status
		if (strcmp(value, "usim_socket_status") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/usim_socket_status.sh", "r");
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

		// different usim status
		if (strcmp(value, "usim_different_status") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/usim_different_status.sh", "r");
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

		// not open usim status
		if (strcmp(value, "usim_open_status") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/usim_open_status.sh", "r");
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

		// sending stop usim status
		if (strcmp(value, "usim_sending_stop_status") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/usim_sending_stop_status.sh", "r");
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

		// shade process
		if (strcmp(value, "is_limited") == 0)
		{
				qcgires_setcontenttype(req, "text/xml");
				fp = popen("/www/cgi-bin/scripts/is_limited.sh", "r");
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
