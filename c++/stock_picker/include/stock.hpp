#ifndef _STOCK_CLASS
#define _STOCK_CLASS

#include<vector>

#define MAX_LENGTH_STOCK_NAME 48 
#define MAX_LENGTH_STOCK_ATTR_CONCEPT 28

struct StockAttr{
	int rank;
	int industries_involved[MAX_LENGTH_STOCK_ATTR_CONCEPT];
	int departments[MAX_LENGTH_STOCK_ATTR_CONCEPT];
};

struct StockTransaction{
	//time(second) offset
	int time;
	int price;
	int turnover;
};

struct Stock{
	int code;
	char name[MAX_LENGTH_STOCK_NAME];
	char date[12]; //YYYYMMDD
	StockAttr attrs;
	std::vector<StockTransaction> transactions;
};

#endif
