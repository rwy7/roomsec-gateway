#include "config.h"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "actor.h"
#include "fingerprintscanner.h"
#include "fingerprintcontroller.h"

namespace roomsec {

  log4cxx::LoggerPtr
  FingerprintController::logger = log4cxx::Logger::getLogger("roomsec.fingerprint");


  FingerprintController::FingerprintController(boost::shared_ptr<FingerprintScanner> const& scanner)
    : stop(false), scanner(scanner)
  {
    // Do Nothing
  }

  FingerprintController::~FingerprintController()
  {
    // Do Nothing
  }
 

  void FingerprintController::run()
  {
    LOG4CXX_DEBUG(logger, "Fingerprint Controller running");
    while(!this->stop) {
      boost::shared_ptr<Fingerprint> fingerprint(scanner->scanFingerprint());
      LOG4CXX_DEBUG(logger, "Fingerprint Scanned");
      fingerprintScanned(fingerprint);
      boost::this_thread::interruption_point();
    }
    return;
  }
}
