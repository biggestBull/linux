
#ifndef _STOCK_PICKER_MYSQLTOOL_H

#define _STOCK_PICKER_MYSQLTOOL_H

#define _MYSQL_TOOL_ERR_BUF 1000

#include<mysql/mysql.h>
#include<stdio.h>

#include<map>
#include<vector>
#include<string>

#include"stock.hpp"
#include"log.hpp"

namespace stockpicker{

	class MySQLTool{
		friend class SpiderStocksOverview;

	private:
		void _error(const std::string&);
		int _exec(const std::string&);
		MYSQL_RES* _query(const std::string&);

	protected:
		SimpleLog& simple_log = SimpleLog::getInstance();

		std::string Table_stocks_info = "stocks_info";
		std::string Table_sectors = "sectors";
		std::string Table_stock_sector_related = "stock_sector_related";
		std::string Table_stocks_history = "stocks_history";

		std::vector<std::vector<std::string>> query(const std::string&);
		bool dataAlreadyExists(const std::string&);

	private:
		MYSQL _mysql;
		char _err_buf[_MYSQL_TOOL_ERR_BUF];

		std::map<std::string, unsigned int> _all_sectors;

		#include"mysql.inc"

		int _prepareTable(){
			int status = 0;
			uint tables_num = sizeof(_tables)/sizeof(*_tables);
			for(int i = 0;i != tables_num;i++){
				if((status = mysql_query(&_mysql,_tables[i]))){
					simple_log.error("Create Table", std::to_string(i), "Failed", std::to_string(status));
					exit(1);	
				}
			}
			return 0;	
		}

		unsigned int _getAllSectors(){
			std::string sql = "SELECT * FROM " + Table_sectors;

			auto results = query(sql);
			for(auto &row:results){
				_all_sectors[row[1]] = std::stoi(row[0]);
			}
			return _all_sectors.size();
		}

		unsigned int _getSectorId(const std::string &sector){
			if(_all_sectors.find(sector) != _all_sectors.end()){
				return _all_sectors[sector];
			}

			std::string sql = "INSERT INTO " + Table_sectors + "(sector_name) VALUE('" + sector + "')";
			if(_exec(sql)) return 0;
			return static_cast<unsigned int>(mysql_insert_id(&_mysql));
		}

	public:
		MySQLTool(const char *url,int port,const char *user,const char *pwd){
			mysql_init(&_mysql);
			if(!mysql_real_connect(&_mysql,url,user,pwd,_db_name,port,0,0)){
				snprintf(_err_buf, _MYSQL_TOOL_ERR_BUF, "%s", mysql_error(&_mysql));
				simple_log.error("Connect Mysql", std::string(url) + ":" + std::to_string(port), "Failed", _err_buf);
				exit(1);	
			}
	
			int status = 0;
			if((status = mysql_select_db(&_mysql,_db_name)) != 0){
				simple_log.error("Select Database", std::string(_db_name), "Failed", std::to_string(status));
				exit(status);	
			}
			
			std::string sql = "SET NAMES UTF8;";
			if((status = _exec(sql))){
				simple_log.error("Set Names", std::string(_db_name), "Failed", std::to_string(status));
				exit(status);	
			}

			// XXX 建表，stock_picker的一生只需要执行一次, 并且需要权限
			//_prepareTable();

			_getAllSectors();
		}
	
		~MySQLTool(){
			mysql_close(&_mysql);
		}

	};
}

#endif

