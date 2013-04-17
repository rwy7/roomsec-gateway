#include "config.h"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "fingerprintscanner.h"
#include "fingerprintcontroller.h"

namespace roomsec {

  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.fingerprint");


  FingerprintController::FingerprintController(boost::shared_ptr<FingerprintScanner> const& scanner)
    : stop(false), scanner(scanner)
  {
    // Do Nothing
  }

  FingerprintController::~FingerprintController()
  {
    // Do Nothing
  }
 

  void FingerprintController::operator()()
  {
    LOG4CXX_DEBUG(logger, "Fingerprint Controller running");
    while(!this->stop) {
      boost::shared_ptr<Fingerprint> fingerprint(scanner->scanFingerprint());
      LOG4CXX_DEBUG(logger, "Fingerprint Scanned");
      fingerprintScanned(fingerprint);
    }
    LOG4CXX_DEBUG(logger, "Fingerprint Controller Stopping");
    // TODO: Clean up fingerprint scanner device.
    return;
  }
}
