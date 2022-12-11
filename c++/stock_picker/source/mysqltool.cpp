#include"../include/mysqltool.hpp"

namespace stockpicker{
	void MySQLTool::_error(const std::string &sql){
		simple_log.error("Execute SQL", sql, "Failed", mysql_error(&_mysql));	
	}

	int MySQLTool::_exec(const std::string &sql){
		int rt = 0;
		if(rt = mysql_query(&_mysql, sql.c_str())){ 
			_error(sql);
			return rt;
		}
		return 0;
	}

	MYSQL_RES* MySQLTool::_query(const std::string &sql){
		_exec(sql);

		auto *result = mysql_store_result(&_mysql);
		if(!result){ 
			_error(sql);
			return NULL;
		}
		return result;
	}

	std::vector<std::vector<std::string>> MySQLTool::query(const std::string& sql){
		std::vector<std::vector<std::string>> results;

		auto query_result = _query(sql);
		
		auto num_fields = mysql_num_fields(query_result);

		MYSQL_ROW row;
		while((row = mysql_fetch_row(query_result))){
			unsigned long *lengths;
			lengths = mysql_fetch_lengths(query_result);

			std::vector<std::string> row_fields;
			for(unsigned int i = 0; i < num_fields; i++){
				row_fields.push_back(std::string(row[i]));
			}
			results.push_back(row_fields);
		}	

		mysql_free_result(query_result);
		return results; 	
	}

	bool MySQLTool::dataAlreadyExists(const std::string &sql){
		MYSQL_RES *result;
		if(!(result = _query(sql))) return false;

		auto rt = mysql_num_rows(result);
		mysql_free_result(result);

		return rt > 0;
	}

	int MySQLTool::updateStockSectors(Stock& stock){
		int rt = 0;

		std::string sql = "DELETE FROM " + Table_stock_sector_related + " WHERE stock_code = " + std::to_string(stock.code);
		if( rt = _exec(sql) ) return rt;

		for(auto &sector:stock.sectors){
			auto sector_id = _getSectorId(sector);
			if(!sector_id) {
				simple_log.error("Get Sector ID", sector, std::to_string(sector_id), "");
				continue;
			}
			sql = "INSERT INTO " + Table_stock_sector_related + "(stock_code, sector_id) VALUES(" + std::to_string(stock.code) + ", " + std::to_string(sector_id) + ")";
			rt  |= _exec(sql);
		}

		if(rt)
			simple_log.error("Update Stock Sectors", std::to_string(stock.code), std::to_string(rt), "");	

		return rt;
	}

	int MySQLTool::updateStockHistory(Stock& stock){
		StockHistoryOverview &history = stock.history_attr;

		std::string sql = "REPLACE INTO stocks_history( stock_code, created_date, rank, price_start, price_newst, price_higest, price_lowest, turnover_rate, turnover_sum, amplitude, change_percent )  VALUES( " + 
								std::to_string(stock.code) + ", '" + history.date +"', " + std::to_string(history.rank) + ", " +
								std::to_string(history.price_start) + ", " + std::to_string(history.price_newst) + ", " + 
								std::to_string(history.price_higest) + ", " + std::to_string(history.price_lowest) + ", " + 
								std::to_string(history.turnover_rate) + ", " + std::to_string(history.turnover_sum) + ", " +
								std::to_string(history.amplitude) + ", " + std::to_string(history.change_percent) + 
							")"
						;

		auto rt = _exec(sql);
		if(rt)
			simple_log.error("Update Stock History", std::to_string(stock.code), std::to_string(rt), "");	

		return rt;
	}

	int MySQLTool::updateStockInfo(Stock& stock){
		int rt = 0;

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

		rt |= _exec(sql);
		if(rt)
			simple_log.error("Update Stock Info", std::to_string(stock.code), std::to_string(rt), "");	

		rt |= updateStockHistory(stock);
		rt |= updateStockSectors(stock);

		return rt;	
	}

}
