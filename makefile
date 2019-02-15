CXX=gcc
#CXXFLAGS=-Isrc -Wall -O3 -c
CXXFLAGS=-Isrc -Wall -g -c
OBJDIR=lib
OBJ=$(OBJDIR)/bytebuffer.o $(OBJDIR)/main.o
EXE=bytebuffer

.PHONY: all clean

all: $(EXE)

bytebuffer: $(OBJ)
	$(CXX) -o $@ $^

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(EXE)
