#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "qdecoder.h"

#define BASEPATH    "/tmp"

ssize_t savefile(const char *filepath, const void *buf, size_t size)
{

		int fd = open(filepath,
						O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
		if (fd < 0) return -1;

		ssize_t count = write(fd, buf, size);
		close(fd);

		return count;
}

int main(void)
{
		// Parse queries.
		qentry_t *req = qcgireq_parse(NULL, 0);

		// get queries
		//const char *text = req->getstr(req, "text", false);
		const char *filedata   = req->getstr(req, "binary", false);
		int filelength = req->getint(req, "binary.length");
		const char *filename   = req->getstr(req, "binary.filename", false);
		//const char *contenttype = req->getstr(req, "binary.contenttype", false);

		// check queries
		//if (text == NULL) qcgires_error(req, "Invalid usages.");
		if (filename == NULL || filelength == 0) {
				qcgires_error(req, "Select file, please.");
		}

		char  filepath[1024];
		sprintf(filepath, "%s/%s", BASEPATH, filename);

		if (savefile(filepath, filedata, filelength) < 0) {
				qcgires_error(req, "File(%s) open fail. Please make sure CGI or directory has right permission.",
								filepath);
		}
		
		// result out
//		qcgires_setcontenttype(req, "text/html");
//		printf("Please try again in a few minutes.");
		//printf("<br><a src='192.168.0.110'>HOME</a>");
		//printf("You entered: <b>%s</b>\n", text);
		//printf("<br><a href=\"%s\">%s</a> (%d bytes, %s) saved.",
		//				filepath, filename, filelength, contenttype);

		// dump
		//printf("\n<p><hr>--[ DUMP INTERNAL DATA STRUCTURE ]--\n<pre>");
		//req->print(req, stdout, false);
		//printf("\n</pre>\n");
		

		FILE *fp;
		//int state;

//		char buff[256];
		fp = popen("./config_unzip.sh", "r");
		if (fp == NULL)
		{
				perror("erro : ");
		}

		/*
		while(fgets(buff, 256, fp) != NULL)
		{
				printf("%s", buff);
		}
		*/

		//state = 
		pclose(fp);
		//printf("state is %d\n", state);
/*
		fp = popen("sync;sync; reboot", "r");
		if (fp != NULL)
		{
			pclose(fp);
		}

		// de-allocate
		req->free(req);
*/		
		//==========================================
		fp = popen("./reboot.sh", "r");
		if (fp == NULL)
		{
				perror("error");
		} else {
				pclose(fp);
		}
		qcgires_setcontenttype(req, "text/html");
		printf("Please try again in a few minutes.");
		req->free(req);
		//==========================================

		return 0;
}
