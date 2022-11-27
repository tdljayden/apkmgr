LIBS = -lcurl -lpcre2-8
CFLAGS = -Wall

apkmgr: apkmgr.c
	cc ${CFLAGS} ${LIBS} apkmgr.c -o apkmgr
