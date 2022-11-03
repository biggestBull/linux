
#ifndef MYSQLTOOL_H

#define MYSQLTOOL_H

#include<stdio.h>
#include<mysql/mysql.h>

class MySQLTool{
private:
	const char *user,*pwd,*url;
	int port;
	MYSQL mysql;

	const char* db_name = "java_test";

public:
	MySQLTool(const char* url,int port,const char* user,const char* pwd):url(url),port(port),user(user),pwd(pwd){
		mysql_init(&mysql);
		
		if(!mysql_real_connect(&mysql,url,user,pwd,db_name,port,0,0)){
			printf("connect mysql failed!\n");	
			exit(1);	
		}
	}

	~MySQLTool(){
		mysql_close(&mysql);
	}
};

#endif
