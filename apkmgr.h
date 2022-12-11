void syncapk(char * apkname);
int grabapkver(char * apkname);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr) {
		printf("Error: Out of memory!\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void syncapk(char * apkname) {
	printf("Syncing package: %s!\n", apkname);
	int apkver = grabapkver(apkname);

	char inputyn[2];
	char prompt[500];
	int promptlen;

	promptlen = snprintf(prompt, 500, "\nPackage Name - %s\n\nPackage Version - %d\n\nAre you sure you want to install? [Y/n]\n", apkname, apkver);
	if (promptlen >= 0 && promptlen < 500) {
		snprintf(prompt, (promptlen + 1), "\nPackage Name - %s\n\nPackage Version - %d\n\nAre you sure you want to install? [Y/n]\n", apkname, apkver);
		printf("%s\n", prompt);
		scanf("%s", inputyn);
		if (strlen(inputyn) == 1) {
			if (strcmp(inputyn, "y") == 0 || strcmp(inputyn, "Y") == 0 || strcmp(inputyn, " ") == 0) {
				printf("\nDownloading package!\n");
				CURL *curl;
				FILE *fp;
				CURLcode res;

				char urlstrng[100];
				int urlstrnglen;

				urlstrnglen = snprintf(urlstrng, 100,  "https://f-droid.org/repo/%s_%d.apk", apkname, apkver);
				if (urlstrnglen >= 0 && urlstrnglen < 100) {
					snprintf(urlstrng, (urlstrnglen + 1), "https://f-droid.org/repo/%s_%d.apk", apkname, apkver);

					char *url = urlstrng;
					char outfilename[FILENAME_MAX];
					char *home = getenv("HOME");
					int outfilenamelen;

					outfilenamelen = snprintf(outfilename, 100, "%s/.cache/apkmgr/%s_%d.apk", home, apkname, apkver);
					if (outfilenamelen >= 0 && outfilenamelen < 100) {
						curl = curl_easy_init();
						snprintf(outfilename, (outfilenamelen + 1), "%s/.cache/apkmgr/%s_%d.apk", home, apkname, apkver);
						if (apkver == 0) {
							printf("\nError: could not find app version!\n");
						} else if (curl) {
							fp = fopen(outfilename,"wb");
							curl_easy_setopt(curl, CURLOPT_URL, url);
							curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
							curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
							res = curl_easy_perform(curl);
							curl_easy_cleanup(curl);
							fclose(fp);
							if(res != CURLE_OK) {
								fprintf(stderr, "\ncurl_easy_perform() failed: %s\n",
								curl_easy_strerror(res));
							} else {
								char apkinstallcmd[(FILENAME_MAX + 10)];
								int apkinstallcmdlen;

								apkinstallcmdlen = snprintf(apkinstallcmd, (FILENAME_MAX + 10), "xdg-open %s", outfilename);
								if (apkinstallcmdlen >= 0 && apkinstallcmdlen < (FILENAME_MAX + 10)) {
									snprintf(apkinstallcmd, (apkinstallcmdlen + 1), "xdg-open %s", outfilename);

									printf("\nInstalling package!\n");
									system(apkinstallcmd);
								} else {
									printf("Error: Buffer overflow detected!\n");
								}
							}
						}
					} else {
						printf("Error: Buffer overflow detected!\n");
					}
				} else {
					printf("Error: Buffer overflow detected!\n");
				}
			} else if (strcmp(inputyn, "n") == 0 || strcmp(inputyn, "N") == 0) {
				printf("\nAborting install!\n");
			} else {
				printf("\nError: Invalid Response. Please type y or n.\n");
			}
		} else {
			printf("\nError: Invalid Response. Please type y or n.\n");
		}
	} else {
		printf("Error: Buffer overflow detected!\n");
	}
}

int grabapkver(char * apkname) {
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0; 

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	char url[100];
	int urllen;
	
	urllen = snprintf(url, 100, "https://f-droid.org/api/v1/packages/%s", apkname);
	if (urllen >= 0 && urllen < 100) {
		snprintf(url, (urllen + 1), "https://f-droid.org/api/v1/packages/%s", apkname);
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		res = curl_easy_perform(curl_handle);

		if(res != CURLE_OK) {
			fprintf(stderr, "Failed to retrieve package: %s\n",
			curl_easy_strerror(res));
			return 0;
		}
		else {
			printf("Package version retrieved! (%lu bytes)\n", (unsigned long)chunk.size);

			/* Parse JSON to grab the what the latest version is from the fdroid API */
			const cJSON *apkverjson = NULL;
			int apkver;
			cJSON *apkrawjson = cJSON_Parse(chunk.memory);
			apkverjson = cJSON_GetObjectItemCaseSensitive(apkrawjson, "suggestedVersionCode");
			apkver = apkverjson->valueint;
			
			return apkver;
		}

		/* Clean up curl */
		curl_easy_cleanup(curl_handle);
		free(chunk.memory);
		curl_global_cleanup();
	} else {
		printf("Error: Buffer overflow detected!\n");
		return 0;
	}
}
