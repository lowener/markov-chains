CXX=clang++
OBJS=cmkv.o
VPATH=src
CXXFLAGS=-pedantic -Wall -Wextra -g -std=c++17

cmkv: $(OBJS)
	$(CXX) $(OBJS) -o cmkv

clean:
	$(RM) $(OBJS) cmkv
