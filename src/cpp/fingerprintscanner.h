#ifndef _FINGERPRINTSCANNER_H_
#define _FINGERPRINTSCANNER_H_

#include "cppfprint.h"

namespace roomsec {

  /**
   * @brief Fingerprint scanning device.
   * 
   * This will read fingerprints from imaging devices, and store them in a
   * standard format.
   */
  class FingerprintScanner {
    public:

      virtual ~FingerprintScanner();

      /**
       * @brief Capture a fingerprint from a device.
       *
       * @return A fingerprint
       */
      std::string captureSerializedFingerprint();

    protected:

  };
}
#endif /*  _FINGERPRINTSCANNER_H_ */

