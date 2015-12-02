#!/bin/bash
arm-openwrt-linux-gcc -std=gnu99 -Wall -fPIC -g -I. -DDEBUG -Iqdecoder-12.0.4/src/  -I./ -D_GNU_SOURCE -c -o upload.o upload.c
arm-openwrt-linux-gcc -std=gnu99 -Wall -fPIC -g -I. -DDEBUG -Iqdecoder-12.0.4/src/  -I./ -D_GNU_SOURCE -o upload.cgi upload.o qdecoder-12.0.4/src/libqdecoder.a
