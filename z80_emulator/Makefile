PROJ	= cpu_em
CC		= gcc
CCFLAGS = -Wall -Wextra -g

SOURCES = $(PROJ).c decoder.c arithmetic.c shifts.c logic.c load_store.c jumps_interrupts.c
OBJECTS = $(PROJ).o decoder.o arithmetic.o shifts.o logic.o load_store.o jumps_interrupts.o

$(PROJ): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $(PROJ) $(OBJECTS)

$(PROJ).o: $(PROJ).c cpu_em.h
	$(CC) $(CCFLAGS) -c $(PROJ).c

decoder.o: decoder.c cpu_em.h instructions.h
	$(CC) $(CCFLAGS) -c decoder.c

arithmetic.o: arithmetic.c instructions.h
	$(CC) $(CCFLAGS) -c arithmetic.c

logic.o: logic.c instructions.h
	$(CC) $(CCFLAGS) -c logic.c

load_store.o: load_store.c instructions.h
	$(CC) $(CCFLAGS) -c load_store.c

jumps_interrupts.o: jumps_interrupts.c instructions.h
	$(CC) $(CCFLAGS) -c jumps_interrupts.c

shifts.o: shifts.c instructions.h
	$(CC) $(CCFLAGS) -c shifts.c

.PHONY: clean

clean:
	del /f /q cpu_em *.o