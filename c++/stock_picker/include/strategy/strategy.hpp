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

	class Strategy{
	public:
		virtual bool checkStock(int stock_code) = 0;

		std::vector<int> operator()(std::vector<int> stockCodes){
			std::vector<int> remainStockCodes;
		
			for(auto curStock = stockCodes.begin(), endStock = stockCodes.end();curStock != endStock;curStock++){
				if(this->checkStock(*curStock)){
					remainStockCodes.push_back(*curStock);
				}
			}

			return remainStockCodes;
		}
	};

	class StrategyClassic: public Strategy{
	private:
		MySQLTool *_mysqltool;
		FileTool *_filetool;

	public:
		StrategyClassic(MySQLTool *mysqltool, FileTool *filetool):_mysqltool(mysqltool), _filetool(filetool){}
		
		bool checkStock(int stock_code) override{
			std::cout<<stock_code<<std::endl;
			return false;
		}
	};
		
}

#endif
