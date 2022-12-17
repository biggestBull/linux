
#ifndef _STOCK_PICKER_FILETOOL_H

#define _STOCK_PICKER_FILETOOL_H

#include<string>
#include<fstream>
#include<iostream>

#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>

namespace stockpicker{
	
	class FileTool{
		friend class SpiderStocksOverview;

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

			target_file.open( path.c_str(), std::ios::out);
            if(target_file.fail()){
				return false;	
			}
			target_file << data << std::endl;
			return true;
		}

	public:
		FileTool(const std::string& dir):_base_dir(dir){}

	};
}

#endif
