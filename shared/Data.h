#ifndef DATA_H
#define DATA_H

#include <cstddef>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

using OpcodeType = uint8_t;

namespace Data
{
  std::pair<void*, size_t> deserializeOpcode(OpcodeType& opcode, void* data, size_t size);
};

constexpr OpcodeType initialHelloOpcode{0};

constexpr OpcodeType boardMessagesOpcode{1};
using BoardMessages = std::unordered_map<std::string, std::vector<std::string>>;

constexpr OpcodeType newMessageOpCode{2};
using NewMessage = std::pair<std::string, std::string>;

constexpr OpcodeType newBoardOpcode{3};
using NewBoard = std::string;

#endif
