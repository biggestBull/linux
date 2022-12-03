
#ifndef _STOCK_PICKER_FILETOOL_H

#define _STOCK_PICKER_FILETOOL_H

#include<string>
#include<fstream>
#include<iostream>

#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>

#include"log.hpp"

namespace stockpicker{
	
	class FileTool{
	protected:
		SimpleLog& simple_log = SimpleLog::getInstance();

	private:
		std::string _base_dir;

		bool _checkAndCreateDir(const std::string& path){
			DIR *dir = opendir( path.c_str());
			if(!dir){
				return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != -1;
			}

			closedir(dir);
			return true;
		}

		bool _writeToFile(const std::string& path,const std::string& data){
			std::fstream target_file;

			target_file.open( path.c_str(), std::ios::app);
            if(target_file.fail()){
				return false;	
			}
			target_file << data << std::endl;
			return true;
		}

	public:
		FileTool(const std::string& dir):_base_dir(dir){}

		int storageStockTransactions(const std::string& tas, const std::string& date, const std::string stock_code){
			std::string path = _base_dir + "/" + date;

			if( _checkAndCreateDir(path) ){
				if( _writeToFile(path + "/" + stock_code, tas)){
					return 0;	
				}
				simple_log.error("Write To File", stock_code, "Failed", "");
			}
			simple_log.error("Create Dir", stock_code, "Failed", "");
			return -1;
		}
	};
}

#endif
