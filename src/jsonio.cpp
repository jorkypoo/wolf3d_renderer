#include "jsonio.h"

json io::read_file(std::string path)
{
  std::ifstream file(path);
  if (!file.is_open())
    return nullptr;

  if (!json::accept(file))
    return nullptr;

  file.seekg(0, std::ios::beg);

  json data = json::parse(file);
  if (data.is_discarded())
    return nullptr;

  return data;
}
