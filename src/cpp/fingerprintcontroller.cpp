#include "config.h"
#include <thread>
#include <exception>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "gen-cpp/authorize_types.h"
#include "authorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "doorstatesensor.h"
#include "ui.h"
#include "uimessage.h"
#include "lock.h"
#include "fingerprintscanner.h"
#include "fingerprintcontroller.h"

namespace roomsec {

  /*
   * Logging
   */

  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.fingerprint");
  static log4cxx::LoggerPtr netLogger = log4cxx::Logger::getLogger("roomsec.net");

  /*
   * Ctor / Dtor
   */

  FingerprintController::FingerprintController(std::string name,
					       boost::shared_ptr<FingerprintScanner> const& scanner,
					       boost::shared_ptr<AuthorityAdapter> authorityAdapter,
					       boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter,
					       boost::shared_ptr<Lock> lock,
					       boost::shared_ptr<DoorStateSensor> doorStateSensor,
					       boost::shared_ptr<Ui> ui)
    : name(name),
      scanner(scanner),
      authorityAdapter(authorityAdapter),
      fingerprintAuthnAdapter(fingerprintAuthnAdapter),
    lock(lock),
    doorStateSensor(doorStateSensor),
    ui(ui),
    stop(false)
  {
    lock->setState(LockState::locked);
  }


  FingerprintController::~FingerprintController()
  {
    lock->setState(LockState::locked);
  }


  /*
   * Running Code
   */

  void FingerprintController::operator()()
  {
    LOG4CXX_DEBUG(logger, "Fingerprint Controller running");

    // lock the door
    lock->setState(LockState::locked);

    while(!this->stop) {

      // Scan for fingerprint.  This is a blocking/waiting operation.
      boost::shared_ptr<Fingerprint> fingerprint(scanner->scanFingerprint());

      LOG4CXX_DEBUG(logger, "Fingerprint Scanned");
      ui->message(UiMessage::Type::info, "Fingerprint Scanned");

      iface::Credential credential;

      // TODO: Handle Failure more robustly
      try {

	fingerprintAuthnAdapter->authenticate(credential, fingerprint->serialize());
  
	if (credential.token == "" &&
	    credential.userid == "") {
	  ui->message(UiMessage::Type::error, "User unknown");
	  LOG4CXX_INFO(netLogger, "roomsec." << name << ".userauth.fail");
	}

	else {

	  iface::AuthorizationRequest authzRequest;
	  authzRequest.resource = name;
	  authzRequest.credentials = std::vector<iface::Credential>();
	  authzRequest.credentials.push_back(credential);

	  iface::AuthorizationReply::type authzReply = authorityAdapter->authorize(authzRequest);

	  ui->message(UiMessage::Type::info, "User: "+ credential.userid);
	  LOG4CXX_INFO(netLogger, "roomsec." << name << ".userauthn.pass." << credential.userid);

	  if(authzReply == iface::AuthorizationReply::grant) {

	    ui->message(UiMessage::Type::success, "Access granted");
	    LOG4CXX_INFO(netLogger, "roomsec." << name << ".userauthz.pass." << credential.userid);
	    lock->setState(LockState::unlocked);

	    /* Hold the door unlocked until it opens or 5 seconds have
	       ellapsed.  Poll the door state every 10 milliseconds. */

	    std::chrono::system_clock::time_point
	      unlockTime = std::chrono::system_clock::now();

	    const std::chrono::seconds unlockDuration(5);
	    std::this_thread::sleep_for(std::chrono::milliseconds(50));

	    while(doorStateSensor->getDoorState() == DoorState::closed ||
		  std::chrono::system_clock::now() - unlockTime < unlockDuration)
	      {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	      }
	
	    lock->setState(LockState::locked);
	  }

	  else {
	    ui->message(UiMessage::Type::error, "Access denied");
	    LOG4CXX_INFO(netLogger, "roomsec." << name << ".userauthz.fail." << credential.userid);
	  }
	}
      }
      catch(std::exception e) {
	ui->message(UiMessage::Type::error, "Network Error");
      }
    }
    LOG4CXX_DEBUG(logger, "Fingerprint Controller Stopping");
    // TODO: Close fingerprint scanner device.
    return;
  }
}
