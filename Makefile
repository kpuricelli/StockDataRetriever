# Makefile v1.1

# kptodo build dir

# Variables
TARGET = Stockxxx
CXXFLAGS = -std=c++20 -Wall -Werror -Wextra -Wconversion -pedantic
CC = g++
LDLIBS = -lcurl
OBJS = main.o StockTimeSeriesData.o SymbolContainer.o WebDataRetriever.o

# Rules
main: $(OBJS)
	$(CC) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LDLIBS)

main.o: main.cpp WebDataRetriever.h SymbolContainer.h
	$(CC) -c main.cpp

StockTimeSeriesData.o: StockTimeSeriesData.cpp
	$(CC) -c StockTimeSeriesData.cpp

SymbolContainer.o: SymbolContainer.cpp StockTimeSeriesData.h
	$(CC) -c SymbolContainer.cpp

WebDataRetriever.o: WebDataRetriever.cpp SymbolContainer.h
	$(CC) -c WebDataRetriever.cpp

clean:
	rm $(TARGET) $(OBJS)
