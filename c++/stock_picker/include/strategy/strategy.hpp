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

	//股票过滤策略基类 ----------------------------------------------------------------------------
	class StrategyFilter{
	protected:
		long long impossible_rank_value = 1L << (sizeof(long long) * 8 - 1);

		MySQLTool *mysqltool;
		FileTool *filetool;

	public:
		StrategyFilter(MySQLTool *mysqltool, FileTool *filetool):mysqltool(mysqltool), filetool(filetool){}

		virtual long long checkStock(int stock_code, std::vector<std::string>& params) = 0;

		std::vector<int> operator()(std::vector<int> stock_codes, std::vector<std::string>& params){
			std::vector<int> remain_stock_codes;

			for(auto cur_stock = stock_codes.begin(), end_stock = stock_codes.end();cur_stock != end_stock;cur_stock++){
				auto rank_value = this->checkStock(*cur_stock, params);
				if(rank_value != impossible_rank_value){
					remain_stock_codes.push_back(*cur_stock);
				}
			}

			return remain_stock_codes;
		}
	};


	// 具体策略 ------------------------------------------------------------------------------- 

	//市值
	class StrategyFilterByMarketValueRange: public StrategyFilter{
	public:
		StrategyFilterByMarketValueRange(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: 0: minimum;1:maximum
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() != 2) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT market_value FROM `" + mysqltool->Table_stocks_info + "` WHERE stock_code = " + std::to_string(stock_code) + " AND market_value BETWEEN " + params[0] + " AND " + params[1];
			auto results = mysqltool->query(sql);

			if(results.size())	return std::atoll(results[0][0].c_str());
			return impossible_rank_value;
		}
	};
		
	//最近多少天的股价趋势
	class StrategyFilterByPriceTrendRecently: public StrategyFilter{
	public:
		StrategyFilterByPriceTrendRecently(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: 0: recently x days;1: minimum difference, can + -，need * 100
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() != 2) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT SUM(change_percent) as change_percent_total from ( " 
									"SELECT change_percent FROM `" + mysqltool->Table_stocks_history + "` WHERE stock_code = " + 
										std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT " + params[0] + 
								") _ ";

			auto results = mysqltool->query(sql);

			if(not results.size())	return impossible_rank_value;
		
			auto a = std::atoi(params[1].c_str());
			auto b = std::atoi(results[0][0].c_str());

			//必须同号，且b大于等于a
			if( (a ^ b) >= 0 && std::abs(a) <= std::abs(b) ) return static_cast<long long>( std::abs(b) - std::abs(a) );
			return impossible_rank_value;
		}
	};

	//板块过滤
	class StrategyFilterBySectors: public StrategyFilter{
	public:
		StrategyFilterBySectors(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: sector_1[, sector_2, ...]
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(!params.size()) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT COUNT(A.id) as cnt from `sectors` A join `stock_sector_related` B ON A.id=B.sector_id AND B.stock_code = " + 
									std::to_string(stock_code) + " AND A.sector_name in ( "; 
			for(uint i = 0;i != params.size();i++){
				sql += "'" + params[i]  + "'";
				sql += ", '\"" + params[i]  + "\"'";

				sql += i < params.size() - 1 ? ", " : "";
			}

			sql += ")";

			auto results = mysqltool->query(sql);

			if(not results.size())	return impossible_rank_value;
		
			auto cnt = std::atoi(results[0][0].c_str());
			return cnt ? cnt : impossible_rank_value;
		}
	};

	//策略加载器 -------------------------------------------------------------------------------
	class StrategiesLoader{
	private:
		MySQLTool *mysqltool;
		FileTool *filetool;

		std::map<std::string, StrategyFilter*> _strategies;

		std::vector<int> _stock_codes;

		void _constructStrategies(){
			_strategies.insert({"MarketVauleRange", new StrategyFilterByMarketValueRange(mysqltool, filetool)}); 	
			_strategies.insert({"PriceTrendRecently", new StrategyFilterByPriceTrendRecently(mysqltool, filetool)}); 	
			_strategies.insert({"Sectors", new StrategyFilterBySectors(mysqltool, filetool)}); 	
		}


	public:
		StrategiesLoader(MySQLTool *mysqltool, FileTool *filetool, std::vector<int> stock_codes = std::vector<int>()):mysqltool(mysqltool), filetool(filetool), _stock_codes(stock_codes){
			if(!_stock_codes.size()){
				std::string sql = "SELECT stock_code FROM " + mysqltool->Table_stocks_info;
        		auto result = mysqltool->query(sql);
				for(auto& row : result){
					_stock_codes.push_back(std::stoi(row[0]));
				}
			}
			
			_constructStrategies();
		}

		 StrategiesLoader& logicAnd(std::string strategy, std::vector<std::string> params){
			 if(_strategies.find(strategy)  != _strategies.end()){
				 _stock_codes = (*_strategies[strategy])(_stock_codes, params);

				 std::cout<<"\r               \r"<<strategy<<" : "<<_stock_codes.size()<<std::endl<<std::endl;
			 }

			return *this;
		};

		const std::vector<int>& get_results(){
			return _stock_codes;
		} 

	};

}

#endif
