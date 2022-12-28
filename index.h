int syncindex() {
	CURL *curl;
	FILE *fp;

	char * urlstrng = "https://f-droid.org/repo/index-v2.json";
	char *url = urlstrng;
	char outfilename[FILENAME_MAX];
	char *home = getenv("HOME");
	int outfilenamelen;
	
	outfilenamelen = snprintf(outfilename, 100, "%s/.cache/apkmgr/fdroidindex.json", home);

	if (outfilenamelen >= 0 && outfilenamelen < 100) {
		snprintf(outfilename, (outfilenamelen + 1), "%s/.cache/apkmgr/fdroidindex.json", home);
		curl = curl_easy_init();
		if (curl) {
			fp = fopen(outfilename,"wb");
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_cleanup(curl);
			fclose(fp);
			printf("Index downloaded!\n");
			return 1;
		} else {
			printf("curl unable to initialize!\n");
			return 0;
		}
	} else {
		printf("Buffer overflow detected!\n");
		return 0;
	}
}
