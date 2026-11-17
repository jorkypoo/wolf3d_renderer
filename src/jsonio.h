#ifndef _JSONIO_
#define _JSONIO_

#include <fstream>
#include <string>
#include "thirdparty/json.hpp"
using namespace nlohmann;

namespace io {

json read_file(std::string path);

}

#endif
