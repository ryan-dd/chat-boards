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
      CerealSerializer::encodeCerealAndSend(rep_sock, messages);
      CerealSerializer::encodeCerealAndSend(pub_sock, messages);
    }
    else if(rep_buf == "New message")
    {
      // TODO Add message to data structure
      CerealSerializer::encodeCerealAndSend(pub_sock, messages);
    }
  }

// shoot off in different thread
//  while(not_finished)
//  sleep_for 1 seconds
//  set send to true
//

}
