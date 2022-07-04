#ifndef CEREAL_SERIALIZER_H
#define CEREAL_SERIALIZER_H

#include "Data.h"

#include "nngpp/socket_view.h"
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>

namespace CerealSerializer
{
  void encodeCerealAndSend(nng::socket_view sock, BoardMessages toSend);
  void decodeCereal(BoardMessages& decodedMap, nng::buffer buffer);
}

#endif // CEREAL_SERIALIZER_H
