CC = g++
CPPFLAGS = -I"C:/CS124/PG1/CS124_PA1/pcg-cpp-0.98/include" -fopenmp
CFLAGS = -Wall -Wextra -std=c++11 -O3
LDFLAGS =
# LDLIBS = -L"C:/CS124/PG1/CS124_PA1/pcg-cpp-0.98/lib" -lpcg_random

all: randmst.exe

randmst.exe: randmst.cc
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) randmst.cc -o randmst.exe

graphgen.exe: graphgen.cc
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) graphgen.cc -o graphgen.exe

startover.exe : startover.cc
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) startover.cc -o startover.exe

clean:
	rm -f randmst.exe
