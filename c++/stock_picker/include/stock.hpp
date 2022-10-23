#ifndef _STOCK_CLASS
#define _STOCK_CLASS

#include<vector>

#define MAX_LENGTH_STOCK_NAME 48 
#define MAX_LENGTH_STOCK_ATTR_CONCEPT 28

struct StockHistoryOverview{
	int rank;
	int price_start;
	int price_higest;
	int price_lowest;
	int turnover_rate;
	long turnover_sum;
	int changepercent;
	char date[12]; //YYYYMMDD
};

struct StockHistoryTransaction{
	//top 10
	int price;
	int turnover;
};

struct Stock{
	int code;
	int pe;
	long market_value;
	long traded_market_value;
	char name[MAX_LENGTH_STOCK_NAME];
	int departments[MAX_LENGTH_STOCK_ATTR_CONCEPT];
	StockHistoryOverview history_attr;
	std::vector<StockHistoryTransaction> transactions;
};

#endif
