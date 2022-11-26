#include"../include/mysqltool.hpp"

namespace stockpicker{
	void MySQLTool::_error(std::string &sql){
		_simpleLog.error("Execute SQL", sql, "Failed", mysql_error(&_mysql));	
	}

	int MySQLTool::_exec(std::string &sql){
		int rt = 0;
		if(rt = mysql_query(&_mysql, sql.c_str())){ 
			_error(sql);
			return rt;
		}
		return 0;
	}

	MYSQL_RES* MySQLTool::_query(std::string &sql){
		_exec(sql);

		auto *result = mysql_store_result(&_mysql);
		if(!result){ 
			_error(sql);
			return NULL;
		}
		return result;
	}

	bool MySQLTool::dataAlreadyExists(std::string &sql){
		MYSQL_RES *result;
		if(!(result = _query(sql))) return false;

		auto rt = mysql_num_rows(result);
		mysql_free_result(result);

		return rt > 0;
	}

	int MySQLTool::updateStockInfo(Stock& stock){
		std::string sql = "SELECT stock_code FROM " + Table_stocks_info + " WHERE stock_code = " + std::to_string(stock.code);  
		
		if(dataAlreadyExists(sql)){
			sql = "UPDATE " + Table_stocks_info + " SET stock_name = '" + stock.name +
													"', pe = " + std::to_string(stock.pe) +
													",  market_value= " + std::to_string(stock.market_value) +
													",  traded_market_value= " + std::to_string(stock.traded_market_value) +
													" WHERE stock_code = " + std::to_string(stock.code)
												;
		}else{
			sql = "INSERT INTO " + Table_stocks_info + "(stock_code, stock_name, pe, market_value, traded_market_value) VALUES(" +	
								std::to_string(stock.code) + ", '" + stock.name + "', " + std::to_string(stock.pe) + 
								", " + std::to_string(stock.market_value) + 
								", " + std::to_string(stock.traded_market_value) + 
							")"
						;
		}

		if(_exec(sql)) return -1;

		return 0;	
	}

}
