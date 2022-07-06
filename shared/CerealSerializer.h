#ifndef CEREAL_SERIALIZER_H
#define CEREAL_SERIALIZER_H

#include "Data.h"

#include "nngpp/socket_view.h"
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/archives/portable_binary.hpp>

namespace CerealSerializer
{

  template <typename T>
  void encodeCerealAndSend(nng::socket_view sock, T& toSend)
  {
    std::stringstream ss;
    {
      cereal::PortableBinaryOutputArchive oarchive( ss );
      oarchive( toSend );
    }
    auto stringToSend = ss.str();
    sock.send({stringToSend.c_str(), stringToSend.size()});
  }

  template <typename T>
  void decodeCereal(T& decodingResult, nng::view buffer)
  {
    std::string outstring{(char*)buffer.data(), buffer.size()};
    std::stringstream ss{outstring};
    {
      cereal::PortableBinaryInputArchive iarchive(ss);
      iarchive(decodingResult);
    }
  }
}

#endif // CEREAL_SERIALIZER_H
