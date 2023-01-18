#ifndef _STOCK_PICKER_STRATEGIESLOADER_H
#define _STOCK_PICKER_STRATEGIESLOADER_H

#include<iostream>
#include<vector>

#include"../public/stock.hpp"
#include"../public/mysqltool.hpp"
#include"../public/filetool.hpp"
#include"../public/log.hpp"

namespace stockpicker{

	class StrategiesLoader{
	private:
		MySQLTool *_mysqltool;
		FileTool *_filetool;

	public:
		StrategiesLoader(MySQLTool *mysqltool, FileTool *filetool):_mysqltool(mysqltool), _filetool(filetool){}
	};

	//过滤股票
	class StrategyFilter{
	public:
		virtual bool checkStock(int stock_code, std::vector<std::string> params = std::vector<std::string>()) = 0;

		std::vector<int> operator()(std::vector<int> stockCodes, std::vector<std::string> params = std::vector<std::string>()){
			std::vector<int> remainStockCodes;
		
			for(auto curStock = stockCodes.begin(), endStock = stockCodes.end();curStock != endStock;curStock++){
				if(this->checkStock(*curStock, params)){
					remainStockCodes.push_back(*curStock);
				}
			}

			return remainStockCodes;
		}
	};


	// 具体策略 

	class StrategyFilterByMarketValueRange: public StrategyFilter{
	private:
		MySQLTool *_mysqltool;
		FileTool *_filetool;

	public:
		StrategyFilterByMarketValueRange(MySQLTool *mysqltool, FileTool *filetool):_mysqltool(mysqltool), _filetool(filetool){}
		
		//params: 0: minimum;1:maximum
		bool checkStock(int stock_code, std::vector<std::string> params = std::vector<std::string>()) override{
			if(params.size() != 2) return false;

			std::string sql = "SELECT market_value FROM `" + _mysqltool->Table_stocks_info + "` WHERE stock_code = " + std::to_string(stock_code) + " AND market_value BETWEEN " + params[0] + " AND " + params[1];
			auto results = _mysqltool->query(sql);

			if(results.size())	return true;
			return false;
		}
	};
		
}

#endif
