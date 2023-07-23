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

			std::string sql = "SELECT traded_market_value FROM `" + mysqltool->Table_stocks_info + "` WHERE stock_code = " + std::to_string(stock_code) + " AND traded_market_value BETWEEN " + params[0] + " AND " + params[1];
			auto results = mysqltool->query(sql);

			if(results.size())	return std::atoll(results[0][0].c_str());
			return impossible_rank_value;
		}
	};
		
	//最近多少天的股价趋势
	class StrategyFilterByPriceTrendRecently: public StrategyFilter{
	public:
		StrategyFilterByPriceTrendRecently(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: 0:before x days;1: selected x days;2: minimum difference, can + -，need * 100
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() < 3) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT SUM(change_percent) as change_percent_total from ( " 
									"SELECT change_percent FROM `" + mysqltool->Table_stocks_history + "` WHERE stock_code = " + 
										std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT " + params[0] + ", " + params[1] + 
								") _ ";

			auto results = mysqltool->query(sql);

			if(not results.size())	return impossible_rank_value;
		
			auto a = std::atoi(params[2].c_str());
			auto b = std::atoi(results[0][0].c_str());

			//必须同号，且b大于等于a
			if( (a ^ b) >= 0 && std::abs(a) <= std::abs(b) ) return static_cast<long long>( std::abs(b) - std::abs(a) );
			return impossible_rank_value;
		}
	};

	//最近多少天的换手率
	class StrategyFilterByTurnoverRateRecently: public StrategyFilter{
	public:
		StrategyFilterByTurnoverRateRecently(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: 0:before x days;1: selected x days;2: minimum difference,need * 100
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() < 3) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT SUM(turnover_rate) as turnover_rate_total from ( " 
									"SELECT turnover_rate FROM `" + mysqltool->Table_stocks_history + "` WHERE stock_code = " + 
										std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT " + params[0] + ", " + params[1] + 
								") _ ";

			auto results = mysqltool->query(sql);

			if(not results.size())	return impossible_rank_value;
		
			auto a = std::atoi(params[2].c_str());
			auto b = std::atoi(results[0][0].c_str());

			//必须同号，且b大于等于a
			if( (a ^ b) >= 0 && std::abs(a) <= std::abs(b) ) return static_cast<long long>( std::abs(b) - std::abs(a) );
			return impossible_rank_value;
		}
	};

	//最近多少天的排名上升
	class StrategyFilterByRankRecently: public StrategyFilter{
	public:
		StrategyFilterByRankRecently(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: 0: selected x days, minimum:2;1: end(day) exceed start(day) 
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() < 2) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT rank FROM `" + mysqltool->Table_stocks_history + "` WHERE stock_code = " + 
										std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT " + params[0];

			auto results = mysqltool->query(sql);

			if(not results.size() or results.size() < 2) return impossible_rank_value;
		
			auto a = std::atoi(results[results.size() - 1][0].c_str());
			auto b = std::atoi(results[0][0].c_str());

			if( a - b >= std::atoi( params[1].c_str() ) ) return static_cast<long long>( a - b );
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

	//价格过滤
	class StrategyFilterByPrice: public StrategyFilter{
	public:
		StrategyFilterByPrice(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: price_left, price_right
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() != 2) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT price_newst FROM `" + mysqltool->Table_stocks_history + "` WHERE stock_code = " + std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT 1 ";
			auto results = mysqltool->query(sql);

			if(results.size() && std::atoll(results[0][0].c_str())  >= std::atoll(params[0].c_str()) && std::atoll(results[0][0].c_str())  <= std::atoll(params[1].c_str())){	
				return std::atoll(results[0][0].c_str());
			}	
			return impossible_rank_value;
		}
	};

	//技术形态过滤 -----------------------------------------------------------------------------
	//十字星-吊锤线-纺锤线-启明星等
	class StrategyFilterByShapeOfDoji: public StrategyFilter{
	public:
		StrategyFilterByShapeOfDoji(MySQLTool *mysqltool, FileTool *filetool):StrategyFilter(mysqltool, filetool){}
		
		//params: 
		//0：收盘与开盘之差的范围（千分比，可为负数, 如要求百分之一，则传入10, 要求百分之零点五，则传入5）left，1: &0-right，2：纳入统计的天数，3：偏移天数（往前便宜多少天），
		//4；上影线长度（千分比, 不能为负数）left，5：&4-right，6：下影线长度（千分比，不能为负数）left，7：&6-right，8：满足该形态的个数要求
		long long checkStock(int stock_code, std::vector<std::string>& params) override{
			if(params.size() < 7) return impossible_rank_value;

			printf("\r           \rcurrent stock: %d", stock_code);

			std::string sql = "SELECT price_start, price_newst, price_higest, price_lowest FROM `" + mysqltool->Table_stocks_history + "` WHERE stock_code = " + std::to_string(stock_code) + " ORDER BY created_date DESC LIMIT " + params[3] + ", " + params[2];
			auto results = mysqltool->query(sql);

			long long cnt = 0;
			for(int i = 0;i < results.size(); i++){
				int price_start = std::atoi(results[i][0].c_str());
				int price_newst = std::atoi(results[i][1].c_str());
				int price_higest = std::atoi(results[i][2].c_str());
				int price_lowest = std::atoi(results[i][3].c_str());

				if(
					(price_newst - price_start) * 1000 / price_start >= std::atoi(params[0].c_str()) &&
					(price_newst - price_start) * 1000 / price_start <= std::atoi(params[1].c_str()) &&
					(price_higest - price_newst) * 1000 / price_start >= std::atoi(params[4].c_str()) &&	
					(price_higest - price_newst) * 1000 / price_start <= std::atoi(params[5].c_str()) &&	
					(price_newst - price_lowest) * 1000 / price_start >= std::atoi(params[6].c_str()) &&	
					(price_newst - price_lowest) * 1000 / price_start <= std::atoi(params[7].c_str())	
				){
					cnt++;
				}
			}

			return cnt >= std::atoll(params[8].c_str()) ? cnt : impossible_rank_value;
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
			_strategies.insert({"MarketValueRange", new StrategyFilterByMarketValueRange(mysqltool, filetool)}); 	
			_strategies.insert({"PriceTrendRecently", new StrategyFilterByPriceTrendRecently(mysqltool, filetool)}); 	
			_strategies.insert({"TurnoverRate", new StrategyFilterByTurnoverRateRecently(mysqltool, filetool)}); 	
			_strategies.insert({"Rank", new StrategyFilterByRankRecently(mysqltool, filetool)}); 	
			_strategies.insert({"Sectors", new StrategyFilterBySectors(mysqltool, filetool)}); 	
			_strategies.insert({"Price", new StrategyFilterByPrice(mysqltool, filetool)}); 	
			_strategies.insert({"ShapeOfDoji", new StrategyFilterByShapeOfDoji(mysqltool, filetool)}); 	
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

				 std::cout<<"\r                                         \r"<<strategy<<" : "<<_stock_codes.size()<<std::endl<<std::endl;
			 }else{
				std::cout << "无效的策略名!" << std::endl;
			 }

			return *this;
		};

		const std::vector<int>& get_results(){
			return _stock_codes;
		} 

	};

}

#endif
