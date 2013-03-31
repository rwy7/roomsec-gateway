/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_FINGERPRINTCONTROLLER_H_
#define _ROOMSEC_FINGERPRINTCONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "actor.h"

namespace roomsec {

  /**
   * This class controls the fingerprint scanning and detection
   * hardware subsystems.  As an actor, it is intended to be run under
   * it's own, dedicated thread.
   */
  class FingerprintController : public Actor {
  public:
    FingerprintController(boost::shared_ptr<FingerprintScanner> const& scanner);
    ~FingerprintController();
    virtual void run();

  private:
    bool stop;
    boost::shared_ptr<FingerprintScanner> scanner;
    static log4cxx::LoggerPtr logger;
    
  };
}

#endif /* _ROOMSEC_FINGERPRINTCONTROLLER_H_ */
