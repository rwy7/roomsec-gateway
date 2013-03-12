#include <iostream>
#include <string>

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

/*  Program Options */
#include "boost/program_options.hpp"

#include "wiringPi/wiringPi.h"
#include "gen-cpp/Authority.h"

#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "authorityadapter.h"
#include "thriftfingerprintauthnadapter.h"

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfprint/fprint.h"
}

namespace roomsec {

#define authorityIP "192.168.0.194"
#define authorityPort 9090
#define fingerprintAuthnIp "192.168.0.172"
#define fingerprintAuthnPort 8080

  int start_repl(AuthorityAdapter& authAdapter,
      FingerprintAuthnAdapter& fingerprintAuthAdapter);

  struct fp_dscv_dev *discover_device(struct fp_dscv_dev **discovered_devs)
  {
    struct fp_dscv_dev *ddev = discovered_devs[0];
    struct fp_driver *drv;
    if (!ddev)
      return NULL;

    drv = fp_dscv_dev_get_driver(ddev);
    printf("Found device claimed by %s driver\n", fp_driver_get_full_name(drv));
    return ddev;
  }

  struct fp_print_data* get_fprint(struct fp_dev *dev) {
    struct fp_print_data *enrolled_print = NULL;
    int r;

    printf("You will need to successfully scan your finger %d times to "
        "complete the process.\n", fp_dev_get_nr_enroll_stages(dev));

    do {
      struct fp_img *img = NULL;
      printf("\nScan your finger now.\n");

      r = fp_enroll_finger_img(dev, &enrolled_print, &img);
      if (img) {
        fp_img_free(img);
      }
      if (r < 0) {
        printf("Enroll failed with error %d\n", r);
        return NULL;
      }

      switch (r) {
        case FP_ENROLL_COMPLETE:
          printf("Enroll complete!\n");
          break;
        case FP_ENROLL_FAIL:
          printf("Enroll failed, something wen't wrong :(\n");
          return NULL;
        case FP_ENROLL_PASS:
          printf("Enroll stage passed. Yay!\n");
          break;
        case FP_ENROLL_RETRY:
          printf("Didn't quite catch that. Please try again.\n");
          break;
        case FP_ENROLL_RETRY_TOO_SHORT:
          printf("Your swipe was too short, please try again.\n");
          break;
        case FP_ENROLL_RETRY_CENTER_FINGER:
          printf("Didn't catch that, please center your finger on the "
              "sensor and try again.\n");
          break;
        case FP_ENROLL_RETRY_REMOVE_FINGER:
          printf("Scan failed, please remove your finger and then try "
              "again.\n");
          break;
      }
    } while (r != FP_ENROLL_COMPLETE);

    if (!enrolled_print) {
      fprintf(stderr, "Enroll complete but no print?\n");
      return NULL;
    }

    printf(" completed!\n\n");
    return enrolled_print;
  }

}

namespace po = boost::program_options;

int main (int argc, char *argv[]) {
  /* fingerprint authority information */
  //int authorityPort;
  std::string authorityAddr;

  /*  Authority information */
  //int fpauthnPort;
  std::string fpauthnAddr;

  /*  Handle Program Options */
  po::options_description desc("Allowed Options");
  desc.add_options()
    ("help", "produce help message")
    ("fpauthn",  po::value< std::string >(), "Set the fingerprint authority server address");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("fpauthn")) {

  }

  roomsec::ThriftAuthorityAdapter
    authzAdapter(authorityIP, authorityPort);

  roomsec::ThriftFingerprintAuthnAdapter
    fingerprintAuthnAdapter(fingerprintAuthnIp, fingerprintAuthnPort);

  roomsec::start_repl(authzAdapter, fingerprintAuthnAdapter);
  return 0;
}

namespace roomsec {

  namespace thrift = ::apache::thrift;
  namespace transport = ::apache::thrift::transport;
  namespace protocol = ::apache::thrift::protocol;

  /* a simple repl */
  int start_repl(AuthorityAdapter& authAdapter,
      FingerprintAuthnAdapter& fingerprintAuthAdapter) {

    std::cout << "RoomSec Gateway\n";

    bool quit = false;
    iface::Credential credential;
    std::string input;

    std::cout << "('help' for commands)\n";

    while(!quit) {
      std::cout << "Input?\n";
      std::getline(std::cin, input);

      if (!input.compare("quit")) {
        quit = true;
      }

      if (!input.compare("authn")) {
        std::cout << "Authenticating\n";

        /** 
         * TODO: Scan fingerprints 
         * NOTE: COPY PASTA NOT OUR CODE HERE 
         */

        int r = fp_init();
        if (r < 0) {
          fprintf(stderr, "Failed to initialize libfprint\n");
        }
        else {
          struct fp_dscv_dev *ddev;
          struct fp_dscv_dev **discovered_devs;
          struct fp_dev *dev;

          fp_set_debug(3);
          discovered_devs = fp_discover_devs();

          if (!discovered_devs) {
            fprintf(stderr, "Could not discover devices\n");
          }

          ddev = discover_device(discovered_devs);
          if (!ddev) {
            fprintf(stderr, "No devices detected.\n");
          }

          dev = fp_dev_open(ddev);
          fp_dscv_devs_free(discovered_devs);

          if (!dev) {
            fprintf(stderr, "Could not open device.\n");
          }

          else {
            fp_print_data* data = get_fprint(dev);

            unsigned char* fprint = 0;
            size_t fprint_size = 0;
            fprint_size = fp_print_data_get_data(data, &fprint);

            fp_print_data_free(data);
            fp_dev_close(dev);

            fingerprintAuthAdapter.authenticate
              (credential, std::string(fprint, fprint + fprint_size));

            std::cout << "Recieved: " << credential.token 
              << " User: " << credential.userid << "\n";
          }

          fp_exit();
        }
      }

      if(!input.compare("authz")) {
        std::cout << "Not yet implemented\n";
      }

      if(!input.compare("check"))  {
        std::string resource;
        std::cout << "resource: ";
        std::cin >> resource;

        std::cout << "checking\n";
        std::vector<iface::CredentialSpec> requiredCreds;
        authAdapter.checkRequirements(requiredCreds, resource);

        std::cout << "requires credentials:\n";
        for(std::vector<iface::CredentialSpec>::iterator it =  requiredCreds.begin(); 
            it != requiredCreds.end(); ++it) {
          std::cout << "  " << "provider: " << it->provider
            <<" mechanism: " << it->mechanism << "\n";
        }
      }

      if(!input.compare("help")) {
        std::cout << "check     - check credential requirements for resource\n"
          << "authn     - authenticate with fingerprint\n"
          << "authz     - authorize \n";
      }
    }

    return 0;
  }
}

