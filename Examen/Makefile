CC=g++
LDFLAGS= -std=c++11 -pthread

LIB = MA1INFO_TpsReel_Examen2015

MAIN = main

TARGET_BIN = $(MAIN).x

TARGET_OBJ = $(LIB).o
TARGET_OBJ += $(MAIN).o

all: $(TARGET_BIN)


#MAIN
$(MAIN).x: $(MAIN).o $(LIB).o $
	$(CC) -o $(MAIN).x $(MAIN).o $(LIB).o $(LDFLAGS)

#MAIN
$(MAIN).o: $(MAIN).cpp
	$(CC) -o $(MAIN).o -c $(MAIN).cpp $(LDFLAGS)

#LIB
$(LIB).o: $(LIB).cpp 
	$(CC) -o $(LIB).o -c $(LIB).cpp $(LDFLAGS)


clean:
	rm -rf $(TARGET_BIN)
	rm -rf $(TARGET_OBJ)

mrproper: clean
	rm -rf $(TARGET_BIN)
	rm -rf $(TARGET_OBJ)