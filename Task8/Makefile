CC =  gcc 
CFLAGS = -Wall -Wextra -Werror -pedantic
SOURCE_DIR = ./arithmetic_functions
EXECUTABLE = calculator
LIB = libcalc.so

all: run

rebuild: clean  exe

exe: objects $(LIB)
	$(CC)  $(CFLAGS)  -c -fPIC main.c
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
	$(CC) -o $(EXECUTABLE) main.o -L./ libcalc.so

run: exe
	./$(EXECUTABLE)

objects:
	$(CC)  $(CFLAGS)  -c -fPIC $(SOURCE_DIR)/*.c

$(LIB): objects
	$(CC) -shared -o $(LIB) *.o
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