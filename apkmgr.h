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
		printf("not enough memory (realloc returned NULL)\n");
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
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char urlstrng[100];
	sprintf(urlstrng, "https://f-droid.org/repo/%s_%d.apk", apkname, apkver);
	char *url = urlstrng;
	char outfilename[FILENAME_MAX];
	sprintf(outfilename, "%s_%d.apk", apkname, apkver);
	curl = curl_easy_init();
	if (apkver == 0) {
		printf("Error: could not find app version!\n");
	} else if (curl)
	{
		fp = fopen(outfilename,"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}
		else {
			char apkinstallcmd[100];
			sprintf(apkinstallcmd, "xdg-open %s_%d.apk", apkname, apkver);
			system(apkinstallcmd);
		}
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
	sprintf(url, "https://f-droid.org/api/v1/packages/%s", apkname);

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	res = curl_easy_perform(curl_handle);

	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));
	}
	else {
		printf("%lu bytes retrieved\n", (unsigned long)chunk.size);

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
	return 0;
}
