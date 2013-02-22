#include <iostream>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "gen-cpp/Authority.h"

#define authorityIP "192.168.0.194"
#define authorityPort 8080

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace std;

int
main (int argc, char *argv[])
{
  std::cout << "RoomSec Gateway\n";

  /* a simple repl */
  bool quit = false;
  string input;

  std::cout<<"('help' for commands)\n";

  while(!quit) {
    cout<<"Input?\n";
    getline(cin, input);

    if (!input.compare("quit")) {
      quit = true;
    }

    if(!input.compare("request"))  {
      boost::shared_ptr<TSocket> socket(new TSocket(authorityIP, authorityPort));
      boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      roomsec::interface::AuthorityClient client(protocol);
      transport->open();

      std::vector<roomsec::interface::CredentialSpec> credential_spec;
      std::string resource = "ca.mcmaster.itb.234";
      client.checkRequirements(credential_spec, resource);
      client.
      cout << "Obtained connection\n";
      cout << "resource: " << resource << "cred: ";
      transport->close();
    }

    if(!input.compare("help")) {
      cout << "request   - try to obtain permission";
    }
  }

  return 0;
}
