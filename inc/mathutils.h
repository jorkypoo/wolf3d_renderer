#ifndef __MATH_UTILS__
#define __MATH_UTILS__

#include <math.h>
#include <algorithm>
using std::max;
using std::min;

typedef struct v2 v2;
struct v2 { double x; double y; };

typedef struct v2i v2i;
struct v2i { int x; int y; };

#define SQ(a) (a*a)
#define CUBE(a) (a*a*a)
#define MS_TO_NS(n) (n * 1000000)


// #define max(a, b) ({ typeof(a) __a = a; typeof(b) __b = b; __a > __b ? __a : __b; })
// #define min(a, b) ({ typeof(a) __a = a; typeof(b) __b = b; __a < __b ? __a : __b; })

#define sign(a) ({ typeof(a) __a = a; __a > 0 ? 1 : -1; })

// dot product
inline double dot(v2 _v1, v2 _v2) {
  return (_v1.x * _v2.x) + (_v1.y * _v2.y);
}

inline double length(v2 _v) {
  return sqrtf(dot(_v, _v));
}

inline v2 normalise(v2 _mth_v) {
  return (v2) { _mth_v.x / length(_mth_v), _mth_v.y / length(_mth_v) };
}

double length(v2);
double dot(v2, v2);
v2     normalise(v2);

#endif
