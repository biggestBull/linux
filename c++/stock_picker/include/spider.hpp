#ifndef _SPIDER_CLASS_
#define _SPIDER_CLASS_

#include<curl/curl.h>
#include<iostream>
#include<map>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include<stdlib.h>

#include"stock.hpp"

#define CURL_MAX_ERROR_BUF_LENGTH 1000

#define _STOCK_NAME_KEY "f14"
#define _STOCK_CODE_KEY "f12"
#define _STOCK_NEWST_PRICE_KEY "f2"
#define _STOCK_TURNOVER_RATE_KEY "f3"
#define _STOCK_TURNOVER_KEY "f6"
#define _STOCK_AMPLITUDE_KEY "f7"
#define _STOCK_HIGEST_PRICE_KEY "f15"
#define _STOCK_LOWST_PRICE_KEY "f16"
#define _STOCK_START_PRICE_KEY "f17"
#define _STOCK_CHANGEPERCENT_KEY "f8"
#define _STOCK_PE_KEY "f9"
#define _STOCK_MARKET_VALUE_KEY "f20"
#define _STOCK_TRADED_MARKET_VALUE_KEY "f21"

inline int float_str_to_int(std::string str){
    return static_cast<int>(std::stof(str) * 100);
}

inline long float_str_to_long(std::string str){
    return static_cast<long>(std::stof(str) * 100);
}
	
inline void randomSleep(int min, int max){
	srand((unsigned)time(NULL));
	sleep( ( rand () % (max-min+1) ) + min);
}

//interface
class Spider{
private:
	CURL* _curl;

protected:
	char _curl_error_buf[CURL_MAX_ERROR_BUF_LENGTH];
	std::string date;

	Spider(){
		_curl = curl_easy_init();	

		char tmp[12];
		time_t t(time(NULL));

		strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&t));
		date = std::string(tmp);
	}

	~Spider(){
		curl_easy_cleanup(_curl);
		curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _curl_error_buf);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
	}
	
	int getWebContent(const char* url,uint (*_writeCb)(char* ,uint ,uint ,char*), void* out_data, int post_req = 0, struct curl_slist * headers  =  NULL, const char *data = ""){
		curl_easy_reset(_curl);

		curl_easy_setopt(_curl, CURLOPT_URL, url);
		
		if(post_req){
			curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "POST");
			curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(_curl, CURLOPT_POST, 1);   //设置为非0表示本次操作为POST
			curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, data);
		}

		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, out_data);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _writeCb);
		return curl_easy_perform(_curl);
	}
};

class SpiderStocksOverview:Spider{
private:
	const char *stock_filter[7] = {"0", "600", "601","602","603","604","605"};
	std::map<std::string, Stock> stocks;
	std::string cur_stock_code;

	static uint _getTransactions(char *in, uint size, uint nmemb, char *out_interface);
	static uint _getSectors(char *in, uint size, uint nmemb, char *out_interface);
	static uint _getRank(char *in, uint size, uint nmemb, char *out_interface);
	static uint _getStockBasicInfo(char *in, uint size, uint nmemb, char *out_interface);

	void addStock(std::map<std::string, std::string> &stock_attr);
	void addRank(std::string rank);
	void addSector(std::string sector);
	void addTransaction(std::string transaction);

public:
	SpiderStocksOverview() = default;

	SpiderStocksOverview& getTransactions();
	SpiderStocksOverview& getSectors();
	SpiderStocksOverview& getRank();
	SpiderStocksOverview& getAllStocks();
};

#endif
