#ifndef _STOCK_PICKER_LOG_H 

#define _STOCK_PICKER_LOG_H 

#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include <cstdlib>

namespace stockpicker{
	class SimpleLog{
	private:
		std::string _filename = "latest.log";
		std::string _filedir = "./logs/";
		uint _log_maxium_size = 1024 * 1024 * 3;
		std::ofstream _log_file;		

		SimpleLog(){
			_openLogFile();
		}

		SimpleLog(const SimpleLog&) = delete;
		SimpleLog& operator=(const SimpleLog&) = delete;

		std::string _now(const char *format = "%Y-%m-%d %T"){
			char tmp[20];
			time_t cur_time(time(NULL));
			strftime(tmp, sizeof(tmp), format, localtime(&cur_time));

			return tmp;
		}

		void _write(std::string level, std::string event, std::string result, std::string reason){
			_log_file<<" [ " << _now() << " ] " << level << " <> " << event << " <> [ " << result << " ] :\n\t" << reason << std::endl;
			_rotateCheck();
		}

		int _renameLogFile(const char *new_name){
			return rename((_filedir + _filename).c_str(), new_name);
		}

		int _openLogFile(){
			_log_file.open( (_filedir + _filename).c_str(), std::ios::app);	
			return _log_file.fail();
		}

		int _tryRotate(){
			if( !_renameLogFile( ( _filedir + _now("%Y_%m_%d_%H_%M_%S") + ".log" ).c_str() ) ){
				_log_file.close();

				return _openLogFile();
			}
			return -1; 
		}

		int _rotateCheck(){
			int size = _log_file.tellp();

			if(size >= _log_maxium_size){
				return _tryRotate();
			}
			return 0;
		}

	public:
		static SimpleLog& getInstance(){
			static SimpleLog simpleLog;
			return simpleLog;
		}

		~SimpleLog(){
			_log_file.close();
		}

		void info(std::string event, std::string result, std::string reason = ""){
			_write("INFO", event, result, reason);
		}	

		void warn(std::string event, std::string result, std::string reason = ""){
			_write("WARNING", event, result, reason);
		}	

		void error(std::string event, std::string result, std::string reason = ""){
			_write("ERROR", event, result, reason);
		}	

		void fatal(std::string event, std::string result, std::string reason = ""){
			_write("FATAL", event, result, reason);
		}	

		int setLogDir(std::string new_filename, std::string new_dir = ""){
			_log_file.close();
			
			_filename = new_filename;
			if(!new_dir.empty()){
				_filedir = new_dir;
			}
			return _openLogFile();
		}
	};
}

#endif
