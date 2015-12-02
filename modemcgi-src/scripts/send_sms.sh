#!/bin/sh
chat TIMEOUT 1 "" "AT+CMGS=$1" "OK" > /dev/ttyS1
chat TIMEOUT 1 "" "Success!!" "OK" > /dev/ttyS1
chat TIMEOUT 1 "" "^Z" "OK" > /dev/ttyS1
echo OK
