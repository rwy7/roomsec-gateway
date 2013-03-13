#include <iostream>

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "wiringPi/wiringPi.h"
#include "gen-cpp/Authority.h"

#include "authorityadapter.h"
#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "thriftfingerprintauthnadapter.h"

#include "replgateway.h"

#define authorityIP "192.168.0.194"
#define authorityPort 9090

#define fingerprintAuthnIp "172.17.144.152"
#define fingerprintAuthnPort 8080

int main (int argc, char *argv[]) {
  boost::shared_ptr<roomsec::ThriftAuthorityAdapter>
    authzAdapter(new roomsec::ThriftAuthorityAdapter(authorityIP, authorityPort));

  boost::shared_ptr<roomsec::ThriftFingerprintAuthnAdapter>
    authnAdapter(new roomsec::ThriftFingerprintAuthnAdapter(fingerprintAuthnIp, fingerprintAuthnPort));

  roomsec::ReplGateway::Builder builder;

  builder
    .authorityAdapter(authzAdapter)
    .fingerprintAuthnAdapter(authnAdapter)
    .build()
    ->start();

  return 0;
}
