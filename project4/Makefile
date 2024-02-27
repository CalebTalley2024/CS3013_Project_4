
CFLAGS += -Wpedantic -pedantic-errors
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
#CFLAGS += -Waggregate-return
CFLAGS += -Wbad-function-cast
CFLAGS += -Wcast-align
CFLAGS += -Wno-cast-qual	# free() should accept const pointers
CFLAGS += -Wno-declaration-after-statement
CFLAGS += -Wfloat-equal
CFLAGS += -Wformat=2
CFLAGS += -Wlogical-op
CFLAGS += -Wmissing-include-dirs
CFLAGS += -Wno-missing-declarations
CFLAGS += -Wno-missing-prototypes
CFLAGS += -Wnested-externs
CFLAGS += -Wpointer-arith
CFLAGS += -Wredundant-decls
CFLAGS += -Wsequence-point
CFLAGS += -Wshadow
CFLAGS += -Wno-strict-prototypes
CFLAGS += -Wswitch
CFLAGS += -Wundef
CFLAGS += -Wunreachable-code
CFLAGS += -Wunused-but-set-parameter
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wwrite-strings

BINARIES += scheduler

all: $(BINARIES)

OPT = -O3

scheduler: scheduler.o
	gcc -g $(OPT) $^ -o $@

%.o: %.c Makefile
	gcc -g $(OPT) -x c $(CFLAGS) $< -c -o $@ -MD -MF $(@:.o=.d)

clean: FORCE
	rm -f *.d *.o $(BINARIES)

test: scheduler FORCE
	@for n in $$(for f in tests/*.run ; do basename $$f .run ; done | sort -n) ; do \
		bash ./run_tests.sh -t $$n ; \
	done

workflows: scheduler FORCE
	@./workload_1.pl && echo workload_1.in is valid
	@./workload_2.pl && echo workload_2.in is valid
	@./workload_3.pl && echo workload_3.in is valid
	@./workload_4.pl && echo workload_4.in is valid
	@./workload_5.pl && echo workload_5.in is valid
	
.PHONY: FORCE clean test all workflows

-include *.d

