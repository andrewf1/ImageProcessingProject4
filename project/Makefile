#all:
	#cd ../iptools; make
#	g++ -o bin/test test.cpp  -L ../lib -l iptools 
#	g++ -o bin/test2 test2.cpp  -L ../lib -l iptools -l mpeg 

	# source files.

BIN_DIR=bin/

EXEC = iptool

# opencv library
OPENCVLIBS = -L/apps/opencv/4.0.1/lib64 -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -Wl,-rpath=/apps/opencv/4.0.1/lib64

# include opencv directory
OPENCVINCLUDE = -I/apps/opencv/4.0.1/include/opencv4


all: $(EXEC)

iptools:
	(cd ../iptools; make)

clean:
	$(RM) $(BIN_DIR)$(EXEC); make all

DO_EXEC = g++ -std=c++11 $@.cpp $(OPENCVLIBS) $(OPENCVINCLUDE) -o $(BIN_DIR)$@ -L ../lib -l iptools

#------ now compile
$(EXEC):   		iptools
				$(DO_EXEC)
