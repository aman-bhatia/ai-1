CXX = g++

CFLAGS = -std=c++11
PFLAGS = -DPRINT
DFLAGS = -g

SRCDIR = src
OBJDIR = obj
INCDIR = inc
DEPDIR = deps

INCPATH = -I$(INCDIR)

OTMP = $(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))
DEPS = $(patsubst %.o,$(DEPDIR)/%.d,$(OTMP))
SRCS = $(wildcard $(SRCDIR)/*.cpp)
INCLUDES = $(wildcard $(INCDIR)/*.hpp)
OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.cpp=.o))

MAIN = a.out

execute: init $(MAIN)
	./$(MAIN) $(input)

debug: CFLAGS += $(DFLAGS)
debug: init $(MAIN)
	./$(MAIN)

print: CFLAGS += $(PFLAGS)
print: init $(MAIN)
	./$(MAIN) $(input)

init:
	mkdir -p $(DEPDIR)
	mkdir -p $(OBJDIR)

-include $(DEPS)
	
$(MAIN): $(OBJS)
	$(CXX) $(CFLAGS) -o $(MAIN) $(OBJS) -L$(SRCDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCPATH) -o $@ -c $<
	$(CXX) -MM -MT $(OBJDIR)/$*.o $(CFLAGS) $(INCPATH) $(SRCDIR)/$*.cpp > $(DEPDIR)/$*.d
	@cp -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's/.*://' -e 's/\\\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
        sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

clean:
	$(RM) $(OBJDIR)/*.o $(DEPDIR)/*.d *~ $(MAIN)
