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
  nng::socket rep_sock = nng::rep::open(); 
  rep_sock.listen( "tcp://localhost:8000" ); 

  nng::socket pub_sock = nng::pub::open();
  pub_sock.listen( "tcp://localhost:8001" ); 

  BoardMessages messages{};

  OpcodeType opcode{};

  spdlog::info("Listening");
  while(true)
  {
    nng::buffer rep_buf = rep_sock.recv(); 
    auto [dataPtr, dataSize] = Data::getMessageOpcode(opcode, rep_buf.data(), rep_buf.size());

    if(opcode == initialHelloOpcode)
    {
      auto serializedData = CerealSerializer::serialize(messages, boardMessagesOpcode);
      rep_sock.send({serializedData.data(), serializedData.size()});
    }
    else if(opcode == newMessageOpCode) 
    {
      spdlog::info("Got New message");
      NewMessage newMessage{};
      CerealSerializer::deserialize(newMessage, dataPtr, dataSize);
      messages.at(newMessage.first).push_back(newMessage.second);
      auto serializedData = CerealSerializer::serialize(messages, boardMessagesOpcode);
      rep_sock.send("");
      pub_sock.send({serializedData.data(), serializedData.size()});
    }
    else if(opcode == newBoardOpcode)
    {
      spdlog::info("Got New Board");
      NewBoard newBoard{};
      CerealSerializer::deserialize(newBoard, dataPtr, dataSize);
      messages.insert({newBoard, {}});
      auto serializedData = CerealSerializer::serialize(messages, boardMessagesOpcode);
      rep_sock.send("");
      pub_sock.send({serializedData.data(), serializedData.size()});
    }
    else
    {
      spdlog::warn("Unknown opcode: {}", opcode);
    }
  }
}
