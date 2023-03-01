#自己把libmysqlclient和libcurl给装好

CXX := g++

SPIDER_LIBS := -lmysqlclient -lcurl  
SPIDER_HEADS := ./include/spider/*.hpp ./include/public/*.hpp ./include/public/*.inc 
SPIDER_SOURCES := ./source/spider/*.cpp ./source/public/*.cpp 

spider: spider_main.cpp $(SPIDER_SOURCES) $(SPIDER_HEADS)
	$(CXX) --std=c++11 spider_main.cpp $(SPIDER_SOURCES) $(SPIDER_LIBS) -o spider 



STRATEGY_LIBS := -lmysqlclient   
STRATEGY_HEADS := ./include/strategy/*.hpp ./include/public/*.hpp ./include/public/*.inc 
STRATEGY_SOURCES := ./source/public/*.cpp 

strategy: strategy_main.cpp $(STRATEGY_SOURCES) $(STRATEGY_HEADS)
	$(CXX) --std=c++11 strategy_main.cpp $(STRATEGY_SOURCES) $(STRATEGY_LIBS) -o strategy



SERVER_LIBS := -lpthread   
server: server_main.cpp 
	$(CXX) --std=c++11 server_main.cpp $(SERVER_LIBS) -o server

.PHONY:clean
clean:
	rm spider
	rm strategy
	rm server

