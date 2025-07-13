CC =  gcc -Wall -Wextra -Werror -pedantic
SOURCE_DIR = ./arithmetic_functions
EXECUTABLE = calculator
LIB = libcalc.a

all: run

rebuild: clean  exe

exe: $(LIB)
	$(CC) main.c -L. $(LIB) -o $(EXECUTABLE)

run: exe
	./$(EXECUTABLE)

objects:
	$(CC) -c $(SOURCE_DIR)/*.c

$(LIB): objects
	ar rcs $(LIB) *.o
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