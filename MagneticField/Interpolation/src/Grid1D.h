#ifndef Grid1D_H
#define Grid1D_H
#include <cmath>
#include <algorithm>

class Grid1D {
public:

  typedef double Scalar;

  Grid1D() {}

  Grid1D( Scalar lower, Scalar upper, int nodes) : 
    lower_(lower), upper_(upper), nodes_(nodes) {
    stepinv_ =  (nodes-1)/(upper - lower);
  }


  Scalar step() const {return 1./step_;}
  Scalar lower() const {return lower_;}
  Scalar upper() const {return upper_;}
  int nodes() const {return nodes_;}
  int cells() const {return nodes()-1;}

  Scalar node( int i) const { return i*step() + lower();}

  bool inRange(int i) const {
    return i>=0 && i<cells();
  }

  // return index and fractional part...
  int index(Scalar a, Scalar & f) const {
    Scalar b;
    f = modf((a-lower())*stepinv_, &b);
    return b;
  }


  Scalar closestNode( Scalar a) const {
    Scalar b = (a-lower())/step();
    Scalar c = floor(b);
    Scalar tmp = (b-c < 0.5) ? std::max(c,0.) : std::min(c+1.,static_cast<Scalar>(nodes()-1));
    return tmp*step()+lower();
  }

  /// returns valid index, or -1 if the value is outside range +/- one cell.
  int index( Scalar a) const {
    int ind = static_cast<int>((a-lower())/step());
    // FIXME: this causes an exception to be thrown later. Should be tested
    // more carefully before release
    //  if (ind < -1 || ind > cells()) {
    //     std::cout << "**** ind = " << ind << " cells: " << cells() << std::endl;
    //    return -1;
    //  }
    return std::max(0, std::min( cells()-1, ind));
  }

 
private:

  Scalar stepinv_;
  Scalar lower_;
  Scalar upper_;
  int    nodes_;

};

#endif
