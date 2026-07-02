CC     = gcc
CFLAGS = -O2 -Wall -Wextra
LIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# All object files required to build the program
OBJS   = main.o sched_ui.o fcfs.o sjf.o stcf.o
EXEC   = scheduler

all: $(EXEC)

# Link all objects to create the final executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LIBS)

# Compile C files into object files
%.o: %.c sched.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC)

.PHONY: all clean