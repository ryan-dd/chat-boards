#include "CerealSerializer.h"

namespace CerealSerializer
{
  void encodeCerealAndSend(nng::socket_view sock, BoardMessages toSend)
  {
    std::stringstream ss;
    {
      cereal::PortableBinaryOutputArchive oarchive( ss );
      oarchive( toSend );
    }
    auto stringToSend = ss.str();
    sock.send({stringToSend.c_str(), stringToSend.size()});
  }

  void decodeCereal(BoardMessages& decodedMap, nng::buffer buffer)
  {
    std::string outstring{(char*)buffer.data(), buffer.size()};
    std::stringstream ss2{outstring};
    {
      cereal::PortableBinaryInputArchive iarchive(ss2);
      iarchive(decodedMap);
    }
  }
}
