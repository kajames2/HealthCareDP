CC := clang++-6.0
SRCDIR := src
BUILDDIR := build
TESTDIR := test
TARGET := health_care_dp_main
TARGET2 := fast_dp
TARGET3 := fast_dp_fitness

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
TESTS := $(shell find $(TESTDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTS:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++14 -O3 -Wl,-rpath,./lib/ -fopenmp -DNDEBUG
LIB := -Wl,--start-group -lgtest -lgtest_main -lpthread -lhealthcare -lgenericdp -lboost_filesystem -lboost_system -lboost_program_options -Llib/ -Wl,--end-group 
INC := -I include -I lib

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $(CFLAGS) $(TARGET).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET)"; $(CC) $(CFLAGS) $(TARGET).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET)

$(TARGET2): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $(CFLAGS) $(TARGET2).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET2)"; $(CC) $(CFLAGS) $(TARGET2).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET2)

$(TARGET3): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $(CFLAGS) $(TARGET3).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET3)"; $(CC) $(CFLAGS) $(TARGET3).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET3)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tests_main: $(TESTOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) tests_main.cc $(INC)  $(TESTOBJECTS) $(OBJECTS) -o bin/tests_main $(LIB)

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	@echo " Making $(@D)"
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
