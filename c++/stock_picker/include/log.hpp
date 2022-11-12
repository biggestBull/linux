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
		std::string _suffix = ".log";
		std::string _filename = "latest";
		std::string _filedir = "./logs/";
		uint _log_maxium_size = 1024 * 1024 * 3;
		std::ofstream _log_file;		

		bool _console = false;

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

		void _write(std::string level, std::string event, std::string who, std::string result, std::string reason){
			std::string output_str(" [ " + _now() + " ] " + level + " <> " + event + " <> Mr. " + who + " <> [ " + result + " ] <> " + reason);
			if(_console){
				_console = false;
				std::cout<<output_str<<std::endl;
			}
			_log_file<<output_str<<std::endl;
			_rotateCheck();
		}

		int _renameLogFile(const char *new_name){
			return rename((_filedir + "/" + _filename + _suffix).c_str(), new_name);
		}

		int _openLogFile(){
			_log_file.open( (_filedir + "/" + _filename + _suffix).c_str(), std::ios::app);	
			return _log_file.fail();
		}

		int _tryRotate(){
			if( !_renameLogFile( ( _filedir + "/" + _now("%Y_%m_%d_%H_%M_%S") + _suffix ).c_str() ) ){
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

		SimpleLog& console(){
			_console = true;
			return *this;
		}

		void info(std::string event, std::string who, bool status = true, std::string reason = ""){
			_write("INFO", event, who, status?"SUCCESS":"FAILED", reason);
		}	

		void warn(std::string event, std::string who, std::string result, std::string reason = ""){
			_write("WARN", event, who, result, reason);
		}	

		void error(std::string event, std::string who, std::string result, std::string reason = ""){
			_write("ERROR", event, who, result, reason);
		}	

		void fatal(std::string event, std::string who, std::string result, std::string reason = ""){
			_write("FATAL", event, who, result, reason);
		}	

		int changeLogPath(std::string new_filename, std::string new_dir = ""){
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
