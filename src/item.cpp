#include "../inc/item.h"

Item::Item(std::string n, uint w, uint v, uint a) {
  name = n;
  weight = w;
  value = v;
  appraisal = a;
  appraised = 0;
}


