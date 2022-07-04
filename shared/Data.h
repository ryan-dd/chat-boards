#ifndef DATA_H
#define DATA_H

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

using BoardMessages = std::unordered_map<std::string, std::vector<std::string>>;
using NewMessage = std::pair<std::string, std::string>;

#endif
