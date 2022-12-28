#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include <cjson/cJSON.h>

#include "apkmgr.h"
#include "sync.h"
#include "index.h"

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
		if (strncmp(argv[1], "sync", 4) == 0 || strncmp(argv[1], "-S", 4) == 0) {
			if (argc >= 3) {
				if (argc == 3) {
					printf("Installing latest version of package!\n");	
					int apkverint = grabapkver(argv[2]);
					if (apkverint > 0) {
						syncapk(argv[2], apkverint, false);
					} else {
						printf("Package version not found!\n");
					}
				} else if (strncmp(argv[3], "-ver", 4) == 0 || strncmp(argv[3], "--apkversion", 4) == 0) {
					if (argc == 4) {
						printf("No package version specified!\n");
					} else if (argc == 5) {
						int apkverint = atoi(argv[4]);
						if (apkverint > 0) {
							printf("Installing package with specified version %d!\n", apkverint);
							syncapk(argv[2], apkverint, false);
						} else {
							printf("Package version not found!\n");
						}
					} else {
						printf("Too many arguments to install!\n");
					}
				} else {
					static char input[1] = "\0";
					int apknumber = 0;
					int apkcount = (argc-2);
					static char **array = NULL;
					while (apkcount != 0) {
						int apknamelocation = (argc - apkcount); 
						int apkverint = grabapkver(argv[apknamelocation]);
						if (apkverint > 0) {
							static char apkfilelocation[1000];
							array = (char**)realloc(array, (apknumber+1)*sizeof(*array));
							array[apknumber] = (char*)malloc(sizeof(apkfilelocation));
							strcpy(array[apknumber], syncapk(argv[apknamelocation], apkverint, true));
							apknumber++;
							apkcount--;
							if (apkcount == 0) {
								printf("Downloading finished!\n");
								int installsleft = (argc-2);
								while (installsleft != 0) {
									static char installcmd[10000];
									int installcmdlen = snprintf(installcmd, 10000,  "xdg-open %s", array[installsleft-1]);
									if (installcmdlen >= 0 && installcmdlen < 10000) {
										snprintf(installcmd, (installcmdlen + 1), "xdg-open %s", array[installsleft-1]);
										printf("Package downloaded at '%s'! Press [ENTER] to install.", array[installsleft-1]);
										scanf("%c", input);
										if (strcmp(input, "\n") == 0) {
											printf("Installing package!\n");
											system(installcmd);
										} else {
											printf("Wrong input! Not installing...\n");
										}
									} else {
										printf("Buffer overflow detected!\n");
									}
									installsleft--;
									if (installsleft == 0) {
										printf("%d packages installed!\n", (argc-2));
									}
								}
							}
						} else {
							printf("Package version not found!\n");
							apkcount--;
						}
					}
				}
			} else {
				printf("No package specified!\n");
			}
		} else if (strcmp(argv[1], "--syncindex") == 0 || strcmp(argv[1], "-I") == 0 ) {
			if (argc == 2) {
				printf("Syncing index!\n");
				syncindex();
			} else {
				printf("Too many arguments!\n");
			}
		} else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0 ) {
			if (argc == 2) {
				helpscreen(apkmgrver);
			} else {
				printf("Too many arguments!\n");
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
