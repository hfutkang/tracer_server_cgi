#!/bin/sh
killall tracercmd.cgi
spawn-fcgi -f ./tracercmd.cgi -p 9000 -a 127.0.0.1 -F 5
