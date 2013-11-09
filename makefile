# GENERIC MAKEFILE
# http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
 
CC := g++ # This is the main compiler

SRCDIR   := src
BUILDDIR := bin
MAINDIR  := src/main # maindir must be a sub of srcdir
SRCEXT   := cpp

CFLAGS   := -g -Wall
LIB      := -pthread -std=c++11
INC      := # -I include

SOURCES  := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS  := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
MAINSRCS := $(shell find $(MAINDIR) -type f -name *.$(SRCEXT))
MAINOBJS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(MAINSRCS:.$(SRCEXT)=.o))
TARGETS  := $(notdir $(basename $(shell find $(MAINDIR) -type f -name *.$(SRCEXT))))

# build all targets
$(TARGETS): $(OBJECTS) $(MAINOBJS)
	@echo "Linking ..."
	@for target in $(TARGETS); do \
	echo $(CC) $(filter-out $(MAINOBJS), $(OBJECTS)) $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(MAINDIR))/$$target.o -o $$target $(LIB); \
	$(CC) $(filter-out $(MAINOBJS), $(OBJECTS)) $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(MAINDIR))/$$target.o -o $$target $(LIB); \
	done

# Rule to build .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Compiling ..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIB) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGETS)

.PHONY: clean