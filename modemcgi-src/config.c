#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include "qdecoder.h"
#define buffsize 256

int IOT_CONFIG(qentry_t *pReq) 
{
		FILE *fp;
		int state;
		char buff[buffsize];

		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// Get values
		char *value = (char *)req->getstr(req, "cmd", false);
		if (strcmp(value, "backup") == 0)
		{
				fp = popen("./config_archive.sh", "r");
				if (fp == NULL)
				{
						perror("erro : ");
				}

				while(fgets(buff, 256, fp) != NULL)
				{
						printf("%s", buff);
				}

				/*
				fp = popen("./ssl.sh", "r");
				if (fp == NULL)
				{
						perror("erro : ");
				}

				while(fgets(buff, 256, fp) != NULL)
				{
						printf("%s", buff);
				}
				*/

				state = pclose(fp);
				printf("state is %d\n", state);
				qcgires_download(req, "/tmp/config.tar.gz", "text/plain");

				fp = popen("rm /tmp/config.tar.gz", "r");
				pclose(fp);
		}
		
		return 0;
}
