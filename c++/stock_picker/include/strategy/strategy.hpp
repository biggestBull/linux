#ifndef _STOCK_PICKER_STRATEGIESLOADER_H
#define _STOCK_PICKER_STRATEGIESLOADER_H

#include<iostream>
#include<vector>
#include<cstdlib>

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

	//市值
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
		
	//最近多少天的股价趋势
	class StrategyFilterByPriceTrendRecently: public StrategyFilter{
	private:
		MySQLTool *_mysqltool;
		FileTool *_filetool;

	public:
		StrategyFilterByPriceTrendRecently(MySQLTool *mysqltool, FileTool *filetool):_mysqltool(mysqltool), _filetool(filetool){}
		
		//params: 0: recently x days;1: minimum difference, can + -，need * 100
		bool checkStock(int stock_code, std::vector<std::string> params = std::vector<std::string>()) override{
			if(params.size() != 2) return false;

			std::string sql = "SELECT SUM(turnover_rate) as turnover_rate_total from ( " 
									"SELECT turnover_rate FROM `" + _mysqltool->Table_stocks_history + "` WHERE stock_code = " + 
										std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT " + params[0] + 
								") _ ";

			auto results = _mysqltool->query(sql);

			if(not results.size())	return false;
		
			auto a = std::atoi(params[1].c_str());
			auto b = std::atoi(results[0][0].c_str());

			//必须同号，且b大于等于a
			return (a ^ b) >=  0 && std::abs(a) <= std::abs(b);
		}
	};
}

#endif
