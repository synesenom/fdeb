SRCDIR = src
CC = g++
CPPFLAGS = -c
UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
	LDFLAGS = -O3 -lglut -lGLU -lgl
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS = -O3 -framework GLUT -framework OpenGL
endif
DEPENDENCIES = main.o graph.o node.o edge.o meerkat_logger.o meerkat_file_manager.o meerkat_argument_manager.o meerkat_vector2.o
BINARY = fdeb

all: $(BINARY)

$(BINARY): $(DEPENDENCIES)
	$(CC) $(LDFLAGS) $^ -o $@ 

main.o: $(SRCDIR)/main.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

graph.o: $(SRCDIR)/graph.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

node.o: $(SRCDIR)/node.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

edge.o: $(SRCDIR)/edge.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

meerkat_vector2.o: $(SRCDIR)/meerkat_vector2.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

meerkat_logger.o: $(SRCDIR)/meerkat_logger.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

meerkat_file_manager.o: $(SRCDIR)/meerkat_file_manager.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

meerkat_argument_manager.o: $(SRCDIR)/meerkat_argument_manager.cpp
	$(CC) $(CPPFLAGS) $^ -o $@


clean:
	rm $(DEPENDENCIES)
