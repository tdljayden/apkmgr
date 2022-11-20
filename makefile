LIBS=-lcurl

apkmgr: apkmgr.c
	cc ${LIBS} apkmgr.c -o apkmgr
