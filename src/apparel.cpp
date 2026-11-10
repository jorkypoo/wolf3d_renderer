#include "item.h"

///////////////////
///// helmets /////
///////////////////

Helmet::Helmet(std::string json_path, std::string json_id) {
  // read from json path etc etc
  // once data has been read, set all the values through provided interfaces

  set_default_item_params("helmet", 10, 5);
  Defense d = Defense();
  update_apparel_defense_params(d);
}

Item* Helmet::clone() {
  return new Helmet(*this);
}


Apparel::Apparel() {
  // nothing needs to be done! wow
  // however, this constructor will be called when initiaising from
  // derived classes such as
  ;
}

Item* Apparel::clone() {
  return new Apparel(*this);
}


