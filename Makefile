CC := clang++-4.0
SRCDIR := src
BUILDDIR := build
TESTDIR := test
TARGET := health_care_dp_main

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
TESTS := $(shell find $(TESTDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTS:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++14 -O3 -Wl,-rpath lib/
LIB := -lgtest -lgtest_main -lpthread -lhealthcare -Llib/
INC := -I include/healthcaredp -I lib/genericdp -I lib/healthcare

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $(TARGET).$(SRCEXT) $^ -o $(TARGET) $(SRCEXT)"; $(CC) $(CFLAGS) $(TARGET).$(SRCEXT) $(INC) $^ $(LIB) -o bin/$(TARGET)

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
