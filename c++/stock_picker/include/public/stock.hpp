#ifndef _STOCK_PICKER_STOCK_H

#define _STOCK_PICKER_STOCK_H

#include<vector>
#include<string>
	
namespace stockpicker{
	
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
		std::string *transactions;
		std::string date; //YYYYMMDD
	
		StockHistoryOverview& setRank(int rank){this->rank = rank;return *this;}
		StockHistoryOverview& setPriceStart(int price_start){this->price_start = price_start;return *this;}
		StockHistoryOverview& setPriceNewst(int price_newst){this->price_newst = price_newst;return *this;}
		StockHistoryOverview& setPriceHigest(int price_higest){this->price_higest = price_higest;return *this;}
		StockHistoryOverview& setPriceLowest(int price_lowest){this->price_lowest = price_lowest;return *this;}
		StockHistoryOverview& setAmplitude(int amplitude){this->amplitude = amplitude;return *this;}
		StockHistoryOverview& setTurnoverRate(int turnover_rate){this->turnover_rate = turnover_rate;return *this;}
		StockHistoryOverview& setTurnoverSum(long turnover_sum){this->turnover_sum = turnover_sum;return *this;}
		StockHistoryOverview& setChangePercent(int change_percent){this->change_percent = change_percent;return *this;}
		StockHistoryOverview& setTransactions(std::string *transactions){this->transactions = transactions;return *this;}
		StockHistoryOverview& setDate(std::string &date){this->date = date;return *this;}
	};
	
	struct Stock{
	public:
		int code;
		int pe;
		long market_value;
		long traded_market_value;
		std::string name;
		std::vector<std::string> sectors;
		StockHistoryOverview history_attr;
	
		Stock() = default;
		Stock(int code):code(code){}

		Stock& setPe(int pe){this->pe = pe;return *this;}
		Stock& setMarketValue(long market_value){this->market_value = market_value;return *this;}
		Stock& setTradedMarketValue(long traded_market_value){this->traded_market_value = traded_market_value;return *this;}
		Stock& setName(const std::string name){this->name = name;return *this;}
		Stock& addSector(std::string sector){this->sectors.push_back(sector);return *this;}
	
		StockHistoryOverview& getHistoryAttr(){return this->history_attr;}
	};
	
	//大盘
	struct Market{
		
	};
}

#endif
