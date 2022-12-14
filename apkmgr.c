#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include <cjson/cJSON.h>

#include "apkmgr.h"

void helpscreen(int apkmgrver);

int main(int argc, char **argv) {
	/* int apkmgrver = 0;
	if(argc==1) {
		helpscreen(apkmgrver);
	} else if(argc>=2) {
		if(strncmp(argv[1], "sync", 4) == 0) {
			if(argc==3) {
				syncapk(argv[2], grabapkver(argv[2]));
			} else if (argc>4) {
				if (strncmp(argv[3], "-ver", 4) == 0) {
					long apkverint = (long)(argv[4]);
					syncapk(argv[2], apkverint);
				}
			} else if (argc==4) {
				printf("No package version specified!\n");
			} else {
				printf("No package specified!\n");
			}
		} else if(strncmp(argv[1], "upgrade", 7) == 0) { 
			printf("Upgrading all packages.\n");
		} else if(strncmp(argv[1], "remove", 6) == 0) { 
			printf("Removing package.\n");
		}
	} */
	// this is previous version code above
	int apkmgrver = 0;
	if (argc >= 2) {
		if (strncmp(argv[1], "sync", 4) == 0) {
			if (argc >= 3) {
				if (argc == 3) {
					printf("Installing latest version of package!\n");	
					syncapk(argv[2], grabapkver(argv[2]), false);
				} else if (strncmp(argv[3], "-ver", 4) == 0) {
					if (argc == 4) {
						printf("No package version specified!\n");
					} else if (argc == 5) {
						printf("Installing package with specified version!\n");
					} else {
						printf("Too many arguments to install!");
					}
				} else {
					int apkcount = (argc-2);
					while (apkcount != 0) {
						int apknamelocation = (argc - apkcount); 
						syncapk(argv[apknamelocation], grabapkver(argv[apknamelocation]), true);
						apkcount--;
					}
				}
			} else {
				printf("No package specified!\n");
			}
		} else {
			printf("Command \"%s\" not found!\n", argv[1]);
		}
	} else {
		helpscreen(apkmgrver);
	}
	return 0;
}

void helpscreen(int apkmgrver) {
	printf("\napkmgr version %d\n\nusage: apkmgr <operation> [...]\noperations:\n	apkmgr {-h --help}\n	apkmgr {-V --version}\n	apkmgr {-I --syncindex}\n	apkmgr {-Q --query}        [packages(s)]\n	apkmgr {-R --remove}       [options] <packages(s)>\n	apkmgr {-S --sync}         [options] [package(s)]\n	apkmgr {-ver --apkversion} [packageversion]\n	apkmgr {-U --upgrade}\n\n", apkmgrver);
}
