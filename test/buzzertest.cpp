#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "wiringPi/wiringPi.h"
#include "buzzer.h"
#include <iostream>

/*  The pin the buzzer is connected to (for broadcom numbering, corresponds to
 *  pin 11 */
static int buzzerPin = 17;

class BuzzerEnvironment : public ::testing::Environment {
  public:
    BuzzerEnvironment() {}
    ~BuzzerEnvironment() {}
    void SetUp() {
      ASSERT_NE(wiringPiSetupGpio(), -1);
    }

    void TearDown() {

    }
};

::testing::Environment* const buzzerEnv = ::testing::AddGlobalTestEnvironment(new BuzzerEnvironment());

namespace roomsec {

  using namespace ::testing;

  TEST(BuzzerTest, SoundTest) {
cout << "Setting up on pin: " << ::buzzerPin;
    Buzzer buzz (::buzzerPin);
    buzz.on();
    delay(200);
    buzz.off();
  }
}
