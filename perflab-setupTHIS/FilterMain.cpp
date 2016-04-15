#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include "Filter.h"

using namespace std;

#include "rtdsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int
main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

struct Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div; //this is the value of divisor now
    filter -> setDivisor(div);

    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  }
}


double
applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{

  long long cycStart, cycStop;

  cycStart = rdtscll();

  output -> width = input -> width;
  output -> height = input -> height;

  //moved the below line out of the for loops, noticed small increase in performance
  int getsize = filter -> getSize();
  int height = (input -> height) -1;
  int width = (input ->width)-1;
  int divisor = filter ->getDivisor();

	//got rid of plane loop and pasted code three times.
    for(int col = 1; col < (width); col = col + 1) {
     for(int row = 1; row < (height); row = row + 1) {
          int value = 0;
          for (int j = 0; j < getsize; j+=1) {
	           for (int i = 0; i < getsize; i+=1) {
               value = value +  input -> color[col + j - 1][0][row + i - 1] * filter -> data[i * getsize + j];

    	  }
	}
	switch(divisor){
		case 16 : //got this fromm the .filter files
			value = value >> 4;
		default :
			break;
		}
	
	value = (value < 0) ? 0:value; //single line if-statments 
	value = (value > 255) ? 255:value;
	output -> color[col][0][row] = value;
      }
      for(int row = 1; row < (height); row = row + 1) {
           int value = 0;
          for (int j = 0; j < getsize; j+=1) {
	           for (int i = 0; i < getsize; i+=1) {
               value = value +  input -> color[col + j - 1][1][row + i - 1] * filter -> data[i * getsize + j];

    	  }
	}
	switch(divisor){
		case 16 : //got this fromm the .filter files
			value = value >> 4;
		default :
			break;
		}
	
	value = (value < 0) ? 0:value; //single line if-statments 
	value = (value > 255) ? 255:value;
	output -> color[col][1][row] = value;
      }
      for(int row = 1; row < (height); row = row + 1) {
			int value = 0;
          for (int j = 0; j < getsize; j+=1) {
	           for (int i = 0; i < getsize; i+=1) {
               value = value +  input -> color[col + j - 1][2][row + i - 1] * filter -> data[i * getsize + j];

    	  }
	}
	switch(divisor){
		case 16 : //got this fromm the .filter files
			value = value >> 4;
		default :
			break;
		}
	
	value = (value < 0) ? 0:value; //single line if-statments 
	value = (value > 255) ? 255:value;
	output -> color[col][2][row] = value;
      }
  }

  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
	  diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}
