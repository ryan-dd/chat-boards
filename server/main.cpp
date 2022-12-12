#include "nngpp/socket.h"
#include <chrono>
#include <nngpp/nngpp.h>
#include <nngpp/protocol/rep0.h>
#include <nngpp/protocol/pub0.h>
#include <ratio>
#include <spdlog/spdlog.h>
#include <thread>

#include "Data.h"
#include "CerealSerializer.h"

int main()
{
  nng::socket rep_sock{nng::rep::open()}; 
  rep_sock.listen( "tcp://localhost:8000" ); 

  nng::socket pub_sock{nng::pub::open()};
  pub_sock.listen( "tcp://localhost:8001" ); 

  BoardMessages messages{};
  OpcodeType opcode{};
  spdlog::info("Listening");

  while (true)
  {
    nng::buffer rep_buf{rep_sock.recv()}; 
    auto [dataPtr, dataSize] = Data::deserializeOpcode(opcode, rep_buf.data(), rep_buf.size());

    switch(opcode)
    {
      case initialHelloOpcode:
      {
        spdlog::info("Received hello");
        auto serializedData{CerealSerializer::serialize(messages, boardMessagesOpcode)};
        rep_sock.send({serializedData.data(), serializedData.size()});
        break;
      }

      case newMessageOpCode: 
      {
        spdlog::info("Received new message");
        NewMessage newMessage{};
        CerealSerializer::deserialize(newMessage, dataPtr, dataSize);
        messages.at(newMessage.first).push_back(newMessage.second);
        auto serializedData{CerealSerializer::serialize(messages, boardMessagesOpcode)};
        rep_sock.send("");
        pub_sock.send({serializedData.data(), serializedData.size()});
        break;
      }

      case newBoardOpcode:
      {
        spdlog::info("Received new board");
        NewBoard newBoard{};
        CerealSerializer::deserialize(newBoard, dataPtr, dataSize);
        messages.insert({newBoard, {}});
        auto serializedData{CerealSerializer::serialize(messages, boardMessagesOpcode)};
        rep_sock.send("");
        pub_sock.send({serializedData.data(), serializedData.size()});
        break;
      }

      default:
        spdlog::warn("Unknown opcode: {}", opcode);
        break;
    }
  }
}
