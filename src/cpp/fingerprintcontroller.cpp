#include "config.h"
#include <thread>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>

#include "authorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "ui.h"
#include "uimessage.h"
#include "gen-cpp/authorize_types.h"
// #include 
#include "fingerprintscanner.h"
#include "fingerprintcontroller.h"

namespace roomsec {

  /*
   * Logging
   */

  // TODO: Repair netlogger
  const std::string gatewayId = "gateway1";
  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.fingerprint");
  static log4cxx::LoggerPtr netLogger = log4cxx::Logger::getLogger("roomsec.net");


  /*
   * Ctor / Dtor
   */

  FingerprintController::FingerprintController(boost::shared_ptr<FingerprintScanner> const& scanner,
					       boost::shared_ptr<AuthorityAdapter> authorityAdapter,
					       boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter,
					       boost::shared_ptr<Ui> ui)
    : scanner(scanner),
      authorityAdapter(authorityAdapter),
      fingerprintAuthnAdapter(fingerprintAuthnAdapter),
      ui(ui),
      stop(false)
  {
    // Do Nothing
  }


  FingerprintController::~FingerprintController()
  {
    // Do Nothing
  }


  /*
   * Running Code
   */

  void FingerprintController::operator()()
  {
    LOG4CXX_DEBUG(logger, "Fingerprint Controller running");
    while(!this->stop) {

      // Scan for fingerprint.  This is a blocking/waiting operation.
      boost::shared_ptr<Fingerprint> fingerprint(scanner->scanFingerprint());

      LOG4CXX_DEBUG(logger, "Fingerprint Scanned");
      ui->message(UiMessage::Type::info, "Fingerprint Scanned");

      iface::Credential credential;

      // Todo: Handle Failure more robustly
      fingerprintAuthnAdapter->authenticate(credential, fingerprint->serialize());

      if (credential.token == "" &&
	  credential.userid == "") {
	ui->message(UiMessage::Type::error, "User Unrecognized");
	LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".userauth.fail");
      }

      else {
	ui->message(UiMessage::Type::warning, "User: "+ credential.userid);

	LOG4CXX_INFO(netLogger,
		     "roomsec." << gatewayId << 
		     ".userauth.pass." << credential.userid);
      }

    }
    LOG4CXX_DEBUG(logger, "Fingerprint Controller Stopping");

    // TODO: Close fingerprint scanner device.
    return;
  }
}
