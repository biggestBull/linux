
#ifndef MYSQLTOOL_H

#define MYSQLTOOL_H

#include<stdio.h>
#include<mysql/mysql.h>

class MySQLTool{
private:
	const char *user,*pwd,*url;
	int port;
	MYSQL mysql;

	const char *db_name = "stocks";
	const char *tables[4] = {
		R"(CREATE TABLE IF NOT EXISTS `stocks_info`(
            `stock_code` INT UNSIGNED,
            `stock_name` VARCHAR(50) NOT NULL,
            `pe` INT NOT NULL,
            `market_value` BIGINT NOT NULL,
			`traded_market_value` BIGINT NOT NULL, 
            PRIMARY KEY ( `stock_code` )
        )ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)", 
		R"(CREATE TABLE IF NOT EXISTS `sectors`(
            `id` INT UNSIGNED AUTO_INCREMENT,
            `sector_name` VARCHAR(100) NOT NULL,
            PRIMARY KEY ( `id` )
		)ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)", 
		R"(CREATE TABLE IF NOT EXISTS `stock_sector_related`(
            `id` INT UNSIGNED AUTO_INCREMENT,
            `stock_code` INT UNSIGNED NOT NULL,
            `sector_id` INT UNSIGNED NOT NULL,
            PRIMARY KEY ( `id` )
		)ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)", 
		R"(CREATE TABLE IF NOT EXISTS `stocks_history`(
            `stock_code` INT UNSIGNED,
            `created_date` CHAR(12),
            `rank` INT UNSIGNED NOT NULL,
            `price_start` INT UNSIGNED NOT NULL,
            `price_newst` INT UNSIGNED NOT NULL,
            `price_higest` INT UNSIGNED NOT NULL,
            `price_lowest` INT UNSIGNED NOT NULL,
            `turnover_rate` INT UNSIGNED NOT NULL,
            `turnover_sum` INT UNSIGNED NOT NULL,
            `amplitude` INT UNSIGNED NOT NULL,
            `change_percent` INT NOT NULL,
            PRIMARY KEY ( `stock_code`, `created_date` )
		)ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)" 
	};

public:
	MySQLTool(const char *url,int port,const char *user,const char *pwd):url(url),port(port),user(user),pwd(pwd){
		int status = 0;

		mysql_init(&mysql);
		if(!mysql_real_connect(&mysql,url,user,pwd,db_name,port,0,0)){
			printf("connect mysql failed\n");	
			exit(1);	
		}

		if((status = mysql_select_db(&mysql,db_name)) != 0){
			printf("select db failed!\n");	
			exit(status);	
		}
		
		// XXX 建表，stock_picker的一生只需要执行一次, 并且需要权限
		//checkTables();
	}

	~MySQLTool(){
		mysql_close(&mysql);
	}

	int checkTables(){
		int status = 0;
		uint tables_num = sizeof(tables)/sizeof(*tables);
		for(int i = 0;i != tables_num;i++){
			if((status = mysql_query(&mysql,tables[i]))){
				printf("create table failed: [ %d ]!", i );	
				exit(status);	
			}
		}
		return 0;	
	}
};

#endif

