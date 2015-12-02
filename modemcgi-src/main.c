/******************************************************************************
 * qDecoder - http://www.qopyright (c) 2000-2012 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 * $Id: query.c 636 2012-05-07 23:40:43Z seungyoung.kim $
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "qdecoder.h"
#include <sys/signal.h>
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

int wait_flag=TRUE;

typedef	struct _COMMAND
{
		char	*lpszName;
		int		(*fService)(qentry_t *req);
} IOT_COMMAND;

int IOT_ForcedCH(qentry_t *pReq);
int IOT_StoredCellList(qentry_t *pReq);
int IOT_SecurityMode(qentry_t *pReq);
int IOT_HSDPA(qentry_t *pReq);
int IOT_INIT(qentry_t *pReq);
int IOT_QOS(qentry_t *pReq);
int IOT_APN(qentry_t *pReq);
int IOT_NETWORK_STATE(qentry_t *pReq);
int IOT_MODULE(qentry_t *pReq);
int IOT_CONFIG(qentry_t *pReq);
int IOT_CNUM(qentry_t *pReq);
int IOT_USIM(qentry_t *pReq);
int IOT_PPP_DATA(qentry_t *pReq);
int IOT_PPP_IP(qentry_t *pReq);

IOT_COMMAND		cmds[] =
{
		{	"forcedch",			IOT_ForcedCH			},
		{	"storedcell",		IOT_StoredCellList		},
		{	"securitymode",		IOT_SecurityMode		},
		{	"hsdpa",			IOT_HSDPA				},
		{	"init",				IOT_INIT				},
		{	"qos",				IOT_QOS					},
		{	"apn",				IOT_APN					},
		{	"network_state",	IOT_NETWORK_STATE		},
		{	"module",			IOT_MODULE				},
		{	"config",			IOT_CONFIG				},
		{	"cnum",				IOT_CNUM				},
		{	"usim",				IOT_USIM				},
		{	"getdata",			IOT_PPP_DATA			},
		{	"pppip",			IOT_PPP_IP				},
		{	NULL,				NULL					}
};

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
				
				tcgetattr( fd, &options );
				cfsetispeed( &options, BAUDRATE );
				cfsetospeed( &options, BAUDRATE );
				options.c_cflag |= ( CLOCAL | CREAD);
				options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CSIZE);
				options.c_cflag |= CS8;
				options.c_cflag &= ~CRTSCTS;
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
				options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR);
				options.c_oflag &= ~OPOST;
				
				/*
				// get the current options
				tcgetattr(fd, &options);
				// set raw input, 1 second timeout
				options.c_cflag |= (CLOCAL | CREAD);
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
				options.c_oflag &= ~OPOST;
				options.c_cc[VMIN] = 0;
				options.c_cc[VTIME] = 10;
				*/

				/*
				if ( tcsetattr( fd, TCSANOW, &options ) == -1 )
						printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
				else        
						printf ( "%s\n", "succeed" );
						*/
		}
		return (fd);
}

int main(int argc, char *argv[])
{
		// Parse queries.
		qentry_t *pReq = qcgireq_parse(NULL, 0);

		IOT_COMMAND	*pCmd = &cmds[0];

		while(pCmd->lpszName != NULL)
		{
				if (strcmp(pCmd->lpszName, argv[0]) == 0)
				{
						break;
				}

				pCmd++;
		}

		if (pCmd->lpszName == NULL)
		{
			qcgires_error(pReq, "Not supported command!");
		} else {
			pCmd->fService(pReq);
		}
		// De-allocate memories
		pReq->free(pReq);
		return 0;
}
