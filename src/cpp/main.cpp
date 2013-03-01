#include <iostream>

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "wiringPi/wiringPi.h"
#include "gen-cpp/Authority.h"

#include "thriftauthorityadapter.h"
#include "authorityadapter.h"

#define authorityIP "192.168.0.194"
#define authorityPort 8080

namespace roomsec {
  int start_repl(AuthorityAdapter& authAdapter);
}

int main (int argc, char *argv[]) {
  roomsec::ThriftAuthorityAdapter authAdapter(authorityIP, authorityPort);
  roomsec::start_repl(authAdapter);
  return 0;
}

namespace roomsec {

  namespace thrift = ::apache::thrift;
  namespace transport = ::apache::thrift::transport;
  namespace protocol = ::apache::thrift::protocol;

  /* a simple repl */
  int start_repl(AuthorityAdapter& authAdapter) {
    std::cout << "RoomSec Gateway\n";

    bool quit = false;
    std::string input;

    std::cout << "('help' for commands)\n";

    while(!quit) {
      std::cout << "Input?\n";
      std::getline(std::cin, input);

      if (!input.compare("quit")) {
	quit = true;
      }

      if(!input.compare("request"))  {

	std::vector<iface::CredentialSpec> requiredCreds;
	std::string resource = "ca.mcmaster.itb.234";

	authAdapter.checkRequirements(requiredCreds, resource);

	std::cout << "Obtained connection\n";
	std::cout << "resource: " << resource << "cred: ";
      }

      if(!input.compare("help")) {
	std::cout << "request   - try to obtain permission";
      }
    }

    return 0;
  }
}
