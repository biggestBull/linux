#include"../../include/spider/spider.hpp"

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

					buf_i = 0;
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

uint stockpicker::SpiderStocksOverview::_parseTransactions(char *in, uint size, uint nmemb, char *out_interface){
	const char *before = "\"details\":";
	const char *after = "}";
	
	auto* spiderStocksOverview = reinterpret_cast<stockpicker::SpiderStocksOverview*>(out_interface);
	spiderStocksOverview->curl_in += in;

	std::string ta_str = findSpecContent(before, after, in);
	if(ta_str.length()){
		spiderStocksOverview->_addTransaction(ta_str);
	}

	uint r = size * nmemb;
	return r;
}

uint stockpicker::SpiderStocksOverview::_parseSectors(char *in, uint size, uint nmemb, char *out_interface){
	const char *before = "\"BOARD_NAME\":";
	const char *after = ",";

	auto* spiderStocksOverview = reinterpret_cast<stockpicker::SpiderStocksOverview*>(out_interface);
	spiderStocksOverview->curl_in += in;

	std::string sector_str = findSpecContent(before, after, in);
	while(*in){
		if(sector_str.length()){
			spiderStocksOverview->_addSector(sector_str);
			sector_str = findSpecContent(before, after, in);
		}
	}

	uint r = size * nmemb;
	return r;
}

uint stockpicker::SpiderStocksOverview::_parseRank(char *in, uint size, uint nmemb, char *out_interface){
	const char *before = "\"rank\":";
	const char *after = ",";
	
	auto* spiderStocksOverview = reinterpret_cast<stockpicker::SpiderStocksOverview*>(out_interface);
	spiderStocksOverview->curl_in += in;

	std::string rank_str = findSpecContent(before, after, in);
	if(rank_str.length()){
		spiderStocksOverview->_addRank(rank_str);
	}
	uint r = size * nmemb;
	return r;
}

uint stockpicker::SpiderStocksOverview::_parseStockBasicInfo(char *in, uint size, uint nmemb, char *out_interface){
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
	static int find_item;

	if(out_interface == nullptr){
		state = JsonState::BeforeStart_1;
		key_index = value_index = find_item = 0;
		entries.clear();
		return 0;
	}
	
	auto* spiderStocksOverview = reinterpret_cast<stockpicker::SpiderStocksOverview*>(out_interface);
	spiderStocksOverview->curl_in += in;

	while(in && *in){
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
				for(const auto* stock_code:spiderStocksOverview->_stock_filter ){
					auto* _code_ = entries[_STOCK_CODE_KEY].c_str();
					int i = 0;
					while(stock_code[i]) if(stock_code[i] == _code_[i]) i += !!stock_code[i];else break;
					if(!stock_code[i]){
						spiderStocksOverview->_addStock(entries);
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

stockpicker::SpiderStocksOverview& stockpicker::SpiderStocksOverview::_getTransactions(){
	// XXX 也省略了一些参数，不知道次数多了会不会就会被判定为爬虫
	// f51-f55: 成交时间，成交价，成交量，多少单（集合竞价时可能会为0），主卖&主买(1主卖2主买)
	const char *base_url = "https://push2.eastmoney.com/api/qt/stock/details/get?fields1=f1&fields2=f51,f52,f53,f54,f55&pos=-1000000&iscca=1&secid=";

	//清除状态
	curl_in.clear();
	char *temp = nullptr;
	findSpecContent(temp, temp, temp, 1);

	std::string url = base_url + std::string(_cur_stock_code[0] == '0'?"0.":"1.") + _cur_stock_code;

	int rs = getWebContent( url.c_str(), _parseTransactions, this); 
	if(rs){
		simple_log.error("Get Stock Transactions", _cur_stock_code, std::to_string(rs), curl_error_buf);
	}else{
		randomSleep(1, 3); //[min, max]
		if(!_transactions.empty()){
			simple_log.info("Get Stock Transactions", _cur_stock_code, true, "");
		}else{
			simple_log.warn("Get Stock Transactions", _cur_stock_code, "", curl_in);
		}
	}				   
	return *this;
}

stockpicker::SpiderStocksOverview& stockpicker::SpiderStocksOverview::_getSectors(){
	const char *base_url = "http://emweb.securities.eastmoney.com/PC_HSF10/CoreConception/PageAjax?code=";

	//清除状态
	curl_in.clear();
	char *temp = nullptr;
	findSpecContent(temp, temp, temp, 1);
		
	std::string url = base_url + std::string(_cur_stock_code[0] == '0'?"SZ":"SH") + _cur_stock_code;

	int rs = getWebContent( url.c_str(), _parseSectors, this); 
	if(rs){
		simple_log.error("Get Stock Sectors", _cur_stock_code, std::to_string(rs), curl_error_buf);
	}else{
		randomSleep(1, 3); //[min, max]
		if(_stocks[_cur_stock_code].sectors.size() > 0){
			simple_log.info("Get Stock Sectors", _cur_stock_code, true, "SIZE :" + std::to_string(_stocks[_cur_stock_code].sectors.size()));
		}else{
			simple_log.warn("Get Stock Sectors", _cur_stock_code, "SIZE :" + std::to_string(_stocks[_cur_stock_code].sectors.size()), curl_in);
		}
	}	
	return *this;
}
	
stockpicker::SpiderStocksOverview& stockpicker::SpiderStocksOverview::_getRank(){
	const char *base_url = "https://emappdata.eastmoney.com/stockrank/getCurrentLatest";

	//清除状态
	curl_in.clear();
	char *temp = nullptr;
	findSpecContent(temp, temp, temp, 1);
		
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");

	// XXX 发现除了srcSecurityCode, 其它的居然可以不填，不知道次数多了会不会就会被判定为爬虫
	std::string params = "{"
		 "\"appId\": \"\", "
		 "\"globalId\": \"\","
		 "\"marketType\": \"\","
		 "\"srcSecurityCode\":\"" + std::string(_cur_stock_code[0] == '0'?"SZ":"SH") + _cur_stock_code + "\""
		"}"
	;
	int rs = getWebContent( base_url, _parseRank, this, 1, headers, params.c_str()); 
	if(rs){
		simple_log.error("Get Stock Rank", _cur_stock_code, std::to_string(rs), curl_error_buf);
	}else{
		randomSleep(1, 3); //[min, max]
		if(_stocks[_cur_stock_code].getHistoryAttr().rank > 0){
			simple_log.info("Get Stock Rank", _cur_stock_code, true, _cur_stock_code + " : " + std::to_string(_stocks[_cur_stock_code].getHistoryAttr().rank));
		}else{
			simple_log.warn("Get Stock Rank", _cur_stock_code, _cur_stock_code + " : " + std::to_string(_stocks[_cur_stock_code].getHistoryAttr().rank), curl_in);
		}
	}
	return *this;	
}

int stockpicker::SpiderStocksOverview::getAllStocks(std::string spec_stock, std::string spec_date){
	/* TODO 这显然不是一个长期的接口 */
	#define _FIELDS "fields="
	#define _BASE_URL "http://45.push2.eastmoney.com/api/qt/clist/get?cb=jQuery112405393508833921838_1666529170574"
	#define _PARAMS "&pn=1&pz=50000&po=1&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&wbp2u=|0|0|0|web&fid=f3&fs=m:0+t:6,m:0+t:80,m:1+t:2,m:1+t:23,m:0+t:81+s:2048&"
	
	_cur_stock_code = spec_stock;

	//重置静态变量
	curl_in.clear();
	_parseStockBasicInfo(nullptr, 0, 0, nullptr);

	//获得基本信息
	int rs = getWebContent( _BASE_URL _PARAMS _FIELDS 
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
		,_parseStockBasicInfo, this);

	if(rs){
		simple_log.error("Get All Stocks Basic Info", "SpiderStocksOverview", std::to_string(rs), curl_error_buf);
	}else{
		if(_stocks.size() > 0){
			simple_log.console.info("Get ALL Stocks Basic Info", "SpiderStocksOverview", true, "SIZE: " + std::to_string(_stocks.size()) );
		}else{
			simple_log.console.warn("Get ALL Stocks Basic Info", "SpiderStocksOverview", "SIZE: " + std::to_string(_stocks.size()), curl_in);
		}

		//获得其它信息
		for(auto iter = _stocks.rbegin(); iter != _stocks.rend(); iter++){
			_cur_stock_code = iter->first;

			_getRank()._getSectors()._getTransactions();
			if(!spec_date.empty()){
				_stocks[_cur_stock_code].getHistoryAttr().setDate(spec_date);
			}

			_updateStockInfo(_stocks[_cur_stock_code]);
			_storageStockTransactions(_transactions, _stocks[_cur_stock_code].getHistoryAttr().date, _cur_stock_code);

			_transactions.clear();
		}
		auto rt = simple_log.recordInfo(true);
		simple_log.console.info("Get ALL Stocks Logs Overview", "warn: " + std::to_string(std::get<1>(rt)) + ", error: " + std::to_string(std::get<2>(rt)) + ", fatal: " + std::to_string(std::get<3>(rt)), true, "");
	}

	return rs;	
}

void stockpicker::SpiderStocksOverview::_addStock(std::map<std::string, std::string> &stock_attr){
	//针对指定的stock的情况:如果当前stock_code不为空，则意味着我仅需要特定的stock
	if((!_cur_stock_code.empty() && stock_attr[_STOCK_CODE_KEY] != _cur_stock_code) || stock_attr[_STOCK_CODE_KEY].empty()) return;

	//std::cout<<stock_attr[_STOCK_NAME_KEY]<<", "<<stock_attr[_STOCK_MARKET_VALUE_KEY]<<", "<<stock_attr[_STOCK_MARKET_VALUE_KEY].compare("-")<<std::endl;

	//退市股
	if(!stock_attr[_STOCK_MARKET_VALUE_KEY].compare("-") || !stock_attr[_STOCK_TRADED_MARKET_VALUE_KEY].compare("-")) return;

	//停牌股
	if(!stock_attr[_STOCK_START_PRICE_KEY].compare("-")) return;

	_stocks[stock_attr[_STOCK_CODE_KEY]] = Stock(std::stoi(stock_attr[_STOCK_CODE_KEY])).setName(stock_attr[_STOCK_NAME_KEY])
									.setPe(float_str_to_int(stock_attr[_STOCK_PE_KEY]))
									.setMarketValue(std::stol(stock_attr[_STOCK_MARKET_VALUE_KEY]))
									.setTradedMarketValue(std::stol(stock_attr[_STOCK_TRADED_MARKET_VALUE_KEY]))
								;
	_stocks[stock_attr[_STOCK_CODE_KEY]].getHistoryAttr().setPriceStart(float_str_to_int(stock_attr[_STOCK_START_PRICE_KEY]))
										.setPriceNewst(float_str_to_int(stock_attr[_STOCK_NEWST_PRICE_KEY]))
										.setPriceHigest(float_str_to_int(stock_attr[_STOCK_HIGEST_PRICE_KEY]))
										.setPriceLowest(float_str_to_int(stock_attr[_STOCK_LOWST_PRICE_KEY]))
										.setAmplitude(float_str_to_int(stock_attr[_STOCK_AMPLITUDE_KEY]))
										.setTurnoverRate(float_str_to_int(stock_attr[_STOCK_TURNOVER_RATE_KEY]))
										.setTurnoverSum(float_str_to_long(stock_attr[_STOCK_TURNOVER_KEY]))
										.setChangePercent(float_str_to_int(stock_attr[_STOCK_CHANGEPERCENT_KEY]))
										.setDate(date)
								;
}

void stockpicker::SpiderStocksOverview::_addRank(std::string rank){
	_stocks[_cur_stock_code].getHistoryAttr().setRank(std::stoi(rank));
}

void stockpicker::SpiderStocksOverview::_addSector(std::string& sector){
	sector.erase(0, sector.find_first_not_of('\"'));
	sector.erase(sector.find_last_not_of('\"') + 1);
	_stocks[_cur_stock_code].addSector(sector);
}

void stockpicker::SpiderStocksOverview::_addTransaction(std::string transactions){
	this->_transactions = transactions;
	_stocks[_cur_stock_code].getHistoryAttr().setTransactions(&this->_transactions);
}

int stockpicker::SpiderStocksOverview::_updateStockSectors(const Stock& stock){
	int rt = 0;

	std::string sql = "DELETE FROM " + _mysqltool->Table_stock_sector_related + " WHERE stock_code = " + std::to_string(stock.code);
	if( rt = _mysqltool->exec(sql) ) return rt;

	for(auto &sector:stock.sectors){
		auto sector_id = _mysqltool->_getSectorId(sector);
		if(!sector_id) {
			simple_log.error("Get Sector ID", sector, std::to_string(sector_id), "");
			continue;
		}
		sql = "INSERT INTO " + _mysqltool->Table_stock_sector_related + "(stock_code, sector_id) VALUES(" + std::to_string(stock.code) + ", " + std::to_string(sector_id) + ")";
		rt  |= _mysqltool->exec(sql);
	}

	if(rt)
		simple_log.error("Update Stock Sectors", std::to_string(stock.code), std::to_string(rt), "");	

	return rt;
}

int stockpicker::SpiderStocksOverview::_updateStockHistory(const Stock& stock){
	const StockHistoryOverview &history = stock.history_attr;

	std::string sql = "REPLACE INTO " + _mysqltool->Table_stocks_history + "( stock_code, created_date, rank, price_start, price_newst, price_higest, price_lowest, turnover_rate, turnover_sum, amplitude, change_percent )  VALUES( " + 
							std::to_string(stock.code) + ", '" + history.date +"', " + std::to_string(history.rank) + ", " +
							std::to_string(history.price_start) + ", " + std::to_string(history.price_newst) + ", " + 
							std::to_string(history.price_higest) + ", " + std::to_string(history.price_lowest) + ", " + 
							std::to_string(history.turnover_rate) + ", " + std::to_string(history.turnover_sum) + ", " +
							std::to_string(history.amplitude) + ", " + std::to_string(history.change_percent) + 
						")"
					;

	auto rt = _mysqltool->exec(sql);
	if(rt)
		simple_log.error("Update Stock History", std::to_string(stock.code), std::to_string(rt), "");	

	return rt;
}

int stockpicker::SpiderStocksOverview::_updateStockInfo(const Stock& stock){
	int rt = 0;

	std::string sql = "SELECT stock_code FROM " + _mysqltool->Table_stocks_info + " WHERE stock_code = " + std::to_string(stock.code);  
		
	if(_mysqltool->dataAlreadyExists(sql)){
		sql = "UPDATE " + _mysqltool->Table_stocks_info + " SET stock_name = '" + stock.name +
												"', pe = " + std::to_string(stock.pe) +
												",  market_value= " + std::to_string(stock.market_value) +
												",  traded_market_value= " + std::to_string(stock.traded_market_value) +
												" WHERE stock_code = " + std::to_string(stock.code)
											;
	}else{
		sql = "INSERT INTO " + _mysqltool->Table_stocks_info + "(stock_code, stock_name, pe, market_value, traded_market_value) VALUES(" +	
							std::to_string(stock.code) + ", '" + stock.name + "', " + std::to_string(stock.pe) + 
							", " + std::to_string(stock.market_value) + 
							", " + std::to_string(stock.traded_market_value) + 
						")"
					;
	}

	rt |= _mysqltool->exec(sql);
	if(rt)
		simple_log.error("Update Stock Info", std::to_string(stock.code), std::to_string(rt), "");	

	rt |= _updateStockHistory(stock);

	if(stock.sectors.size()){
		rt |= _updateStockSectors(stock);
	}

	return rt;	
}

int stockpicker::SpiderStocksOverview::_storageStockTransactions(const std::string& tas, const std::string& date, const std::string stock_code){
	std::string path = _filetool->_base_dir + "/" + date;

	if( _filetool->_checkAndCreateDir(path) ){
		if( _filetool->_writeToFile(path + "/" + stock_code, tas)){
			return 0;	
		}
		simple_log.error("Write To File", stock_code, "Failed", "");
	}
	simple_log.error("Create Dir", stock_code, "Failed", "");
	return -1;
}
