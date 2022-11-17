
#ifndef _STOCK_PICKER_MYSQLTOOL_H

#define _STOCK_PICKER_MYSQLTOOL_H

#define _MYSQL_TOOL_ERR_BUF 1000

#include<mysql/mysql.h>
#include<cstdlib>

#include"stock.hpp"
#include"log.hpp"

namespace stockpicker{

	class MySQLTool{
	private:
		MYSQL _mysql;
		char _err_buf[_MYSQL_TOOL_ERR_BUF];

		SimpleLog& _simpleLog = SimpleLog::getInstance();

		#include"mysql.inc"

		int _checkTables(){
			int status = 0;
			uint tables_num = sizeof(_tables)/sizeof(*_tables);
			for(int i = 0;i != tables_num;i++){
				if((status = mysql_query(&_mysql,_tables[i]))){
					_simpleLog.error("Create Table", std::to_string(i), "Failed", std::to_string(status));
					exit(1);	
				}
			}
			return 0;	
		}
	
	public:
		MySQLTool(const char *url,int port,const char *user,const char *pwd){
			mysql_init(&_mysql);
			if(!mysql_real_connect(&_mysql,url,user,pwd,_db_name,port,0,0)){
				snprintf(_err_buf, _MYSQL_TOOL_ERR_BUF, "%s", mysql_error(&_mysql));
				_simpleLog.error("Connect Mysql", std::string(url) + ":" + std::to_string(port), "Failed", _err_buf);
				exit(1);	
			}
	
			int status = 0;
			if((status = mysql_select_db(&_mysql,_db_name)) != 0){
				_simpleLog.error("Selecy Database", std::string(_db_name), "Failed", std::to_string(status));
				exit(status);	
			}
			
			// XXX 建表，stock_picker的一生只需要执行一次, 并且需要权限
			//_checkTables();
		}
	
		~MySQLTool(){
			mysql_close(&_mysql);
		}

		int updateStockInfo(Stock&);
	};
}

#endif

