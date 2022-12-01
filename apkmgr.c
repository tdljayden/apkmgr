#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include <cjson/cJSON.h>

#include "apkmgr.h"

int main(int argc, char **argv)
{
	if(argc==1) {
		printf("Displaying help screen.\n");
	} else if(argc>=2) {
		if(strncmp(argv[1], "sync\n", 4) == 0) {
			if(argc==3) {
				syncapk(argv[2]);
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
