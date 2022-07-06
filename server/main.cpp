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

  BoardMessages messages{
    {"board1", {"hi", "hi2"}},
    {"board2", {"hi", "hi3"}}
  };

  while(true)
  {
    spdlog::info("Listening");
    nng::buffer rep_buf = rep_sock.recv(); 
    spdlog::info("Got message");

    if(rep_buf == "Hello")
    {
      auto serializedData = CerealSerializer::serialize(messages);
      rep_sock.send({serializedData.data(), serializedData.size()});
    }
    else 
    {
      NewMessage newMessage;
      CerealSerializer::deserialize(newMessage, rep_buf);
      messages.at(newMessage.first).push_back(newMessage.second);
      auto serializedData = CerealSerializer::serialize(messages);
      rep_sock.send({serializedData.data(), serializedData.size()});
      pub_sock.send({serializedData.data(), serializedData.size()});
    }
  }

// shoot off in different thread
//  while(not_finished)
//  sleep_for 1 seconds
//  set send to true
//

}
