#ifndef _STOCK_PICKER_LOG_H 

#define _STOCK_PICKER_LOG_H 

#include<iostream>
#include<fstream>
#include<string>
#include<tuple>
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

		std::string _record_date;
		uint _warn_cnt;
		uint _error_cnt;
		uint _fatal_cnt;

		class Console{
			friend class SimpleLog;
		private:
			SimpleLog &_simple_log;
			Console(SimpleLog& simple_log):_simple_log(simple_log){}

		public:
			template<typename ...Ts>
			void info(Ts ...args){
				_simple_log.info(std::forward<Ts>(args)..., true);
			}	

			template<typename ...Ts>
			void warn(Ts ...args){
				_simple_log.warn(std::forward<Ts>(args)..., true);
			}	

			template<typename ...Ts>
			void error(Ts ...args){
				_simple_log.error(std::forward<Ts>(args)..., true);
			}	

			template<typename ...Ts>
			void fatal(Ts ...args){
				_simple_log.fatal(std::forward<Ts>(args)..., true);
			}	
		};

	public:
		Console console;

	private:
		SimpleLog():console(*this), _record_date(_now()){
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

		void _write(std::string level, std::string event, std::string who, std::string result, std::string reason, bool console){
			std::string output_str(" [ " + _now() + " ] " + level + " <> " + event + " <> Mr. " + who + " <> [ " + result + " ] <> " + reason);
			if(console){
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

		void info(std::string event, std::string who, bool status, std::string reason, bool _console = false){
			_write("INFO", event, who, status?"SUCCESS":"FAILED", reason, _console);
		}	

		void warn(std::string event, std::string who, std::string result, std::string reason, bool _console = false){
			_warn_cnt++;
			_write("WARN", event, who, result, reason, _console);
		}	

		void error(std::string event, std::string who, std::string result, std::string reason, bool _console = false){
			_error_cnt++;
			_write("ERROR", event, who, result, reason, _console);
		}	

		void fatal(std::string event, std::string who, std::string result, std::string reason, bool _console = false){
			_fatal_cnt++;
			_write("FATAL", event, who, result, reason, _console);
		}	

		int changeLogPath(std::string new_filename, std::string new_dir = ""){
			_log_file.close();
			
			_filename = new_filename;
			if(!new_dir.empty()){
				_filedir = new_dir;
			}
			return _openLogFile();
		}

		std::tuple<std::string, uint, uint, uint> recordInfo(bool reset = false){
			auto rt = std::make_tuple(_record_date, _warn_cnt, _error_cnt, _fatal_cnt);
			if(reset){
				_record_date = _now();
				_warn_cnt = _error_cnt = _fatal_cnt = 0;
			}
			return rt;
		}
	};
}

#endif
