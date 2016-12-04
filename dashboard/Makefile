CC=g++
CFLAGS=-O3 -lpqxx -lpq 
DEPS =bancoDeDados.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dashboard: dashboard.o bancoDeDados.o
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o dashboard
