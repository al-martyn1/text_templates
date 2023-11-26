DLLNAME = out/release/texttp.dll
SRCS = src/texttp.cpp
DEF = src/texttp.def


INCLUDE = -IC:/MinGW/include -IC:/MinGW/include/c++/3.4.2 -ID:/PSDK/Include -ID:/PSDK/Include/atl


CXX = g++
CXXFLAGS = -v -mno-cygwin -mdll -O2 -pedantic $(INCLUDE)
LNKFLAGS = -mno-cygwin -mdll -luser32 -lole32 -s
RM = rm -f
DLLTOOL=dlltool

all: $(DLLNAME)

OBJS = $(patsubst %.def,%.o,$(patsubst %.cpp,%.o,$(filter %.cpp %.def,$(SRCS))))
DEPS = $(patsubst %.cpp,%.d,$(filter %.cpp,$(SRCS)))

%.d: %.cpp
	@echo making depends for $<
	@$(SHELL) -ec '$(CXX) -c -M $(CXXFLAGS) $< \
		| sed '\''s;\($*\).o[ :]*;\1.o $@: ;g'\'' > $@; [ -s $@ ] || rm -f $@'

%.o: %.cpp
	@echo compiling $<
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.def
	@echo creating export file
	@dlltool -e $@ -d $<

$(DLLNAME): $(OBJS)
	@echo linking
	@$(CXX) -mdll -o nul -Xlinker --base-file -Xlinker $@.base $^ $(LNKFLAGS)
	@$(DLLTOOL) --dllname $@ --base-file $@.base --output-exp $@.exp --def $(DEF)
	@$(CXX) -mdll  -o $@ $^ $@.exp $(LNKFLAGS)
	@$(RM) $@.base
	@$(RM) $@.exp

-include $(DEPS)

clean:
	@echo cleaning up
	@rm -f $(DEPS) $(OBJS)
