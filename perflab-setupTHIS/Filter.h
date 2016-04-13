//-*-c++-*-
#ifndef _Filter_h_
#define _Filter_h_

using namespace std;

class Filter {




public:
  int divisor;
  Filter(int _dim);
  int dim;
  int get(int r, int c);
  void set(int r, int c, int value);
  int *data;
  int getDivisor();
  void setDivisor(int value);

  int getSize();
};

#endif
