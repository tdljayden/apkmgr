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
		/* out of memory! */
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
        char apkinstallcmd[100];
        sprintf(apkinstallcmd, "xdg-open %s_%d.apk", apkname, apkver);
	system(apkinstallcmd);
	/* sprintf(apkvercmd, "GET https://f-droid.org/api/v1/packages/%s HTTP/1.1 | grep -oP '(?<=suggestedVersionCode\":).*(?=,\"packages)'", apkname); */
	/* char synccmd[100];
	sprintf(synccmd, "wget https://f-droid.org/repo/%s_%s.apk", apkname, apkversion);
	system(synccmd); */
}

int grabapkver(char * apkname) {
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;

	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* specify URL to get */
	char url[100];
	sprintf(url, "https://f-droid.org/api/v1/packages/%s", apkname);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	/* some servers do not like requests that are made without a user-agent
	*      field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* get it! */
	res = curl_easy_perform(curl_handle);

	/* check for errors */
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
	}
	else {
		/*
		Now, our chunk.memory points to a memory block that is chunk.size
		bytes big and contains the remote file.

		Do something nice with it!
		*/

		printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
		/*char apkver[100];
		sprintf(apkver, "%s\n", chunk.memory);*/

		const cJSON *apkverjson = NULL;
		int apkver;
		cJSON *apkinfo = cJSON_Parse(chunk.memory);
		apkverjson = cJSON_GetObjectItemCaseSensitive(apkinfo, "suggestedVersionCode");
		apkver = apkverjson->valueint;
		
		return apkver;
	}

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	free(chunk.memory);

	/* we are done with libcurl, so clean it up */
	curl_global_cleanup();
}
