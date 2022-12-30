char *getindexpath();

int syncindex() {
	CURL *curl;
	FILE *fp;

	char * urlstrng = "https://f-droid.org/repo/index-v2.json";
	char *url = urlstrng;
	char * outfilename;
	outfilename = getindexpath();

	curl = curl_easy_init();
	if (curl) {
		fp = fopen(outfilename,"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
		printf("Index downloaded!\n");
		return 1;
	} else {
		printf("curl unable to initialize!\n");
		return 0;
	}
}

const cJSON *parseindex(char * apkname) {
	char * buffer = 0;
	long length;
	FILE * f = fopen(getindexpath(), "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length);
		if (buffer) {
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}

	if (buffer) {
		const cJSON *apkinfo = NULL;
		apkinfo = cJSON_Parse(buffer);
		return apkinfo;
	}
}

char *getindexpath() {
	char *home = gethome();
	char outfilename[(strlen(home) + 32)];
	int outfilenamelen;
	
	outfilenamelen = snprintf(outfilename, 100, "%s/.cache/apkmgr/fdroidindex.json", home);

	if (outfilenamelen >= 0 && outfilenamelen < 100) {
		snprintf(outfilename, (outfilenamelen + 1), "%s/.cache/apkmgr/fdroidindex.json", home);
		char *outfilenamecpy;
		outfilenamecpy = (char*)malloc(sizeof(outfilename));
		strcpy(outfilenamecpy, outfilename);
		
		return outfilenamecpy;
	}
}
