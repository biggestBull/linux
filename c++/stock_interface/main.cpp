#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<string>

#include"include/mysqltool"

int
main(int argc,char * argv[]){
	enum OPTS{
		OPT_VALUE_URL = 1,
		OPT_VALUE_PORT,
		OPT_VALUE_USER,
		OPT_VALUE_PWD
	};

	static const struct option long_options[] = {
		{ "url", required_argument, NULL, OPTS::OPT_VALUE_URL },
		{ "port", required_argument, NULL, OPTS::OPT_VALUE_PORT },
		{ "user", required_argument, NULL, OPTS::OPT_VALUE_USER },
		{ "pwd", required_argument, NULL, OPTS::OPT_VALUE_PWD },
		{ NULL, no_argument, NULL, 0 }
	};
	
	const char* opt_string = "url:port:user:pwd:";
	int opt_index;

	int port;
	const char* url;
	const char* user;
	const char* pwd;

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
			default:
				printf("unkown param : %d\n",opterr);
		}
	}
	
	if(url && port && user && pwd){
		MySQLTool mysqltool(url,port,user,pwd);
	}else{
		printf("param error! correct format :\n\t%s --url _url --port _port --user _user --pwd _pwd\n",argv[0]);
	}
	

	return 0;
}

