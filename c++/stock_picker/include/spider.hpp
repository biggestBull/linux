#ifndef _SPIDER_CLASS_
#define _SPIDER_CLASS_

#include<curl/curl.h>
#include<string.h>
#include <iostream>

#define CURL_MAX_BUF_LENGTH 10000
#define CURL_MAX_ERROR_BUF_LENGTH 1000

//interface
class Spider{
private:
	CURL* _curl;

	static uint _writeCb(char *in, uint size, uint nmemb, char *out){
		uint r;

		r = size * nmemb;
		strncpy ( out, in, CURL_MAX_BUF_LENGTH < r ? CURL_MAX_BUF_LENGTH : r );
		std::cout<<in;
		return r;
	}

public:
	char _curl_error_buf[CURL_MAX_ERROR_BUF_LENGTH];
	char _curl_buf[CURL_MAX_BUF_LENGTH];

	Spider(){
		_curl = curl_easy_init();	
	}

	~Spider(){
		curl_easy_cleanup(_curl);
	}
	
	int getWebContent(const char* url){
		curl_easy_setopt(_curl, CURLOPT_URL, url);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _writeCb);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, _curl_buf);
		curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _curl_error_buf);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
		return curl_easy_perform(_curl);
	}
};

class SpiderStocksOverview:Spider{
public:
	SpiderStocksOverview() = default;
	void test(){
		getWebContent("http://51.push2.eastmoney.com/api/qt/stock/details/sse?fields1=f1,f2,f3,f4&fields2=f51,f52,f53,f54,f55&mpi=2000&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&pos=-0&secid=0.000001&wbp2u=|0|0|0|web");
	}
};

#endif
