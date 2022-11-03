#ifndef _STOCK_CLASS
#define _STOCK_CLASS

#include<vector>
#include<string.h>

#define MAX_LENGTH_STOCK_ATTR_CONCEPT 28

struct StockHistoryTransaction{
	//top 10
	int price;
	int turnover;

	StockHistoryTransaction(int price, int turnover):price(price), turnover(turnover){}
};

struct StockHistoryOverview{
	int rank;
	int price_start;
	int price_newst;
	int price_higest;
	int price_lowest;
	int amplitude;
	int turnover_rate;
	long turnover_sum;
	int change_percent;
	std::vector<StockHistoryTransaction> transactions;
	char date[12]; //YYYYMMDD

	StockHistoryOverview& setRank(int rank){this->rank = rank;return *this;}
	StockHistoryOverview& setPriceStart(int price_start){this->price_start = price_start;return *this;}
	StockHistoryOverview& setPriceNewst(int price_newst){this->price_newst = price_newst;return *this;}
	StockHistoryOverview& setPriceHigest(int price_higest){this->price_higest = price_higest;return *this;}
	StockHistoryOverview& setPriceLowest(int price_lowest){this->price_lowest = price_lowest;return *this;}
	StockHistoryOverview& setAmplitude(int amplitude){this->amplitude = amplitude;return *this;}
	StockHistoryOverview& setTurnoverRate(int turnover_rate){this->turnover_rate = turnover_rate;return *this;}
	StockHistoryOverview& setTurnoverSum(int turnover_sum){this->turnover_sum = turnover_sum;return *this;}
	StockHistoryOverview& setChangePercent(int change_percent){this->change_percent = change_percent;return *this;}
	StockHistoryOverview& addTransaction(int price, int turnover){this->transactions.push_back(StockHistoryTransaction(price, turnover));return *this;}
	StockHistoryOverview& setDate(const char date[12]){strncpy(this->date, date, 12);return *this;}
};

struct Stock{
private:
	int departments_index;
public:
	int pe;
	long market_value;
	long traded_market_value;
	std::string name;
	int departments[MAX_LENGTH_STOCK_ATTR_CONCEPT];
	StockHistoryOverview history_attr;

	Stock& setPe(int pe){this->pe = pe;return *this;}
	Stock& setMarketValue(long market_value){this->market_value = market_value;return *this;}
	Stock& setTradedMarketValue(long traded_market_value){this->traded_market_value = traded_market_value;return *this;}
	Stock& setName(const std::string name){this->name = name;return *this;}
	Stock& addDp(int dp){this->departments[this->departments_index++] = dp;return *this;}

	StockHistoryOverview& getHistoryAttr(){return this->history_attr;}
};

//大盘
struct Market{
	
};

#endif
