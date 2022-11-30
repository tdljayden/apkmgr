LIBS = -lcurl -lcjson
CFLAGS = -Wall

apkmgr: apkmgr.c
	cc ${CFLAGS} ${LIBS} apkmgr.c -o apkmgr
