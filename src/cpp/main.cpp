#include <iostream>

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "wiringPi/wiringPi.h"
#include "gen-cpp/Authority.h"

#define authorityIP "192.168.0.194"
#define authorityPort 8080

namespace roomsec {
  int start_repl();
}

int main (int argc, char *argv[]) {
  roomsec::start_repl();
  return 0;
}

namespace roomsec {

  namespace thrift = ::apache::thrift;
  namespace transport = ::apache::thrift::transport;
  namespace protocol = ::apache::thrift::protocol;

  int start_repl(void);


  int start_repl() {
    std::cout << "RoomSec Gateway\n";

    /* a simple repl */
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
	/* Open a new request */
	boost::shared_ptr<transport::TSocket> socket(new transport::TSocket(authorityIP, authorityPort));
	boost::shared_ptr<transport::TTransport> transport(new transport::TBufferedTransport(socket));
	boost::shared_ptr<protocol::TProtocol> protocol(new protocol::TBinaryProtocol(transport));

	iface::AuthorityClient client(protocol);
	transport->open();

	std::vector<iface::CredentialSpec> requiredCreds;
	std::string resource = "ca.mcmaster.itb.234";

	client.checkRequirements(requiredCreds, resource);

	std::cout << "Obtained connection\n";
	std::cout << "resource: " << resource << "cred: ";
	transport->close();
      }

      if(!input.compare("help")) {
	std::cout << "request   - try to obtain permission";
      }
    }

    return 0;
  }
}
