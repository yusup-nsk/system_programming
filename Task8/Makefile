CC =  gcc 
CFLAGS = -Wall -Wextra -Werror -pedantic
SOURCE_DIR = ./arithmetic_functions
EXECUTABLE = calculator
LIB = libcalc.so

all: exe run

rebuild: clean  exe

exe: $(LIB)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main.c -L. -l:$(LIB) -Wl,-rpath,"${PWD}"

run:
	./$(EXECUTABLE)

objects:
	$(CC)  $(CFLAGS)  -c -fPIC $(SOURCE_DIR)/*.c

$(LIB): objects
	$(CC) -o $(LIB) -shared  *.o
	make rm_objects

format:
	clang-format --style=Google -i $(SOURCE_DIR)/*.c *.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c $(SOURCE_DIR)/*.c

rm_objects:
	rm -f *.o 

clean: rm_objects
	rm -f $(EXECUTABLE)
	rm -f $(LIB)
	rm -f a.out