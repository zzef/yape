CC = g++
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
IDIR = include
SDIR = src
OBJDIR = .
BDIR = .

DEPS = $(OBJDIR)/Circle.o $(OBJDIR)/utils.o $(OBJDIR)/Vector.o $(OBJDIR)/Polygon.o $(OBJDIR)/Body.o $(OBJDIR)/World.o $(OBJDIR)/Shape.o $(OBJDIR)/Display.o $(OBJDIR)/QuadTree.o 

$(BDIR)/yape: $(DEPS) $(OBJDIR)/main.o
	$(CC) -o $@ $^ $(LIBS)

$(OBJDIR)/main.o: $(SDIR)/main.cpp $(IDIR)/main.h $(DEPS)
	$(CC) -c -o $@ $<

$(OBJDIR)/World.o: $(SDIR)/World.cpp $(IDIR)/World.h $(IDIR)/Constraints.h $(OBJDIR)/Body.o $(OBJDIR)/utils.o $(IDIR)/Manifold.h $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Display.o: $(SDIR)/Display.cpp $(IDIR)/Display.h $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Body.o: $(SDIR)/Body.cpp $(IDIR)/Body.h $(IDIR)/Constraints.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Polygon.o: $(SDIR)/Polygon.cpp $(IDIR)/Polygon.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Circle.o: $(SDIR)/Circle.cpp $(IDIR)/Circle.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Vector.o: $(SDIR)/Vector.cpp $(IDIR)/Vector.h $(OBJDIR)/utils.o $(IDIR)/includes.h 
	$(CC) -c -o $@ $<

$(OBJDIR)/QuadTree.o: $(SDIR)/QuadTree.cpp $(IDIR)/QuadTree.h $(IDIR)/includes.h 
	$(CC) -c -o $@ $<

$(OBJDIR)/Shape.o: $(SDIR)/Shape.cpp $(IDIR)/Shape.h $(IDIR)/includes.h 
	$(CC) -c -o $@ $<

$(OBJDIR)/utils.o: $(SDIR)/utils.cpp $(IDIR)/utils.h $(IDIR)/includes.h
	$(CC) -c -o $@ $<

clean:
	rm -f $(OBJDIR)/*.o core
