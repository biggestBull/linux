#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<string>

#include"include/public/mysqltool.hpp"
#include"include/spider/spider.hpp"
#include"include/public/stock.hpp"

int
main(int argc,char *argv[]){
	curl_global_init(CURL_GLOBAL_DEFAULT);

	enum OPTS{
		OPT_VALUE_URL = 1,
		OPT_VALUE_PORT,
		OPT_VALUE_USER,
		OPT_VALUE_PWD, 
		OPT_VALUE_CODE, 
		OPT_VALUE_DATE 
	};

	static const option long_options[] = {
		{ "url", required_argument, NULL, OPTS::OPT_VALUE_URL },
		{ "port", required_argument, NULL, OPTS::OPT_VALUE_PORT },
		{ "user", required_argument, NULL, OPTS::OPT_VALUE_USER },
		{ "pwd", required_argument, NULL, OPTS::OPT_VALUE_PWD },
		{ "code", required_argument, NULL, OPTS::OPT_VALUE_CODE },
		{ "date", required_argument, NULL, OPTS::OPT_VALUE_DATE },
		{ NULL, no_argument, NULL, 0 }
	};
	
	const char *opt_string = "url:port:user:pwd:";
	int opt_index;

	int port = 0;
	const char *url = nullptr;
	const char *user = nullptr;
	const char *pwd = nullptr;

	const char *code = nullptr;
	const char *date = nullptr;

	if(argc > 5){
		while(true){
			int ci = getopt_long(argc,argv,opt_string,long_options,&opt_index);
			if(ci == -1) break;
	
			OPTS opts = static_cast<OPTS>(ci);
	
			switch(opts){
				case OPTS::OPT_VALUE_URL:
					url = optarg;
					break;
				case OPTS::OPT_VALUE_PORT:
					port = std::stoi(optarg);
					break;
				case OPTS::OPT_VALUE_USER:
					user = optarg;
					break;
				case OPTS::OPT_VALUE_PWD:
					pwd = optarg;
					break;
				case OPTS::OPT_VALUE_CODE:
					code = optarg;
					break;
				case OPTS::OPT_VALUE_DATE:
					date = optarg;
					break;
				default:
					printf("unkown param : %d\n",opterr);
			}
		}
		if(url && port && user && pwd){
			stockpicker::MySQLTool mysqltool(url,port,user,pwd);
			stockpicker::FileTool filetool("./stocks");
			
			auto spider = stockpicker::SpiderStocksOverview(&mysqltool, &filetool);  

			std::string _code, _date;
			if(code){
				_code = code;
			}
			if(date){
				_date = date;
			}
			spider.getAllStocks(_code, _date);
		}
	}else{
		printf("param error! correct format :\n\t%s --url _url --port _port --user _user --pwd _pwd [ --code _code ] [ --date _date ( for example: 2022-12-17 ) ]\n",argv[0]);
	}

	curl_global_cleanup();
	return 0;
}
