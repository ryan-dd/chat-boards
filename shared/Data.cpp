#include "Data.h"

std::pair<void*, size_t> Data::deserializeOpcode(OpcodeType& opcode, void* data, size_t size)
{
  memcpy(&opcode, data, sizeof(OpcodeType));
  void* dataPtr{static_cast<char*>(data) + sizeof(OpcodeType)};
  return {dataPtr, size - sizeof(OpcodeType)};
}
