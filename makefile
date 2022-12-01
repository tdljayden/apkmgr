LIBS = -lcurl -lcjson
CFLAGS = -Wall

apkmgr: apkmgr.c apkmgr.h
	cc ${CFLAGS} ${LIBS} apkmgr.c -o apkmgr
