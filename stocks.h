#ifndef stocksHeader
#define stocksHeader

#include <stdlib.h>

typedef struct
{
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

int StockList_getLen(MarketStockList *list);
float *StockList_toArray(MarketStockList *list);
float StockList_getMax(MarketStockList *list);
void StockList_print(MarketStockList *list);
MarketStockList **queryStocksFromUser(const char *queryString, size_t *len, size_t *selectedIndex, char **selectedSymbol);

#endif