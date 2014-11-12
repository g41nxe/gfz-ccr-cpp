# GENERIC MAKEFILE
# http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
 
CC := g++ # This is the main compiler

SRCDIR   := src
BUILDDIR := bin
MAINDIR  := src/main # maindir must be a sub of srcdir
SRCEXT   := cpp

CFLAGS   := -g -Wall
LIB      := -pthread -fopenmp -std=c++11
INC      := # -I include

SOURCES  := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS  := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
MAINSRCS := $(shell find $(MAINDIR) -type f -name *.$(SRCEXT))
MAINOBJS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(MAINSRCS:.$(SRCEXT)=.o))
TARGETS  := $(notdir $(basename $(shell find $(MAINDIR) -type f -name *.$(SRCEXT))))

BENCHRESULT_FILE = $(shell date)
NOW := $(shell date +"%Y-%m-%d:%H:%M:%S" | tr ' :' '_')

# threacdount for openmp
THREADCOUNT := 4
export THREADCOUNT
OMP_NUM_THREADS := 4
export OMP_NUM_THREADS

# build all targets
$(TARGETS): $(OBJECTS) $(MAINOBJS)
	@echo "Linking ..."
	@for target in $(TARGETS); do \
	$(CC) $(filter-out $(MAINOBJS), $(OBJECTS)) $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(MAINDIR))/$$target.o -o $$target $(LIB); \
	done

# Rule to build .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIB) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGETS)
	$(RM) benchmark/*.txt
	$(RM) results.tgz

.PHONY: clean

benchmark: $(TARGETS)
	@echo "Running iterative benchmark"
	./test 0 >> benchmark/ite.txt
	@echo "Running recursive benchmark"
	./test 1 >> benchmark/rec.txt
	@echo "Running omp benchmark"
	./test 3 >> benchmark/omp.txt 
	@echo "Running bruteforce benchmark"
	./test 4 >> benchmark/bru.txt

.PHONY: benchmark

threadtest: $(TARGETS)
	@for threads in 4 8 16 32; do \
		rm -f benchmark/omp_$$threads.txt; \
		touch benchmark/omp_$$threads.txt; \
		echo "Running omp benchmark with $$threads threads"; \
		THREADCOUNT=$$threads; \
		export THREADCOUNT; \
		OMP_NUM_THREADS=$$threads; \
		export OMP_NUM_THREADS; \
		./test 3 >> benchmark/omp_$$threads.txt; \
	done

.PHONY: threadtest

gnuplot:
	@echo "Creating Benchmark Diagrams"
	$(shell cd benchmark; gnuplot gnuplot_array.plt; gnuplot gnuplot.plt; gnuplot gnuplot_8192.plt; gnuplot gnuplot_threads.plt)

.PHONY: gnuplot


pack: 
	@echo "Packing result files"
	@$(RM) -f results.tgz
	tar czf results.tgz benchmark/*.txt

.PHONY: pack
