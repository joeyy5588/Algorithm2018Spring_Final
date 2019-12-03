# CC and CFLAGS are varilables
CC = g++ -std=c++11
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2

all	: watermark
	@echo -n ""

# optimized version
watermark	: fsm_opt.o main_opt.o lib
			$(CC) $(OPTFLAGS) fsm_opt.o main_opt.o -ltm_usage -Llib -o watermark
main_opt.o	: main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
fsm_opt.o	: fsm.cpp fsm.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
# DEBUG Version
dbg : watermark_dbg
	@echo -n ""

watermark_dbg	: fsm_dbg.o main_dbg.o tm_usage.o
			$(CC) $(DBGFLAGS) fsm_dbg.o main_dbg.o time_usage.o -o watermark_dbg
main_dbg.o	: main.cpp
			$(CC) $(CFLAGS) main.cpp
fsm_dbg.o	: fsm.cpp fsm.h
			$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<



# clean all the .o and executable files
clean:
		rm -f watermark *.o

