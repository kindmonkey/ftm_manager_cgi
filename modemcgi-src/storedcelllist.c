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

/*
int open_port(char *port)
{
		struct termios options;
		int fd;
		fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1)
		{
				printf("open_port: Unable to open the port - ");
		}
		else
		{
				printf ( "Port %s with file descriptor=%i",port, fd);

				fcntl(fd, F_SETFL, FNDELAY);
				//tcgetattr( fd, &options );
				//cfsetispeed( &options, BAUDRATE );
				//cfsetospeed( &options, BAUDRATE );
				//options.c_cflag |= ( CLOCAL | CREAD);
				//options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CSIZE);
				//options.c_cflag |= CS8;
				//options.c_cflag &= ~CRTSCTS;
				//options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
				//options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR);
				//options.c_oflag &= ~OPOST;
				

				// get the current options
				tcgetattr(fd, &options);
				// set raw input, 1 second timeout
				options.c_cflag |= (CLOCAL | CREAD);
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
				options.c_oflag &= ~OPOST;
				options.c_cc[VMIN] = 0;
				options.c_cc[VTIME] = 10;
				// set the options
				//tcsetattr(fd, TCSANOW, &options);

				if ( tcsetattr( fd, TCSANOW, &options ) == -1 )
						printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
				else		
						printf ( "%s\n", "succeed" );
						
		}
		return (fd);
}
*/

int IOT_StoredCellList(qentry_t *pReq) 
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
		if (strcmp(value, "set") == 0) {

				char command[100] = "AT#EMCC\r";

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
