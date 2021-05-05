#include <stdio.h>
#include <string.h>
#include "download.h"
#include <cjson/cJSON.h>

typedef struct
{
	float open;
	float close;
	float avg;
	char date[19];
} Stock;

Stock *stock_make(cJSON *stockJSON)
{
	cJSON *open = cJSON_GetObjectItem(stockJSON, "open");
	cJSON *close = cJSON_GetObjectItem(stockJSON, "close");
	cJSON *date = cJSON_GetObjectItem(stockJSON, "date");

	Stock *stock = malloc(sizeof(Stock));

	stock->open = atof(open->valuestring);
	stock->close = atof(close->valuestring);
	stock->avg = (atof(open->valuestring) + atof(close->valuestring)) / 2.;

	strcpy(stock->date, date->valuestring);

	return stock;
}

void stock_print(Stock *stock)
{
	printf("{\n\tdate: %s\n\topen: %f\n\tclose: %f\n\tavg: %f\n}\n", stock->date, stock->open, stock->close, stock->avg);
}

int main()
{
	char *data = getStockPrice("AAPL,GOOG");

	cJSON *json = cJSON_Parse(data);
	free(data);

	cJSON *aapl = cJSON_GetObjectItem(json, "AAPL");
	cJSON *aapl_values = cJSON_GetObjectItem(aapl, "values");
	int aapl_valuesLen = cJSON_GetArraySize(aapl_values);

	Stock *aapl_opens[aapl_valuesLen];

	for (int i = 0; i < aapl_valuesLen; i++)
	{
		cJSON *curr = cJSON_GetArrayItem(aapl_values, i);
		aapl_opens[i] = stock_make(curr);
		// stock_print(aapl_opens[i]);
	}

	return 0;
}