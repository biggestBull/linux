#ifndef _STOCK_PICKER_STOCK_VIEWER_H

#define _STOCK_PICKER_STOCK_VIEWER_H

#include<vector>
#include<string>

namespace stockpicker{

	class StockViewer{
	private:
		MySQLTool *_mysqltool;
		FileTool *_filetool;


	public:
		StockViewer(MySQLTool *mysqltool, FileTool *filetool) : _mysqltool(mysqltool), _filetool(filetool){}

		std::vector<std::string> getStockBasicInfo(int stock) const{
			std::string sql = "SELECT A.stock_code, A.stock_name, A.traded_market_value, B.price_newst FROM " + _mysqltool->Table_stocks_info 
				+ " A JOIN " + _mysqltool->Table_stocks_history 
				+ " B ON A.stock_code  =  B. stock_code AND A.stock_code = " + std::to_string(stock) + " ORDER BY B.created_date DESC LIMIT 1";
			auto result = _mysqltool->query(sql);

			std::vector<std::string> rt;
			if(sizeof(result)){
				rt.emplace_back(std::string(result[0][0]));
				rt.emplace_back(std::string(result[0][1]));

				std::string traded_market_value(result[0][2]);
				traded_market_value.insert(traded_market_value.size() > 8 ? traded_market_value.size() - 8 : 0, traded_market_value.size() > 2 ? "." : "0.");
				rt.emplace_back(traded_market_value);

				std::string price(result[0][3]);
				price.insert(price.size() > 2 ? price.size() - 2 : 0, price.size() > 2 ? "." : "0.");
				rt.emplace_back(price);
			}
			return rt;
		}

		void getStockHistory(std::string stock, std::string date) const{
		}

		void getStockTransactionDetails(std::string stock, std::string date) const{
		}

	};

}

#endif

