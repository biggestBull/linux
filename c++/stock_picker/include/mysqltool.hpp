
#ifndef MYSQLTOOL_H

#define MYSQLTOOL_H

#include<stdio.h>
#include<mysql/mysql.h>

#include"stock.hpp"

namespace stockpicker{

	class MySQLTool{
	private:
		MYSQL _mysql;

		#include"mysql.inc"

		int _checkTables(){
			int status = 0;
			uint tables_num = sizeof(_tables)/sizeof(*_tables);
			for(int i = 0;i != tables_num;i++){
				if((status = mysql_query(&_mysql,_tables[i]))){
					printf("create table failed: [ %d ]!", i );	
					exit(status);	
				}
			}
			return 0;	
		}
	
	public:
		MySQLTool(const char *url,int port,const char *user,const char *pwd){
			mysql_init(&_mysql);
			if(!mysql_real_connect(&_mysql,url,user,pwd,_db_name,port,0,0)){
				printf("connect mysql failed\n");	
				exit(1);	
			}
	
			int status = 0;
			if((status = mysql_select_db(&_mysql,_db_name)) != 0){
				printf("select db failed!\n");	
				exit(status);	
			}
			
			// XXX 建表，stock_picker的一生只需要执行一次, 并且需要权限
			//_checkTables();
		}
	
		~MySQLTool(){
			mysql_close(&_mysql);
		}

		int insertStockInfo(Stock &stock){
			printf("%d", stock.history_attr.rank);
			return 0;
		}
	};
}

#endif

