#ifndef _SPIDER_CLASS_
#define _SPIDER_CLASS_

#include<curl/curl.h>
#include<string.h>
#include <iostream>

#define CURL_MAX_ERROR_BUF_LENGTH 1000

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
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, NULL);
		curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _curl_error_buf);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
	}
	
	int getWebContent(const char* url,uint (*_writeCb)(char* ,uint ,uint ,char*)){
		curl_easy_setopt(_curl, CURLOPT_URL, url);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _writeCb);
		return curl_easy_perform(_curl);
	}
};

class SpiderStocksOverview:Spider{
private:
	const char *stock_filter[7] = {"000", "600", "601","602","603","604","605"};

	static uint _writeCb(char *in, uint size, uint nmemb, char *out){
		enum struct JsonState{
			BeforeStart,
			LookForKey,
			InKey,
			LookForValue,
			InValue,
		};

		static JsonState state = JsonState::BeforeStart;
		static char key[100],value[100];
		static int key_index,value_index;
		static int find_item = 0;

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
					if(*in ==',' || *in == '}'){ 
						find_item = 1;
						state = JsonState::LookForKey;
					}else{
						value[value_index++] = *in;
						value[value_index] = '\0';
					}
					break;
			}
			if(find_item){
				find_item = 0;
				std::cout << key << ":" << value << std::endl;
			}
			in++;
		}

		uint r = size * nmemb;
		return r;
	}

public:
	SpiderStocksOverview() = default;

	int getStockByCode(char *stock_code){
		return 0;	
	}

	int getAllStocks(){
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
		,_writeCb);
		return 0;	
	}
};

#endif
