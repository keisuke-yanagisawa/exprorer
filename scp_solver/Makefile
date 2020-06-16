CXX ?= g++
CXXFLAGS = -std=c++11
ifeq ($(Debug),Y) 
CXXFLAGS += -g -O0
else
CXXFLAGS += -O2
endif

ifeq ($(Symbols),Y)
CXXFLAGS += -g
endif

.SUFFIXES: .cpp .o
OBJS= objs/main.o objs/beasley1990.o objs/problem.o objs/state.o
ALL = objs scp_solver

.PHONY: clean all

all:  $(ALL)

objs:
	mkdir -p objs

scp_solver: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

objs/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
objs/beasley1990.o: src/beasley1990.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
objs/problem.o: src/problem.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
objs/state.o: src/state.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf $(ALL)
