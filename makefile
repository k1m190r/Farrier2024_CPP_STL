NAME = main
CFILE = main
CPP = clang++-20

CFLAGS1 = -stdlib=libc++ -O3 -std=c++26 -g3 -pthread  -fexperimental-library
CFLAGS2 = -Wall -Wextra -pedantic -fsanitize=address -Werror
CFLAGS = $(CFLAGS1) $(CFLAGS2)

LDLIBS = `pkg-config --libs fmt`

# get the top most directory
DIR = $(shell python -c "import os; print(sorted([x for x in os.listdir() if x[0]!='.'])[0])")

$(NAME): $(CFILE).o
	@$(CPP) -o out/$(NAME) out/$(CFILE).o $(CFLAGS) $(LDLIBS) 
	@./out/$(NAME)


$(CFILE).o: $(DIR)/$(CFILE).cpp
	@mkdir -p out
	@$(CPP) -o out/$(CFILE).o -c $(DIR)/$(CFILE).cpp $(CFLAGS)


