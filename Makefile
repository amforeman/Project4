SRCS = Snake.cpp
EXE=Snake


# Everything below that should not have to change ever.

CXX=g++
LD=g++
CC=g++

TARGET =  all

OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -I/home/amforeman/bridges/include -I/home/amforeman/bridges/include/rapidjson

LDFLAGS = -L/home/amforeman/bridges/lib -L/usr/include/x86_64-linux-gnu/curl
LIBS =  -lcurl -l bridges

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LDFLAGS) $(LIBS)

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

all : $(EXE)

clean:
	-rm $(OBJS)
	-rm $(EXE)
