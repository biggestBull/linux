#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<string>

#include"include/public/mysqltool.hpp"
#include"include/strategy/strategy.hpp"
#include"include/public/stock.hpp"

int
main(int argc,char *argv[]){

	enum OPTS{
		OPT_VALUE_URL = 1,
		OPT_VALUE_PORT,
		OPT_VALUE_USER,
		OPT_VALUE_PWD, 
		OPT_VALUE_STRATEGY_NAME, 
	};

	static const option long_options[] = {
		{ "url", required_argument, NULL, OPTS::OPT_VALUE_URL },
		{ "port", required_argument, NULL, OPTS::OPT_VALUE_PORT },
		{ "user", required_argument, NULL, OPTS::OPT_VALUE_USER },
		{ "pwd", required_argument, NULL, OPTS::OPT_VALUE_PWD },
		{ "strategy_name", required_argument, NULL, OPTS::OPT_VALUE_STRATEGY_NAME },
		{ NULL, no_argument, NULL, 0 }
	};
	
	const char *opt_string = "url:port:user:pwd:strategy_name:";
	int opt_index;


	int port = 0;
	const char *url = nullptr;
	const char *user = nullptr;
	const char *pwd = nullptr;
	const char *strategy_name = nullptr;

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
				case OPTS::OPT_VALUE_STRATEGY_NAME:
					strategy_name = optarg;
					break;
				default:
					printf("unkown param : %d\n",opterr);
			}
		}
		if(url && port && user && pwd && strategy_name){
			stockpicker::MySQLTool mysqltool(url,port,user,pwd);
			stockpicker::FileTool filetool("./stocks");
			
			stockpicker::StrategyFilter *strategy = new stockpicker::StrategyFilterByMarketValueRange(&mysqltool, &filetool);

			//testing

			std::vector<int> stockcodes;
			stockcodes.push_back(11);
			stockcodes.push_back(2);
			stockcodes.push_back(4);

			auto params = std::vector<std::string>();
			params.push_back("1110000000");
			params.push_back("6220000000");

			//(*strategy)(stockcodes, params);

			strategy = new stockpicker::StrategyFilterByPriceTrendRecently(&mysqltool, &filetool);
			params[0] = "20";
			params[1] = "183";
			auto results = (*strategy)(stockcodes, params);
			std::cout<<results.size()<<std::endl;
		}
	}else{
		printf("param error! correct format :\n\t%s --url _url --port _port --user _user --pwd _pwd --strategy_name\n",argv[0]);
	}

	return 0;
}
