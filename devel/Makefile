all: tracercmd.cgi

OBJS=tracercmd.o commands.o dbaccess.o http_parser.o requestBSposition.o
LIBS=-lfcgi -lmysqlclient
CFLAGS=-O2

tracercmd.cgi: $(OBJS)
	gcc -o tracercmd.cgi $(OBJS) $(LIBS)

.c.o:

.PHONY: clean

clean:
	rm $(OBJS) *~

