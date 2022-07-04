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
  BoardMessages toEncode{
    {"key1", {"hi1","hi2"}},
    {"key2", {"hi1","hi2"}}
  };

	nng::socket rep_sock = nng::rep::open();
	rep_sock.listen( "tcp://localhost:8000" );

	nng::socket req_sock = nng::req::open();
	req_sock.dial( "tcp://localhost:8000" );

  CerealSerializer::encodeCerealAndSend(req_sock, toEncode);

  BoardMessages toDecode;
	nng::buffer rep_buf = rep_sock.recv();
  CerealSerializer::decodeCereal(toDecode, rep_buf);

  spdlog::info(toDecode.at("key1")[0]);
}
/* try { */
  
/*   // Client */
/* 	nng::socket req_sock = nng::req::open(); */
/* 	req_sock.dial( "tcp://localhost:8000" ); */
/* 	req_sock.send("Hello"); */
/* 	nng::buffer req_buf = req_sock.recv(); */

/*   nng::socket sub_socket = nng::sub::open(); */
/*   sub_socket.set_opt( NNG_OPT_SUB_SUBSCRIBE, {} ); */
/*   sub_socket.dial("tcp://localhost:8001"); */ 
/*   auto msg = sub_socket.recv(); */
/*   // Decode response and populate chat_board data */
/*   // */
/* } */
/* catch( const nng::exception& e ) { */
/* 	// who() is the name of the nng function that produced the error */
/* 	// what() is a description of the error code */
/*   spdlog::error( "{}: {}\n", e.who(), e.what() ); */
/* 	return 1; */
/* } */  


/* GUI::start(); */

/* } */
