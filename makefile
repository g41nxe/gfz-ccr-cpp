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
	@echo "Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGETS)
	$(RM) benchmark/*.txt

.PHONY: clean

benchmark:
	@echo "running bruteforce benchmark"
	@bash ./benchmark/benchmark.sh ./bench_bru benchmark/bru 0
	@echo "running iterative benchmark"
	@bash ./benchmark/benchmark.sh ./bench_fft benchmark/ite 0
	@echo "running recursive benchmark"
	@bash ./benchmark/benchmark.sh ./bench_fft benchmark/rec 1
	@echo "running thread benchmark"
	@bash ./benchmark/benchmark.sh ./bench_fft benchmark/thr 2
	@echo "running omp benchmark"
	@bash ./benchmark/benchmark.sh ./bench_fft benchmark/omp 3
.PHONY: benchmark

gnuplot:
	@echo "creating benchmark diagram"
	$(shell cd benchmark; gnuplot gnuplot.plt)

.PHONY: gnuplot

