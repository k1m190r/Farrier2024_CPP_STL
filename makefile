NAME = main
CFILE = main
CPP = clang++-20

# -stdlib=libc++ : for std::print
CFLAGS0 = -std=c++26 -g3 -pthread -stdlib=libc++ -fexperimental-library 

# optimizations use the SIMD/AVX
CFLAGS1 = -O3 -march=native -mavx2 

# safety 
CFLAGS2 = -Wall -Wextra -Werror -pedantic -fsanitize=address 

CFLAGS = $(CFLAGS0) $(CFLAGS1) $(CFLAGS2)

# -static-libstdc++ : to prevent ASAN: alloc-dealloc-mismatch on exception
LDLIBS = -static-libstdc++ #`pkg-config --libs fmt`

# get the top most directory
DIR = $(shell python -c "import os; print(sorted([x for x in os.listdir() if x[0]!='.'])[0])")

$(NAME): $(CFILE).o
	@$(CPP) -o out/$(NAME) out/$(CFILE).o $(CFLAGS) $(LDLIBS) 
	@./out/$(NAME)


$(CFILE).o: $(DIR)/$(CFILE).cpp
	@mkdir -p out
	@$(CPP) -o out/$(CFILE).o -c $(DIR)/$(CFILE).cpp $(CFLAGS)


