#自己把libmysqlclient和libcurl给装好

CXX := g++
LIBS := -lmysqlclient -lcurl  
HEADS := ./include/spider/*.hpp ./include/public/*.hpp ./include/public/*.inc 
SOURCES := ./source/spider/*.cpp ./source/public/*.cpp 

spider: spider_main.cpp $(SOURCES) $(HEADS)
	$(CXX) --std=c++11 spider_main.cpp $(SOURCES) $(LIBS) -o spider 

.PHONY:clean
clean:
	rm spider

