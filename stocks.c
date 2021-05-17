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

typedef struct
{
	float buyValue;
	char symbol[];
} OwnedStock;

MarketStock *marketStock_make(cJSON *stockJSON, char *symbol)
{

	// float open = atof(cJSON_GetObjectItem(stockJSON, "open")->valuestring);
	float close = atof(cJSON_GetObjectItem(stockJSON, "close")->valuestring);
	// float high = atof(cJSON_GetObjectItem(stockJSON, "high")->valuestring);
	// float low = atof(cJSON_GetObjectItem(stockJSON, "low")->valuestring);
	char *date = cJSON_GetObjectItem(stockJSON, "datetime")->valuestring;

	MarketStock *stock = malloc(sizeof(MarketStock) + strlen(symbol) + 1);

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

void marketStock_print(MarketStock *stock)
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
	char *symbol = strdup(cJSON_GetObjectItem(meta, "symbol")->valuestring);

	cJSON *values = cJSON_GetObjectItem(data, "values");

	MarketStock *first = marketStock_make(cJSON_GetArrayItem(values, 0), symbol);
	MarketStockList *head = StockList_makeElement(first, NULL);

	MarketStockList *curr = head;

	for (int i = 1; i < cJSON_GetArraySize(values); i++)
	{
		MarketStock *stock = marketStock_make(cJSON_GetArrayItem(values, i), symbol);
		MarketStockList *next = StockList_makeElement(stock, curr);
		curr->next = next;
		curr = next;
	}

	curr->next = NULL;

	return head;
}

int StockList_getLen(MarketStockList *list)
{
	int count = 1;

	MarketStockList *curr = list;

	while (curr->next != NULL)
	{
		count++;
		curr = curr->next;
	}

	return count;
}

void StockList_print(MarketStockList *list)
{
	MarketStockList *curr = list;

	while (curr->next != NULL)
	{
		marketStock_print(curr->stock);
		curr = curr->next;
	}
}

float *StockList_toArray(MarketStockList *list)
{
	int len = StockList_getLen(list);
	float *output = malloc(sizeof(float) * len);
	MarketStockList *curr = list;

	for (int i = 0; i < len; i++)
	{
		output[i] = curr->stock->value;
		curr = curr->next;
	}

	return output;
}

OwnedStock *stock_make(float buyValue, char *symbol)
{
	OwnedStock *stock = malloc(sizeof(OwnedStock) + strlen(symbol) + 1);
	stock->buyValue = buyValue;
	strcpy(stock->symbol, symbol);
	return stock;
}

OwnedStock *buyStock(MarketStockList **stockVals, size_t len, char *symbol)
{
	int index = -1;

	for (size_t i = 0; i < len; i++)
	{
		if (strcmp(symbol, stockVals[i]->stock->symbol))
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		return NULL;
	}

	MarketStockList *end = stockVals[index];
	while (end->next != NULL)
	{
		end = end->next;
	}

	OwnedStock *stock = stock_make(end->stock->value, symbol);
	return stock;
}

MarketStockList **queryStocks(const char *queryString, size_t *len)
{
	char *data = getStockPrice(queryString);
	cJSON *json = cJSON_Parse(data);

	cJSON *first = json->child;

	size_t stockDataLen = 1;
	cJSON *curr = first;

	while (curr->next != NULL)
	{
		stockDataLen++;
		curr = curr->next;
	}

	MarketStockList **stockData = malloc(sizeof(MarketStockList)*stockDataLen);
	curr = first;

	for (int i = 0; i < stockDataLen; i++)
	{
		stockData[i] = StockList_makeList(curr);
		StockList_print(stockData[i]);
		curr = curr->next;
	}

	*len = stockDataLen;

	return stockData;
}

int main(int argc, char *argv[])
{
	char *data = NULL;
	char *queryString;
	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "1")))
	{
		queryString = "AAPL,GOOG";
	}
	else
	{
		size_t len;
		FILE *fp = fopen("stockData.json", "r");
		ssize_t bytes_read = getdelim(&data, &len, '\0', fp);
		if (bytes_read == -1)
		{
			queryString = "AAPL,GOOG";
		}
	}

	if (data == NULL)
	{
		data = getStockPrice(queryString);
	}

	cJSON *json = cJSON_Parse(data);
	free(data);

	cJSON *first = json->child;

	size_t stockDataLen = 1;
	cJSON *curr = first;

	while (curr->next != NULL)
	{
		stockDataLen++;
		curr = curr->next;
	}

	MarketStockList *stockData[stockDataLen];
	curr = first;

	for (int i = 0; i < stockDataLen; i++)
	{
		stockData[i] = StockList_makeList(curr);
		StockList_print(stockData[i]);
		curr = curr->next;
	}

	return 0;
}
