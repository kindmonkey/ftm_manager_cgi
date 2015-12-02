#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"

int IOT_QOS(qentry_t *pReq) 
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
				//fp = popen("echo AT+CGEQNEG=2 > /dev/ttyS1; sleep 0.1", "r");
				//fp = popen("echo AT+CGEQREQ? > /dev/ttyS1; sleep 0.1", "r");
				//if (fp != NULL)
				//{
				//		pclose(fp);
				//}
				fp = popen("/www/cgi-bin/scripts/qos.sh", "r");
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
				char *traffic = req->getstrf(req, false, "traffic");
				char *mbUplink = req->getstrf(req, false, "mb_uplink");
				char *mbDnlink = req->getstrf(req, false, "mb_dnlink");
				/*
				char *gbUplink = req->getstrf(req, false, "gb_uplink");
				char *gbDnlink = req->getstrf(req, false, "gb_dnlink");
				char *deliveryOrder = req->getstrf(req, false, "delivery_order");
				char *maxSDUsize = req->getstrf(req, false, "max_sdu_size");
				char *sduErrorRatio = req->getstrf(req, false, "sdu_error_ratio");
				char *residualBitErrorRatio = req->getstrf(req, false, "residual_bit_error_ratio");
				char *deliveryErrorSDUs = req->getstrf(req, false, "delivery_error_sdus");
				char *transferDelay = req->getstrf(req, false, "transfer_delay");
				char *trafficHandlingPriority = req->getstrf(req, false, "traffic_handling_priority");
				*/
				char command[100];
				sprintf(command, "echo 'at+cgeqreq=%s,%s,%s,%s' > /dev/ttyACM0; sleep 0.1", 
								cid, traffic, mbUplink, mbDnlink/*, gbUplink, gbDnlink, deliveryOrder,
								maxSDUsize, sduErrorRatio, residualBitErrorRatio, deliveryErrorSDUs, 
								transferDelay, trafficHandlingPriority*/);

				qcgires_setcontenttype(req, "text/xml");
				fp = popen(command, "r");
				while(fgets(resBuf, sizeof(resBuf), fp))
				{
						resBuf[strlen(resBuf)-1] = 0;
						
						printf("<data>\n");
						printf("<res>OK</res>\n");
						printf("<text>%s</text>\n", resBuf);
						printf("</data>");

				}
				pclose(fp);

				//fp = popen("killall pppd", "r");
				fp = popen("/www/cgi-bin/scripts/modem_detach.sh", "r");
				if (fp != NULL)
				{
						pclose(fp);
				}
				return 0;
			
		}
		return 0;
}
