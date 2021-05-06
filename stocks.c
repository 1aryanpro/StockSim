#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "download.h"
#include <cjson/cJSON.h>

typedef struct
{
	float open;
	float close;
	float avg;
	float high;
	float low;
	char date[11];
	char symbol[];
} MarketStock;

MarketStock *stock_make(cJSON *stockJSON, char *symbol)
{

	cJSON *open = cJSON_GetObjectItem(stockJSON, "open");
	cJSON *close = cJSON_GetObjectItem(stockJSON, "close");
	cJSON *high = cJSON_GetObjectItem(stockJSON, "high");
	cJSON *low = cJSON_GetObjectItem(stockJSON, "low");
	cJSON *date = cJSON_GetObjectItem(stockJSON, "datetime");

	MarketStock *stock = malloc(sizeof(MarketStock));

	stock->open = atof(open->valuestring);
	stock->close = atof(close->valuestring);
	stock->avg = (atof(open->valuestring) + atof(close->valuestring)) / 2.;
	stock->high = atof(high->valuestring);
	stock->low = atof(low->valuestring);

	// printf("%s\n", date->valuestring);

	strncpy(stock->date, date->valuestring, 10);
	stock->date[10] = 0;
	strcpy(stock->symbol, symbol);

	return stock;
}

void stock_print(MarketStock *stock)
{
	printf("{\n\tsymbol: %s\n\tdate: %s\n\topen: %f\n\tclose: %f\n\tavg: %f\n}\n", stock->symbol, stock->date, stock->open, stock->close, stock->avg);
}

int main(int argc, char *argv[])
{
	char *data = NULL;
	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "1")))
	{
		data = getStockPrice("AAPL,GOOG");
	}
	else
	{
		size_t len;
		FILE *fp = fopen("stockData.json", "r");
		ssize_t bytes_read = getdelim(&data, &len, '\0', fp);
		if (bytes_read == -1)
		{
			data = getStockPrice("AAPL,GOOG");
		}
	}

	cJSON *json = cJSON_Parse(data);
	free(data);

	cJSON *aapl = cJSON_GetObjectItem(json, "AAPL");
	cJSON *aapl_values = cJSON_GetObjectItem(aapl, "values");
	int aapl_valuesLen = cJSON_GetArraySize(aapl_values);

	MarketStock *aapl_opens[aapl_valuesLen];

	for (int i = 0; i < aapl_valuesLen; i++)
	{
		cJSON *curr = cJSON_GetArrayItem(aapl_values, i);
		aapl_opens[i] = stock_make(curr, "AAPL");
		stock_print(aapl_opens[i]);
	}

	return 0;
}