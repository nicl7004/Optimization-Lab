##
##

CXX	=g++
CXXFLAGS= -m64 -Os -o2 -fopenmp -funroll-loops   ##get rid of static. Add -os -0s -fopenmp -funrol
goals: judge
	echo "Done"

filter: FilterMain.cpp Filter.cpp cs1300bmp.cc
	$(CXX) $(CXXFLAGS) -o filter FilterMain.cpp Filter.cpp cs1300bmp.cc
##
## Parameters for the test run
##
FILTERS = gauss.filter vline.filter hline.filter emboss.filter
IMAGES = boats.bmp blocks-small.bmp
TRIALS = 1 2 3 4

judge: filter
	#-./Judge -p ./filter -i boats.bmp
	-./Judge -p ./filter -i blocks-small.bmp

clean:
	-rm filter
	-rm filtered-*.bmp
