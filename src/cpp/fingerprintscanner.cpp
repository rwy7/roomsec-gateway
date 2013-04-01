#include "config.h"
#include <log4cxx/logger.h>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "libfprint/fprint.h"
#include "fingerprintscanner.h"

namespace roomsec {
  log4cxx::LoggerPtr
    FingerprintScanner::logger(log4cxx::Logger::getLogger("roomsec.hardware.fingerprintscanner"));
  log4cxx::LoggerPtr
    FingerprintScannerFactory::logger(log4cxx::Logger::getLogger("roomsec.hardware.fingerprintscanner"));

  FingerprintScannerFactory::FingerprintScannerFactory() : discoveredDevs(NULL), devCount(0) {
    this->scanDevices();
  }

  FingerprintScannerFactory::~FingerprintScannerFactory() {
    LOG4CXX_INFO(logger, "Closing factory");
  }

  void FingerprintScannerFactory::scanDevices() {
    this->discoveredDevs = fp_discover_devs();
    for (this->devCount = 0; this->discoveredDevs[devCount] != NULL ;devCount++) {}
    LOG4CXX_INFO(logger, "Found " << this->devCount << " devices");
  }

  int FingerprintScannerFactory::getDeviceCount() {
    return this->devCount;
  }

  boost::shared_ptr<FingerprintScanner>
    FingerprintScannerFactory::getFingerprintScanner(int dev) {
      LOG4CXX_INFO(logger, "Creating a scanner on dev" << dev);

      if (dev > devCount || dev < 1) {
        LOG4CXX_INFO(logger, "Dev number " << dev << " outside of devcount range");
        return boost::shared_ptr <FingerprintScanner> ();
      }

      struct fp_dscv_dev *ddev = discoveredDevs [dev - 1];

      struct fp_dev *fpDev = fp_dev_open(ddev);
      if (!fpDev) {
        LOG4CXX_INFO(logger, "Could not open device");
        return (boost::shared_ptr <FingerprintScanner> ());
      }

      if (!fp_dev_supports_imaging(fpDev)) {
        LOG4CXX_INFO(logger, "Fingerprint device does not support imaging");
        fp_dev_close(fpDev);
        return boost::shared_ptr< FingerprintScanner > (); 
      }

      boost::shared_ptr< FingerprintScanner > fpScanner (new FingerprintScanner(fpDev));
      return fpScanner;
    }

  /* ******************
   * FingerprintScanner
   * ******************/

  FingerprintScanner::FingerprintScanner(struct fp_dev *fpDev) :
    asyncComplete(true), result(0),fpDev(fpDev) {

    }

  FingerprintScanner::~FingerprintScanner() {
    /*  The device must be closed properly!  */
    fp_dev_close(this->fpDev);
  }

  boost::shared_ptr<Fingerprint> FingerprintScanner::scanFingerprint() {
    struct fp_print_data *enrolled_print = NULL;
    LOG4CXX_INFO(logger, "Scanning for fingerprint in " <<
        fp_dev_get_nr_enroll_stages(this->fpDev) << "stages.");

    struct fp_img *img;
    int r = fp_enroll_finger_img(this->fpDev, &enrolled_print, &img);

    do {
      switch (r) {
        case FP_ENROLL_COMPLETE:
          LOG4CXX_INFO(logger, "Fingerprint found and scanned");
          break;
        case FP_ENROLL_FAIL:
          LOG4CXX_INFO(logger, "Fingerprint detection failed");
          return boost::shared_ptr<Fingerprint> ();
        case FP_ENROLL_PASS:
          LOG4CXX_INFO(logger, "One enrollment stage passed");
          break;
        case FP_ENROLL_RETRY:
          LOG4CXX_INFO(logger, "Fingerprint scan must retry");
          break;
        case FP_ENROLL_RETRY_TOO_SHORT:
          LOG4CXX_INFO(logger, "Fingerprint swipe too short");
          break;
        case FP_ENROLL_RETRY_CENTER_FINGER:
          LOG4CXX_INFO(logger, "Fingerprint was not centered, must retry");
          break;
        case FP_ENROLL_RETRY_REMOVE_FINGER:
          LOG4CXX_INFO(logger, "Must remove finger and try again");
          break;
      }
    } while (r != FP_ENROLL_COMPLETE);


    boost::shared_ptr<Fingerprint> fprint (new Fingerprint(enrolled_print));

    return fprint;
  }

  void FingerprintScanner::setResult(int result) {
    this->result = result;
    if (result < 0) {
      LOG4CXX_INFO(logger, "Async enroll failed. Device disconnected?");
    }
    return;
  }

  void FingerprintScanner::setFinished(bool result) {
    this->asyncComplete = result;
    return;
  }

  void FingerprintScanner::setAsyncFP(struct fp_print_data *data) {
    this->asyncFP = boost::shared_ptr<Fingerprint> (new Fingerprint(data));
    return;
  }

  struct fp_dev *FingerprintScanner::getFPDev () {
      boost::unique_lock< boost::mutex > lock (this->mutex);
    return this->fpDev;
  }

  bool FingerprintScanner::isFinished() {
      boost::unique_lock< boost::mutex > lock (this->mutex);
      return this->asyncComplete;
  }

  int FingerprintScanner::getResult() {
      boost::unique_lock< boost::mutex > lock (this->mutex);
      return this->asyncComplete;
  }

  extern "C" {
    void enrollCB(struct fp_dev *dev, int result,
        struct fp_print_data *print, struct fp_img *img, void *userData) {
      FingerprintScanner *fpscan = (FingerprintScanner *)userData;
      boost::unique_lock< boost::mutex > lock (fpscan->mutex);
      fpscan->setFinished(true);
      fpscan->setResult(result);
      fpscan->setAsyncFP(print);
      return;
    }

    void enrollStopCB(struct fp_dev *dev, void *userData) {
      FingerprintScanner *fpscan = (FingerprintScanner *)userData;
      boost::unique_lock< boost::mutex > lock (fpscan->mutex);
      fpscan->setFinished(true);
      fpscan->setResult(-1);
      fpscan->setAsyncFP(NULL);
      return;
    }
  }

  int FingerprintScanner::startAsyncScan() {
    LOG4CXX_INFO(logger, "Async. Scanning for fingerprint in " <<
        fp_dev_get_nr_enroll_stages(this->fpDev) << "stages.");

    struct fp_dev *dev = this->getFPDev();
    int r = fp_async_enroll_start(dev, &::roomsec::enrollCB, (void *)this);
    return r;
  } 


  int FingerprintScanner::stopAsyncScan() {
    LOG4CXX_INFO(logger, "Async. Scanning for fingerprint in " <<
        fp_dev_get_nr_enroll_stages(this->fpDev) << "stages.");

    struct fp_dev *dev = this->getFPDev();
    int r = fp_async_enroll_stop(dev, &::roomsec::enrollStopCB, (void *)this);
    return r;
  }


  Fingerprint::Fingerprint(struct fp_print_data *fpData) :
    fpData(fpData) {
    }

  Fingerprint::~Fingerprint() {
    fp_print_data_free(this->fpData);
  }

  std::string Fingerprint::serialize() {
    unsigned char *charString;
    size_t charStringSize = 0;
    charStringSize = fp_print_data_get_data(this->fpData, &charString);
    return (std::string(charString, charString + charStringSize));
  }
}

