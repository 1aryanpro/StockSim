#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memory
{
	char *response;
	size_t size;
} memory;

curl_off_t pdltotal = -1;
curl_off_t pdlnow = -1;

int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	if ((dlnow != 0 && dltotal == 0) || (dltotal == pdltotal && dlnow == pdlnow))
	{
		return 0;
	}

	fprintf(stderr, "Progress: %d%%; %ld / %ld \n", dltotal == 0 ? 0 : (int)((float)dlnow / (float)dltotal * 100.0), dlnow, dltotal);

	pdlnow = dlnow;
	pdltotal = dltotal;

	return 0;
}

static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct memory *mem = (struct memory *)userp;

	char *ptr = realloc(mem->response, mem->size + realsize + 1);
	if (ptr == NULL)
		return 0; /* out of memory! */

	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}

char *download_url(const char *url, size_t *len)
{
	CURL *curl = curl_easy_init();
	if (curl)
	{
		CURLcode res;
		memory chunk = {NULL, 0};

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		// curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		// curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		res = curl_easy_perform(curl);
		if (res != 0)
		{
			fprintf(stderr, "%s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
		len = &chunk.size;
		return chunk.response;
	}
	return NULL;
}

char *getStockPrice(char *symbols)
{
	size_t len = 0;

	char reqStart[] = "https://api.twelvedata.com/time_series?symbol=";
	char reqEnd[] = "&interval=30min&apikey=0e79298e4e524595906decac29f21781";

	char *request;
	asprintf(&request, "%s%s%s", reqStart, symbols, reqEnd);

	char *response = download_url(request, &len);

	free(request);

	return response;
}

// int main(int argc, char *argv[])
// {
// 	if (argc != 2)
// 	{
// 		fprintf(stderr, "Not enough Arguments\n");
// 		return 1;
// 	}

// 	char *response = getStockPrice(argv[1]);

// 	printf("%s\n", response);

// 	free(response);

// 	return 0;
// }
