#include"../include/spider.hpp"

/* -----------------------------------  解析器 Start -------------------------------------  */

std::string findSpecContent(const char *before, const char *after, char *&in, int reset = 0){
	enum struct HTMLState{
		BeforeFind,
		Find 
	};

	static HTMLState state = HTMLState::BeforeFind;
	static char buf[1000000];
	static int buf_i = 0;
	static int i = 0;

	if(reset){
		buf_i  =  0;
		i = 0;
		state = HTMLState::BeforeFind;
	}

	while(in && *in){
		switch(state){
			case HTMLState::BeforeFind:
				while(*in == before[i] && before[i]) i++, in++;
				if(!before[i]){ 
					i = 0;
					state = HTMLState::Find;
				}else if(*in){
					i = 0, in++;
				}
				break;
			case HTMLState::Find:
				while(*in == after[i] && after[i]) buf[buf_i++] = *in, i++, in++;
					
				if(!after[i]){ 
					buf[buf_i - i] = '\0';

					buf_i  =  0;
					i = 0;
					state = HTMLState::BeforeFind;
					return buf;
				}else if(*in){
					i = 0;
					buf[buf_i++] = *in++;
				}
				buf[buf_i] = '\0';
				break;
		}
	}
	
	return "";	
}

uint SpiderStocksOverview::_getTransactions(char *in, uint size, uint nmemb, char *out_interface){
	const char *before = "\"details\":";
	const char *after = "}";
	
	auto* spiderStocksOverview = reinterpret_cast<SpiderStocksOverview*>(out_interface);
	std::string ta_str = findSpecContent(before, after, in);
	if(ta_str.length()){
		spiderStocksOverview->addTransaction(ta_str);
	}

	uint r = size * nmemb;
	return r;
}

uint SpiderStocksOverview::_getSectors(char *in, uint size, uint nmemb, char *out_interface){
	const char *before = "\"BOARD_NAME\":";
	const char *after = ",";

	auto* spiderStocksOverview = reinterpret_cast<SpiderStocksOverview*>(out_interface);

	std::string sector_str = findSpecContent(before, after, in);
	while(*in){
		if(sector_str.length()){
			spiderStocksOverview->addSector(sector_str);
			sector_str = findSpecContent(before, after, in);
		}
	}

	uint r = size * nmemb;
	return r;
}

uint SpiderStocksOverview::_getRank(char *in, uint size, uint nmemb, char *out_interface){
	const char *before = "\"rank\":";
	const char *after = ",";
	
	auto* spiderStocksOverview = reinterpret_cast<SpiderStocksOverview*>(out_interface);
	std::string rank_str = findSpecContent(before, after, in);
	if(rank_str.length()){
		spiderStocksOverview->addRank(rank_str);
	}
	uint r = size * nmemb;
	return r;
}

uint SpiderStocksOverview::_getStockBasicInfo(char *in, uint size, uint nmemb, char *out_interface){
	enum struct JsonState{
		BeforeStart_1,
		BeforeStart_2,
		LookForKey,
		InKey,
		LookForValue,
		InValue,
	};

	/* TODO 完全可以优化成类成员，从而支持多线程，不过暂时没必要 */
	static JsonState state = JsonState::BeforeStart_1;
	static char key[100],value[100];
	static std::map<std::string, std::string> entries;
	static int key_index,value_index;
	static int find_item = 0;
	
	auto* spiderStocksOverview = reinterpret_cast<SpiderStocksOverview*>(out_interface);
	while(*in){
		switch(state){
			case JsonState::BeforeStart_1:
				if(*in == '['){
					state = JsonState::BeforeStart_2;
				}
				break;
			case JsonState::BeforeStart_2:
				if(*in == '{'){
					state = JsonState::LookForKey;
				}else{
					state = JsonState::BeforeStart_1;
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
					auto* _code_ = entries[_STOCK_CODE_KEY].c_str();
					int i = 0;
					while(stock_code[i]) if(stock_code[i] == _code_[i]) i += !!stock_code[i];else break;
						if(!stock_code[i]){
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

/* -----------------------------------  解析器 End -------------------------------------  */

SpiderStocksOverview& SpiderStocksOverview::getTransactions(){
	// XXX 也省略了一些参数，不知道次数多了会不会就会被判定为爬虫
	// f51-f55: 成交时间，成交价，成交量，多少单（集合竞价时可能会为0），主卖&主买(1主卖2主买)
	const char *base_url = "https://push2.eastmoney.com/api/qt/stock/details/get?fields1=f1&fields2=f51,f52,f53,f54,f55&pos=-1000000&iscca=1&secid=";

	char *_temp = nullptr;
	findSpecContent(_temp, _temp, _temp, 1);

	for(auto iter = this->stocks.rbegin(); iter != this->stocks.rend(); iter++){
		std::string url = base_url + std::string(iter->first[0] == '0'?"0.":"1.") + std::string(iter->first);

		this->cur_stock_code = iter->first;
		auto rs = getWebContent( url.c_str(), _getTransactions, this); 
		randomSleep(1, 3); //[min, max]
	}
	return *this;
}

SpiderStocksOverview& SpiderStocksOverview::getSectors(){
	const char *base_url = "http://emweb.securities.eastmoney.com/PC_HSF10/CoreConception/PageAjax?code=";

	char *_temp = nullptr;
	findSpecContent(_temp, _temp, _temp, 1);
		
	for(auto iter = this->stocks.rbegin(); iter != this->stocks.rend(); iter++){
		std::string url = base_url + std::string(iter->first[0] == '0'?"SZ":"SH") + std::string(iter->first);

		this->cur_stock_code = iter->first;
		auto rs = getWebContent( url.c_str(), _getSectors, this); 
		randomSleep(1, 3); //[min, max]
	}
	return *this;
}
	
SpiderStocksOverview& SpiderStocksOverview::getRank(){
	const char *base_url = "https://emappdata.eastmoney.com/stockrank/getCurrentLatest";

	char *_temp = nullptr;
	findSpecContent(_temp, _temp, _temp, 1);
		
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");

	for(auto iter = this->stocks.rbegin(); iter != this->stocks.rend(); iter++){
		// XXX 发现除了srcSecurityCode, 其它的居然可以不填，不知道次数多了会不会就会被判定为爬虫
		std::string params = "{"
			 "\"appId\": \"\", "
			 "\"globalId\": \"\","
			 "\"marketType\": \"\","
			 "\"srcSecurityCode\":\"" + std::string(iter->first[0] == '0'?"SZ":"SH") + std::string(iter->first) + "\""
			"}"
		;
		this->cur_stock_code = iter->first;
		auto rs = getWebContent( base_url, _getRank, this, 1, headers, params.c_str()); 
		randomSleep(1, 3); //[min, max]
	}

	return *this;	
}

SpiderStocksOverview& SpiderStocksOverview::getAllStocks(){
	/* TODO 这显然不是一个长期的接口 */
	#define _FIELDS "fields="
	#define _BASE_URL "http://45.push2.eastmoney.com/api/qt/clist/get?cb=jQuery112405393508833921838_1666529170574"
	#define _PARAMS "&pn=1&pz=27&po=1&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&wbp2u=|0|0|0|web&fid=f3&fs=m:0+t:6,m:0+t:80,m:1+t:2,m:1+t:23,m:0+t:81+s:2048&"
	
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
		,_getStockBasicInfo, this);

	return *this;	
}

void SpiderStocksOverview::addStock(std::map<std::string, std::string> &stock_attr){
	this->stocks[stock_attr[_STOCK_CODE_KEY]] = Stock().setName(stock_attr[_STOCK_NAME_KEY])
									.setPe(float_str_to_int(stock_attr[_STOCK_PE_KEY]))
									.setMarketValue(std::stol(stock_attr[_STOCK_MARKET_VALUE_KEY]))
									.setTradedMarketValue(std::stol(stock_attr[_STOCK_TRADED_MARKET_VALUE_KEY]))
								;
	this->stocks[stock_attr[_STOCK_CODE_KEY]].getHistoryAttr().setPriceStart(float_str_to_int(stock_attr[_STOCK_START_PRICE_KEY]))
										.setPriceNewst(float_str_to_int(stock_attr[_STOCK_NEWST_PRICE_KEY]))
										.setPriceHigest(float_str_to_int(stock_attr[_STOCK_HIGEST_PRICE_KEY]))
										.setPriceLowest(float_str_to_int(stock_attr[_STOCK_LOWST_PRICE_KEY]))
										.setAmplitude(float_str_to_int(stock_attr[_STOCK_AMPLITUDE_KEY]))
										.setTurnoverRate(float_str_to_int(stock_attr[_STOCK_TURNOVER_RATE_KEY]))
										.setTurnoverSum(float_str_to_int(stock_attr[_STOCK_TURNOVER_KEY]))
										.setChangePercent(float_str_to_int(stock_attr[_STOCK_CHANGEPERCENT_KEY]))
										.setDate(date.c_str())
								;
}

void SpiderStocksOverview::addRank(std::string rank){
	this->stocks[this->cur_stock_code].getHistoryAttr().setRank(std::stoi(rank));
	std::cout<<this->cur_stock_code<<", "<<rank<<std::endl;
}

void SpiderStocksOverview::addSector(std::string sector){
	this->stocks[this->cur_stock_code].addSector(sector);
	std::cout<<this->cur_stock_code<<", "<<sector<<std::endl;
}

void SpiderStocksOverview::addTransaction(std::string transaction){
	std::cout<<this->cur_stock_code<<", "<<transaction<<std::endl;
}

