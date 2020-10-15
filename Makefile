CC = g++
<<<<<<< HEAD
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2
IDIR = include
EXT_INCLUDE_PATHS = -I SDL2/include
EXT_LIBRARY_PATHS = -L SDL2/lib
SDIR = src
OBJDIR = .
BDIR = .
FLAGS = $(LIBS) $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

DEPS = $(OBJDIR)/Circle.o $(OBJDIR)/utils.o $(OBJDIR)/Vector.o $(OBJDIR)/Polygon.o $(OBJDIR)/Body.o $(OBJDIR)/World.o $(OBJDIR)/Shape.o $(OBJDIR)/Display.o

$(BDIR)/engine: $(DEPS) $(OBJDIR)/main.o
	$(CC)  $^ $(LIBS) -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/main.o: $(SDIR)/main.cpp $(IDIR)/main.h $(DEPS)
	$(CC) -c $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/World.o: $(SDIR)/World.cpp $(IDIR)/World.h $(IDIR)/Joints.h $(OBJDIR)/Body.o $(OBJDIR)/utils.o $(IDIR)/Manifold.h $(IDIR)/includes.h
	$(CC) -c  $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/Display.o: $(SDIR)/Display.cpp $(IDIR)/Display.h $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c  $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/Body.o: $(SDIR)/Body.cpp $(IDIR)/Body.h $(IDIR)/Joints.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c  $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/Polygon.o: $(SDIR)/Polygon.cpp $(IDIR)/Polygon.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c  $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/Circle.o: $(SDIR)/Circle.cpp $(IDIR)/Circle.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/Vector.o: $(SDIR)/Vector.cpp $(IDIR)/Vector.h $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/Shape.o: $(SDIR)/Shape.cpp $(IDIR)/Shape.h $(IDIR)/includes.h
	$(CC) -c  $< -o $@ $(EXT_INCLUDE_PATHS) $(EXT_LIBRARY_PATHS)

$(OBJDIR)/utils.o: $(SDIR)/utils.cpp $(IDIR)/utils.h $(IDIR)/includes.h
	$(CC) -c $< -o $@
=======
LIBS = -lSDL2
IDIR = include
SDIR = src
OBJDIR = .
BDIR = .

DEPS = $(OBJDIR)/Circle.o $(OBJDIR)/utils.o $(OBJDIR)/Vector.o $(OBJDIR)/Polygon.o $(OBJDIR)/Body.o $(OBJDIR)/World.o $(OBJDIR)/Shape.o $(OBJDIR)/Display.o 

$(BDIR)/engine: $(DEPS) $(OBJDIR)/main.o
	$(CC) -o $@ $^ $(LIBS)

$(OBJDIR)/main.o: $(SDIR)/main.cpp $(IDIR)/main.h $(DEPS)
	$(CC) -c -o $@ $<

$(OBJDIR)/World.o: $(SDIR)/World.cpp $(IDIR)/World.h $(IDIR)/Joints.h $(OBJDIR)/Body.o $(OBJDIR)/utils.o $(IDIR)/Manifold.h $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Display.o: $(SDIR)/Display.cpp $(IDIR)/Display.h $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Body.o: $(SDIR)/Body.cpp $(IDIR)/Body.h $(IDIR)/Joints.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Polygon.o: $(SDIR)/Polygon.cpp $(IDIR)/Polygon.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Circle.o: $(SDIR)/Circle.cpp $(IDIR)/Circle.h $(OBJDIR)/Shape.o $(OBJDIR)/utils.o $(IDIR)/includes.h
	$(CC) -c -o $@ $<

$(OBJDIR)/Vector.o: $(SDIR)/Vector.cpp $(IDIR)/Vector.h $(OBJDIR)/utils.o $(IDIR)/includes.h 
	$(CC) -c -o $@ $<

$(OBJDIR)/Shape.o: $(SDIR)/Shape.cpp $(IDIR)/Shape.h $(IDIR)/includes.h 
	$(CC) -c -o $@ $<

$(OBJDIR)/utils.o: $(SDIR)/utils.cpp $(IDIR)/utils.h $(IDIR)/includes.h
	$(CC) -c -o $@ $<
>>>>>>> ff48d433afb346640b1cdb3f3018f41592b056a4

clean:
	rm -f $(OBJDIR)/*.o core
