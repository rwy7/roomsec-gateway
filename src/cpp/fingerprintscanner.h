#ifndef _FINGERPRINTSCANNER_H_
#define _FINGERPRINTSCANNER_H_

#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>

/*  From <libfprint/fprint.h> */
struct fp_dscv_dev;
struct fp_dev;
struct fp_print_data;

namespace roomsec {

  /**
   * @brief A fingerprint.
   * Representative data of a fingerprint.  This is not just an image.  It
   * represents the fingerprint as a set of minutia.
   */
  class Fingerprint {
    public:
      friend class FingerprintScanner;
      ~Fingerprint();

      /**
       * @brief Get a string representation of a fingerprint.
       * This function does not memoize the value of the function, and is
       * costly co compute. There is no data or precision lost in this
       * transformation.
       *
       * @return A string representing the fingerprint.
       */
      std::string serialize();

      /**
       * @brief Get a fingerprint from its string representation.
       *
       * @param serialFP A serialized string fingerprint.
       */
      Fingerprint(std::string serialFP);

    protected:

      /**
       * @brief Create a fingerprint from a raw fp_print_data from an imaging
       * device.
       *
       * @param fprint A list of minutiae on the fingerprint.
       */
      Fingerprint(struct fp_print_data * fprint);

    private:
      struct fp_print_data *fpData;
  };

  /**
   * @brief Fingerprint scanning device.
   * 
   * This will read fingerprints from imaging devices, and store them in a
   * standard format.
   */
  class FingerprintScanner {
    public:
      friend class FingerprintScannerFactory;

      ~FingerprintScanner();

      /**
       * @brief Scan for a fingerprint.
       * This function will block until a fingerprint has successfully been
       * obtained.  If the scan is poor, it may take several scans of a finger
       * before it is happy.
       * @return A fingerprint as taken from the device.
       */
      boost::shared_ptr<Fingerprint> scanFingerprint();

    protected:

      /**
       * @brief Create a fingerprint scanner from a device.
       *
       * @param fpDev A fingerprint device.
       */
      FingerprintScanner(struct fp_dev *fpDev);

    private:
      struct fp_dev *fpDev;
      static log4cxx::LoggerPtr logger;
  };

  /**
   * @brief Creates initialized FingerprintScanners.
   * This will initialize the hardware on any supported imaging device, and
   * return a FingerprintScanner handler which will obtain fingerprints from
   * the device.
   */
  class FingerprintScannerFactory {
    public:
      FingerprintScannerFactory();
      ~FingerprintScannerFactory();

      /**
       * @brief Scan for devices.
       * This function does not need to be called when this class in
       * instantiated.  If there is a change in the hardware (i.e. device
       * disconnected), this function should be called to check on the current
       * status of the hardware.  It would be advisable to call this function
       * before creating any new FingerprintScanners.
       */
      void scanDevices();

      /**
       * @brief Gets the current count of known devices.
       * This will only know about the devices at the time of the last scan.
       *
       * @return Device count.
       */
      int getDeviceCount();

      /**
       * @brief Create a fingerprint scanner.
       * A fingerprint scanner will be capable of using the hardware to obtain
       * fingerprints.  You must specify which device number of the scanner you
       * wish to use.  Only one fingerprint scanner should be created for each
       * device.
       *
       * @param dev The device number.
       *
       * @return A scanner for fingerprints from the device.
       */
      boost::shared_ptr< FingerprintScanner > getFingerprintScanner(int dev);

    protected:

    private:
      /*  A list of the discovered devices */
      struct fp_dscv_dev ** discoveredDevs;
      /*  A count of the discovered devices */
      int devCount;
      static log4cxx::LoggerPtr logger;
  };
}

#endif /*  _FINGERPRINTSCANNER_H_ */

