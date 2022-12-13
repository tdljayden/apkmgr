#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include <cjson/cJSON.h>

#include "apkmgr.h"

void helpscreen(int apkmgrver);

int main(int argc, char **argv) {
	int apkmgrver = 0;
	if(argc==1) {
		helpscreen(apkmgrver);
	} else if(argc>=2) {
		if(strncmp(argv[1], "sync\n", 4) == 0) {
			if(argc==3) {
				syncapk(argv[2], grabapkver(argv[2]));
			} else {
				printf("No package specified!\n");
			}
		} else if(strncmp(argv[1], "upgrade", 7) == 0) { 
			printf("Upgrading all packages.\n");
		} else if(strncmp(argv[1], "remove", 6) == 0) { 
			printf("Removing package.\n");
		}
	}
	return 0;
}

void helpscreen(int apkmgrver) {
	printf("\napkmgr version %d\n\nusage: apkmgr <operation> [...]\noperations:\n	apkmgr {-h --help}\n	apkmgr {-V --version}\n	apkmgr {-I --syncindex}\n	apkmgr {-Q --query}        [packages(s)]\n	apkmgr {-R --remove}       [options] <packages(s)>\n	apkmgr {-S --sync}         [options] [package(s)]\n	apkmgr {-ver --apkversion} [packageversion]\n	apkmgr {-U --upgrade}\n\n", apkmgrver);
}
