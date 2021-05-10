#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "download.h"
#include <cjson/cJSON.h>

typedef struct
{
	// float open;
	// float close;
	// float avg;
	// float high;
	// float low;
	float value;
	char date[11];
	char symbol[];
} MarketStock;

struct MarketStockList
{
	MarketStock *stock;
	struct MarketStockList *next;
	struct MarketStockList *prev;
};
typedef struct MarketStockList MarketStockList;

MarketStock *stock_make(cJSON *stockJSON, char *symbol)
{

	// float open = atof(cJSON_GetObjectItem(stockJSON, "open")->valuestring);
	float close = atof(cJSON_GetObjectItem(stockJSON, "close")->valuestring);
	// float high = atof(cJSON_GetObjectItem(stockJSON, "high")->valuestring);
	// float low = atof(cJSON_GetObjectItem(stockJSON, "low")->valuestring);
	char *date = cJSON_GetObjectItem(stockJSON, "datetime")->valuestring;

	MarketStock *stock = malloc(sizeof(MarketStock));

	// stock->open = open;
	// stock->close = close;
	// stock->avg = (open + close) / 2.;
	// stock->high = high;
	// stock->low = low;
	stock->value = close;

	strncpy(stock->date, date, 10);
	stock->date[10] = 0;
	strcpy(stock->symbol, symbol);

	return stock;
}

void stock_print(MarketStock *stock)
{
	// printf("{\n\tsymbol: %s\n\tdate: %s\n\topen: %f\n\tclose: %f\n\tavg: %f\n}\n", stock->symbol, stock->date, stock->open, stock->close, stock->avg);
	printf("{\n\tsymbol: %s\n\tdate: %s\n\tvalue: %f\n}\n", stock->symbol, stock->date, stock->value);
}

MarketStockList *StockList_makeElement(MarketStock *stock, MarketStockList *prev)
{
	MarketStockList *le = malloc(sizeof(MarketStockList));
	le->stock = stock;
	le->prev = prev;

	return le;
}

MarketStockList *StockList_makeList(cJSON *data)
{
	cJSON *meta = cJSON_GetObjectItem(data, "meta");
	char *symbol = "";
	strcpy(symbol, cJSON_GetObjectItem(meta, "symbol")->valuestring);

	cJSON *values = cJSON_GetObjectItem(data, "values");

	MarketStock *first = stock_make(cJSON_GetArrayItem(values, 0), symbol);
	MarketStockList *head = StockList_makeElement(first, NULL);

	return head;
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