#ifndef _STOCK_PICKER_SPIDER_H
#define _STOCK_PICKER_SPIDER_H

#include<iostream>
#include<map>
#include<sstream>

#include<curl/curl.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>

#include"../public/stock.hpp"
#include"../public/mysqltool.hpp"
#include"../public/filetool.hpp"
#include"../public/log.hpp"

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

namespace stockpicker{

	inline int float_str_to_int(std::string str){
	    return static_cast<int>(round(std::stof(str) * 100));
	}
	
	inline long float_str_to_long(std::string str){
	    return static_cast<long>(round(std::stof(str) * 100));
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
		std::string date;
		std::string curl_in;
		char curl_error_buf[CURL_MAX_ERROR_BUF_LENGTH];

		SimpleLog &simple_log = SimpleLog::getInstance();
	
		Spider(){
			_curl = curl_easy_init();	
	
			char tmp[12];
			time_t t(time(NULL));
	
			strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&t));
			date = std::string(tmp);
		}
	
		~Spider(){
			curl_easy_cleanup(_curl);
			curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, curl_error_buf);
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
		MySQLTool *_mysqltool;	
		FileTool *_filetool;

		const char *_stock_filter[7] = {"0", "600", "601","602","603","604","605"};
		std::map<std::string, Stock> _stocks;
		std::string _cur_stock_code;

		//内存扛不住, 只能出此下策
		std::string _transactions;
	
		static uint _parseTransactions(char *in, uint size, uint nmemb, char *out_interface);
		static uint _parseSectors(char *in, uint size, uint nmemb, char *out_interface);
		static uint _parseRank(char *in, uint size, uint nmemb, char *out_interface);
		static uint _parseStockBasicInfo(char *in, uint size, uint nmemb, char *out_interface);
	
		SpiderStocksOverview& _getTransactions();
		SpiderStocksOverview& _getSectors();
		SpiderStocksOverview& _getRank();
	
		void _addStock(std::map<std::string, std::string> &stock_attr);
		void _addRank(std::string rank);
		void _addSector(std::string sector);
		void _addTransaction(std::string transaction);
	
		int _updateStockInfo(const Stock&);
		int _updateStockHistory(const Stock&);
		int _updateStockSectors(const Stock&);

		int _storageStockTransactions(const std::string& tas, const std::string& date, const std::string stock_code);

	public:
		SpiderStocksOverview(MySQLTool *mysqltool, FileTool *filetool):_mysqltool(mysqltool), _filetool(filetool){}

		int getAllStocks(std::string spec_stock = "", std::string spec_date = "");
	};
}

#endif
