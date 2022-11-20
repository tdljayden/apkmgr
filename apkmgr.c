#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void syncapk(char * apkname);

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

void syncapk(char * apkname) {
	printf("Syncing package: %s!\n", apkname);
	char apkversion[100];
	sprintf(apkversion, "2007");
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char urlstrng[100];
	sprintf(urlstrng, "https://f-droid.org/repo/%s_%s.apk", apkname, apkversion);
	char *url = urlstrng;
	char outfilename[FILENAME_MAX];
	sprintf(outfilename, "%s_%s.apk", apkname, apkversion);
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(outfilename,"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res =  curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
	system("xdg-open %s_%s.apk", apkname, apkversion);
	/* sprintf(apkvercmd, "GET https://f-droid.org/api/v1/packages/%s HTTP/1.1 | grep -oP '(?<=suggestedVersionCode\":).*(?=,\"packages)'", apkname); */
	/* char synccmd[100];
	sprintf(synccmd, "wget https://f-droid.org/repo/%s_%s.apk", apkname, apkversion);
	system(synccmd); */
}
