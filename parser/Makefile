CC=g++
CFLAGS=-O3 -lpqxx -lpq 
DEPS =bancoDeDados.h categorie.h parser.h product.h review.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parser: parser.o bancoDeDados.o categorie.o parser.o product.o review.o main.o
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o parser
