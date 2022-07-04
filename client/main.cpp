#include "GUI.h"
#include "nngpp/buffer.h"
#include "nngpp/socket.h"
#include "nngpp/socket_view.h"
#include <cstdio>
#include <cstring>
#include <spdlog/spdlog.h>

#include <nngpp/nngpp.h>
#include <nngpp/socket_view.h>
#include <nngpp/protocol/sub0.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/rep0.h>
#include <unordered_map>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <sstream>

#include "CerealSerializer.h"
#include "Data.h"

int main()
{
  GUI::start();
}
