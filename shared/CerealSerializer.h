#ifndef CEREAL_SERIALIZER_H
#define CEREAL_SERIALIZER_H

#include "Data.h"

#include <nngpp/buffer.h>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/archives/portable_binary.hpp>

namespace CerealSerializer
{
  template <typename T>
  std::string serialize(T& toSend, OpcodeType opcode)
  {
    std::stringstream ss;
    ss << opcode;

    {
      cereal::PortableBinaryOutputArchive oarchive( ss );
      oarchive( toSend ); // archive only writes to stringstream in destructor
    }
    return ss.str();
  }

  template <typename T>
  void deserialize(T& decodingResult, void* data, size_t size)
  {
    std::string outstring{reinterpret_cast<char*>(data), size};

    std::stringstream ss{outstring};
    {
      cereal::PortableBinaryInputArchive iarchive(ss);
      iarchive(decodingResult);
    }
  }
}

#endif // CEREAL_SERIALIZER_H
