//doc: https://github.com/yhirose/cpp-httplib

#include<iostream>
#include<string>

#include "./third-parts/yhirose/cpp-httplib/httplib.h"
#include "./include/public/mysqltool.hpp"

class View{
private:
	stockpicker::MySQLTool *_mysqltool;


public:
	View(stockpicker::MySQLTool *mysqltool):_mysqltool(mysqltool){}

	void getStockBasicInfo(std::string stock) const{
		
	}

	void getStockHistory(std::string stock, std::string date) const{
		std::string sql = "SELECT * FROM " + _mysqltool->Table_stocks_history + " WHERE stock_code = " + stock + " AND created_date = '" + date + "'";
		auto result = _mysqltool->query(sql);
		std::cout<<result[0][1]<<std::endl;
	}

	void getStockTransactionDetails(std::string stock, std::string date) const{
		
	}

};

class Router{};

class Job{};

int main(){
	// HTTP
	httplib::Server svr;

	stockpicker::MySQLTool mysqltool =  stockpicker::MySQLTool("60.205.210.253",3306,"stock_picker","happenbigtreasure");
	View view = View(&mysqltool);

	svr.Get("/view/stock_history/single", [view](const httplib::Request &req, httplib::Response &res) {
		std::string stock = "";
		std::string date = "";
		
		if (req.has_param("stock")) {
			stock = req.get_param_value("stock");
		}

		if (req.has_param("date")) {
			date = req.get_param_value("date");
		}
		if(stock != "" && date != ""){
			view.getStockHistory(stock, date);		
		}
	});

	svr.listen("0.0.0.0", 8080);

	return 0;
}
