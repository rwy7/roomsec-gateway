/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_FINGERPRINTCONTROLLER_H_
#define _ROOMSEC_FINGERPRINTCONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>

namespace roomsec {

  class FingerprintScanner;
  class Fingerprint;

  /**
   * This class controls the fingerprint scanning and detection
   * hardware subsystems.  As an actor, it is intended to be run under
   * it's own, dedicated thread.
   */
  class FingerprintController {
  public:
    FingerprintController(boost::shared_ptr<FingerprintScanner> const& scanner);
    ~FingerprintController();

    virtual void operator()();

    boost::signal<void (boost::shared_ptr<Fingerprint> fingerprint)> fingerprintScanned;

  private:
    bool stop;
    boost::shared_ptr<FingerprintScanner> scanner;
  };
}

#endif /* _ROOMSEC_FINGERPRINTCONTROLLER_H_ */
