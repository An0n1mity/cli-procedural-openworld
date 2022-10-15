DEBUG = no

ifeq ($(DEBUG),yes)
	DEBUGFLAGS = -O0
else
	DEBUGFLAGS = -O2
endif

CC = gcc
CCFLAGS = $(DEBUGFLAGS) -Wall
LDFLAGS = 
INCLUDES = -I. -I/usr/include -Iscplib
LIBS = -L/usr/lib -lncursesw -lm -lform -lmenu

EXEC = toRun
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

empty = 
space = $(empty) $(empty)
comma = ,

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $(EXEC) $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES)

.PHONY: all run clear mrproper

run: all
	@./$(EXEC)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
