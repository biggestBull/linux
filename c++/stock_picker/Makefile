#自己把libmysqlclient和libcurl给装好

CXX := g++
LIBS := -lmysqlclient -lcurl  
HEADS := ./include/*.hpp ./include/*.inc 
SOURCES := ./source/*.cpp  

a.out: main.cpp $(SOURCES) $(HEADS)
	$(CXX) --std=c++11 main.cpp $(SOURCES) $(LIBS) 
