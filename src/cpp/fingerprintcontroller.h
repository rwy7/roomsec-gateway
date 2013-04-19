/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_FINGERPRINTCONTROLLER_H_
#define _ROOMSEC_FINGERPRINTCONTROLLER_H_

#include <boost/shared_ptr.hpp>

namespace roomsec {

  class FingerprintScanner;
  class AuthorityAdapter;
  class FingerprintAuthnAdapter;
  class Lock;
  class Ui;
  class DoorStateSensor;
  
  /**
   * This class controls the fingerprint scanning and detection
   * hardware subsystems.  It is intended to be run under it's own,
   * dedicated thread.  This class also controls authentication and
   * authorization subsystems.
   */
  class FingerprintController {
  public:
    FingerprintController(std::string name,
			  boost::shared_ptr<FingerprintScanner> const& scanner,
			  boost::shared_ptr<AuthorityAdapter> authorityAdapter,
			  boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter,
			  boost::shared_ptr<Lock> lock,
			  boost::shared_ptr<DoorStateSensor> doorStateSensor,
			  boost::shared_ptr<Ui> ui);

    FingerprintController(FingerprintController const& source) = delete;
    FingerprintController(FingerprintController && source) = delete;

    ~FingerprintController();

    virtual void operator()();

  private:
    std::string name;
    boost::shared_ptr<FingerprintScanner> scanner;
    boost::shared_ptr<AuthorityAdapter> authorityAdapter;
    boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter;
    boost::shared_ptr<Lock> lock;
    boost::shared_ptr<DoorStateSensor> doorStateSensor;
    boost::shared_ptr<Ui> ui;
    bool stop;
  };
}

#endif /* _ROOMSEC_FINGERPRINTCONTROLLER_H_ */
