#ifndef _SPIDER_CLASS_
#define _SPIDER_CLASS_

#include<curl/curl.h>
#include<string.h>
#include<iostream>
#include<map>
#include <sstream>
#include <time.h>

#include"stock.hpp"

#define CURL_MAX_ERROR_BUF_LENGTH 1000

inline int float_str_to_int(std::string str){
    return static_cast<int>(std::stof(str) * 100);
}

inline long float_str_to_long(std::string str){
    return static_cast<long>(std::stof(str) * 100);
}

//interface
class Spider{
private:
	CURL* _curl;

public:
	char _curl_error_buf[CURL_MAX_ERROR_BUF_LENGTH];

	Spider(){
		_curl = curl_easy_init();	
	}

	~Spider(){
		curl_easy_cleanup(_curl);
		curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _curl_error_buf);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
	}
	
	int getWebContent(const char* url,uint (*_writeCb)(char* ,uint ,uint ,char*), void* out_data = NULL){
		curl_easy_setopt(_curl, CURLOPT_URL, url);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, out_data);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _writeCb);
		return curl_easy_perform(_curl);
	}
};

class SpiderStocksOverview:Spider{
private:
	const char *stock_filter[7] = {"000", "600", "601","602","603","604","605"};
	std::map<int, Stock> stocks;

	static uint _writeCb(char *in, uint size, uint nmemb, char *out_interface){
		enum struct JsonState{
			BeforeStart,
			LookForKey,
			InKey,
			LookForValue,
			InValue,
		};

		/* TODO 完全可以优化成类成员，从而支持多线程，不过暂时没必要 */
		static JsonState state = JsonState::BeforeStart;
		static char key[100],value[100];
		static std::map<std::string, std::string> entries;
		static int key_index,value_index;
		static int find_item = 0;
		
		auto* spiderStocksOverview = reinterpret_cast<SpiderStocksOverview*>(out_interface);

		while(*in){
			switch(state){
				case JsonState::BeforeStart:
					if(*in == '[' && in[1] == '{'){
						state = JsonState::LookForKey;
					}
					break;
				case JsonState::LookForKey:
					if(*in =='"'){
						state = JsonState::InKey;
						key_index = 0;
					}
					break;
				case JsonState::InKey:
					if(*in =='"'){ 
						state = JsonState::LookForValue;
					}else{
						key[key_index++] = *in;
						key[key_index] = '\0';
					}
					break;
				case JsonState::LookForValue:
					if(*in ==':'){ 
						state = JsonState::InValue;
						value_index = 0;
					}
					break;
				case JsonState::InValue:
					if(*in == ',' || *in == '}'){ 
						find_item = 1;
						state = JsonState::LookForKey;
					}else if(*in != '\"'){
						value[value_index++] = *in;
						value[value_index] = '\0';
					}
					break;
			}
			if(find_item){
				entries[key] = value;
				if(*in == '}'){
					for(const auto* stock_code:spiderStocksOverview->stock_filter ){
						if(strcmp(stock_code, value)){
							spiderStocksOverview->addStock(entries);
							break;
						}
					}
				}
				find_item = 0;
			}
			in++;
		}

		uint r = size * nmemb;
		return r;
	}

public:
	SpiderStocksOverview() = default;

	int getAllStocks(){
		/* TODO 这显然不是一个长期的接口 */
		#define _FIELDS "fields="
		#define _BASE_URL "http://45.push2.eastmoney.com/api/qt/clist/get?cb=jQuery112405393508833921838_1666529170574"
		#define _PARAMS "&pn=1&pz=1&po=1&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&wbp2u=|0|0|0|web&fid=f3&fs=m:0+t:6,m:0+t:80,m:1+t:2,m:1+t:23,m:0+t:81+s:2048&"

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

		auto rs = getWebContent( _BASE_URL _PARAMS _FIELDS 
			 _STOCK_NAME_KEY ","
			 _STOCK_CODE_KEY ","
			 _STOCK_NEWST_PRICE_KEY ","
			 _STOCK_TURNOVER_RATE_KEY ","
			 _STOCK_TURNOVER_KEY ","
			 _STOCK_AMPLITUDE_KEY ","
			 _STOCK_HIGEST_PRICE_KEY ","
			 _STOCK_LOWST_PRICE_KEY ","
			 _STOCK_START_PRICE_KEY ","
			 _STOCK_CHANGEPERCENT_KEY ","
			 _STOCK_PE_KEY ","
			 _STOCK_MARKET_VALUE_KEY ","
			 _STOCK_TRADED_MARKET_VALUE_KEY 
			"&_=1666529170575"
		,_writeCb, this);
		return 0;	
	}

	int addStock(std::map<std::string, std::string> &stock_attr){
		int stock_code = std::stoi(stock_attr[_STOCK_CODE_KEY]);
		
		time_t t = time(0);
		char tmp[12];
		
		strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&t));
		std::string date(tmp);

		this->stocks[stock_code] = Stock().setName(stock_attr[_STOCK_NAME_KEY])
										.setPe(float_str_to_int(stock_attr[_STOCK_PE_KEY]))
										.setMarketValue(std::stol(stock_attr[_STOCK_MARKET_VALUE_KEY]))
										.setTradedMarketValue(std::stol(stock_attr[_STOCK_TRADED_MARKET_VALUE_KEY]))
										;
		this->stocks[stock_code].getHistoryAttr().setPriceStart(float_str_to_int(stock_attr[_STOCK_START_PRICE_KEY]))
												.setPriceNewst(float_str_to_int(stock_attr[_STOCK_NEWST_PRICE_KEY]))
												.setPriceHigest(float_str_to_int(stock_attr[_STOCK_HIGEST_PRICE_KEY]))
												.setPriceLowest(float_str_to_int(stock_attr[_STOCK_LOWST_PRICE_KEY]))
												.setAmplitude(float_str_to_int(stock_attr[_STOCK_AMPLITUDE_KEY]))
												.setTurnoverRate(float_str_to_int(stock_attr[_STOCK_TURNOVER_RATE_KEY]))
												.setTurnoverSum(float_str_to_int(stock_attr[_STOCK_TURNOVER_KEY]))
												.setChangePercent(float_str_to_int(stock_attr[_STOCK_CHANGEPERCENT_KEY]))
												.setDate(date.c_str())
										;
		return 0;	
	}

};

#endif
