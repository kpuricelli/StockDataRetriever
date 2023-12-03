# Makefile v1.2

# Variables
TARGET = StockDataRetriever

# kptodo build dir

# Compiler args
CC = g++
CXXFLAGS = -std=c++20 -Wall -Werror -Wextra -Wconversion -pedantic

# kptodo src

# Objs
OBJDIR=obj
OBJS = $(OBJDIR)/main.o
OBJS += $(OBJDIR)/StockTimeSeriesData.o
OBJS += $(OBJDIR)/SymbolContainer.o
OBJS += $(OBJDIR)/WebDataRetriever.o

# Libs
LDLIBS = -lcurl

# Rules
main: $(OBJS)
	$(CC) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LDLIBS)

$(OBJDIR)/main.o: main.cpp WebDataRetriever.h SymbolContainer.h
	$(CC) -c main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/StockTimeSeriesData.o: StockTimeSeriesData.cpp
	$(CC) -c StockTimeSeriesData.cpp -o $(OBJDIR)/StockTimeSeriesData.o

$(OBJDIR)/SymbolContainer.o: SymbolContainer.cpp StockTimeSeriesData.h
	$(CC) -c SymbolContainer.cpp -o $(OBJDIR)/SymbolContainer.o

$(OBJDIR)/WebDataRetriever.o: WebDataRetriever.cpp SymbolContainer.h
	$(CC) -c WebDataRetriever.cpp -o $(OBJDIR)/WebDataRetriever.o

clean:
	@rm -rf $(TARGET) $(OBJDIR) 2>/dev/null
