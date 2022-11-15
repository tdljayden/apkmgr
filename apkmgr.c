#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void syncapk(char * apkname);

int main(int argc, char **argv)
{
	if(argc==1) {
		printf("Displaying help screen.\n");
	} else if(argc>=2) {
		if(strncmp(argv[1], "sync\n", 4) == 0) {
			syncapk(argv[2]);
		} else if(strncmp(argv[1], "upgrade", 7) == 0) { 
			printf("Upgrading all packages!\n");
		} else if(strncmp(argv[1], "remove", 6) == 0) { 
			printf("Removing package!\n");
		}
	}
	return 0;
}

void syncapk(char * apkname) {
	printf("Syncing package: %s!\n", apkname);
	char synccmd[100];
	sprintf(synccmd, "fdroidcl download %s", apkname);
	system(synccmd);
}
