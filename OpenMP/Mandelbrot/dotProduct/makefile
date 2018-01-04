CFLAGS = -g -O3 -fopenmp -fsanitize=address

all: dotProduct

dotProduct: dotProduct.cpp walltime.h
	g++  $(CFLAGS) $< -o $@

clean:
	rm -rf dotProduct
