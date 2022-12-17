#include"../../include/public/mysqltool.hpp"

namespace stockpicker{
	void MySQLTool::_error(const std::string &sql){
		simple_log.error("Execute SQL", sql, "Failed", mysql_error(&_mysql));	
	}

	int MySQLTool::_exec(const std::string &sql){
		int rt = 0;
		if(rt = mysql_query(&_mysql, sql.c_str())){ 
			_error(sql);
			return rt;
		}
		return 0;
	}

	MYSQL_RES* MySQLTool::_query(const std::string &sql){
		_exec(sql);

		auto *result = mysql_store_result(&_mysql);
		if(!result){ 
			_error(sql);
			return NULL;
		}
		return result;
	}

	std::vector<std::vector<std::string>> MySQLTool::query(const std::string& sql){
		std::vector<std::vector<std::string>> results;

		auto query_result = _query(sql);
		
		auto num_fields = mysql_num_fields(query_result);

		MYSQL_ROW row;
		while((row = mysql_fetch_row(query_result))){
			unsigned long *lengths;
			lengths = mysql_fetch_lengths(query_result);

			std::vector<std::string> row_fields;
			for(unsigned int i = 0; i < num_fields; i++){
				row_fields.push_back(std::string(row[i]));
			}
			results.push_back(row_fields);
		}	

		mysql_free_result(query_result);
		return results; 	
	}

	bool MySQLTool::dataAlreadyExists(const std::string &sql){
		MYSQL_RES *result;
		if(!(result = _query(sql))) return false;

		auto rt = mysql_num_rows(result);
		mysql_free_result(result);

		return rt > 0;
	}

}
