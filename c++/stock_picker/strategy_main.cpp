#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<string>

#include"include/public/mysqltool.hpp"
#include"include/strategy/strategy.hpp"
#include"include/public/stock.hpp"
#include"include/public/stock_viewer.hpp"

std::map<std::string, std::vector<std::string>>
parse_strategies_data(std::string strategiesData){
/*
 * &[STRATEGY_NAME_1]:PARAM_1:PARAM_2:...&
 * &[STRATEGY_NAME_2]:PARAM_1:PARAM_2:...&
 * &[STRATEGY_NAME_3]& //无参数
 */

	enum struct StrategiesDataState{
		ItemStart, 	
		StrategyNameStart, 	
		StrategyNameEnd, 	
		ParamGap, 	
		ItemEnd, 	
	};

	StrategiesDataState state = StrategiesDataState::ItemEnd;
	uint startIndex = 0;

	std::string paramName;

	std::map<std::string, std::vector<std::string>> params;

	for(uint i = 0;i < strategiesData.size();i++){
		if(strategiesData[i] == '&'){
			if(state == StrategiesDataState::ItemEnd){
				state = StrategiesDataState::ItemStart;
				startIndex = i+1;
			}

			if(state == StrategiesDataState::StrategyNameEnd || state == StrategiesDataState::ParamGap){
				if(state == StrategiesDataState::ParamGap){
					if(startIndex < i && paramName.size()){
						params[paramName].emplace_back(strategiesData.substr(startIndex, i - startIndex));
					}
				}else{
					params[paramName] = std::vector<std::string>();
				}

				paramName.clear();
				state = StrategiesDataState::ItemEnd;
			}
		}else if(strategiesData[i] == '[' && state == StrategiesDataState::ItemStart){
			state = StrategiesDataState::StrategyNameStart;
			startIndex = i+1;
		}else if(strategiesData[i] == ']' && state == StrategiesDataState::StrategyNameStart){
			if(startIndex < i){
				paramName = strategiesData.substr(startIndex, i - startIndex);
			}
			state = StrategiesDataState::StrategyNameEnd;
		}else if(strategiesData[i] == ':' && (state == StrategiesDataState::StrategyNameEnd || state == StrategiesDataState::ParamGap)){ 
			if(state == StrategiesDataState::ParamGap && startIndex < i && paramName.size()){
				params[paramName].emplace_back(strategiesData.substr(startIndex, i - startIndex));
			}
			state = StrategiesDataState::ParamGap;
			startIndex = i+1;
		}
	}
	return params;
}

int
main(int argc,char *argv[]){

	enum OPTS{
		OPT_VALUE_URL = 1,
		OPT_VALUE_PORT,
		OPT_VALUE_USER,
		OPT_VALUE_PWD, 
		OPT_VALUE_STRATEGIES_DATA, 
	};

	static const option longOptions[] = {
		{ "url", required_argument, NULL, OPTS::OPT_VALUE_URL },
		{ "port", required_argument, NULL, OPTS::OPT_VALUE_PORT },
		{ "user", required_argument, NULL, OPTS::OPT_VALUE_USER },
		{ "pwd", required_argument, NULL, OPTS::OPT_VALUE_PWD },
		{ "strategies_data", required_argument, NULL, OPTS::OPT_VALUE_STRATEGIES_DATA },
		{ NULL, no_argument, NULL, 0 }
	};
	
	const char *optString = "url:port:user:pwd:strategies_data:";
	int optIndex;


	int port = 0;
	const char *url = nullptr;
	const char *user = nullptr;
	const char *pwd = nullptr;
	const char *strategiesData = nullptr;

	if(argc > 5){
		while(true){
			int ci = getopt_long(argc,argv,optString,longOptions,&optIndex);
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
				case OPTS::OPT_VALUE_STRATEGIES_DATA:
					strategiesData = optarg;
					break;
				default:
					printf("unkown param : %d\n",opterr);
			}
		}
		if(url && port && user && pwd && strategiesData){
			stockpicker::MySQLTool mysqltool(url,port,user,pwd);
			stockpicker::FileTool filetool("./stocks");
			
			stockpicker::StrategiesLoader strategiesLoader = stockpicker::StrategiesLoader(&mysqltool, &filetool);

			const auto params_ = parse_strategies_data(strategiesData);
			for(auto& param : params_){
				std::cout << std::endl << "param_name: " << param.first << " : param_num: " << param.second.size() << std::endl << "\tparams: ";
				for(int i = 0;i < param.second.size();i++){
					std::cout << param.second[i] << (i == param.second.size() - 1 ? "" : ", ");
				}
				std::cout << std::endl;

				strategiesLoader = strategiesLoader.logicAnd(param.first, param.second);
			}

			const auto& results = strategiesLoader.get_results();
			if(results.size()){
				stockpicker::StockViewer viewer = stockpicker::StockViewer(&mysqltool, &filetool);
				for(auto& item : results){
					const std::vector<std::string>& stockBasicInfo = viewer.getStockBasicInfo(item);
					for(int i = 0; i < stockBasicInfo.size();i++){
						std::cout << stockBasicInfo[i] << (i == stockBasicInfo.size() - 1 ? "" : ", ");
					}
					std::cout << std::endl;
				}
			}

			/*
			auto _params = std::vector<std::string>();
			_params.push_back("3000000000");
			_params.push_back("6220000000");
			
			auto params  =  std::vector<std::string>();
			params.push_back("20");
			params.push_back("183");

			auto __params = std::vector<std::string>();
			__params.push_back("文体传媒");
			__params.push_back("数字经济");

			strategiesLoader.logicAnd("MarketVauleRange", _params)
							.logicAnd("PriceTrendRecently", params)
							.logicAnd("Sectors", params)
			;
			*/


			//auto results = (*strategy)(stockcodes, params);
			//std::cout<<results.size()<<std::endl;
		}else{
			/*
			useage:
				./strategy --url 60.205.210.253 --port 3306 --user stock_picker --pwd happenbigtreasure \
					--strategies_data "&[MarketVauleRange]:3000000000:6220000000&\
										&[PriceTrendRecently]:20:183&\
										&[Sectors]:文体传媒:数字经济&"
			*/
			printf("param error! correct format :\n\t%s --url _url --port _port --user _user --pwd _pwd --strategies_data\n ",argv[0]);
		}
	}else{
		printf("param error! correct format :\n\t%s --url _url --port _port --user _user --pwd _pwd --strategies_data\n",argv[0]);
	}

	return 0;
}
