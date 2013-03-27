#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "wiringPi/wiringPi.h"
#include "doorstatesensor.h"
#include <iostream>

/*  Thde pin the buzzer is connected to (for broadcom numbering, corresponds to pin 11 */
int pin = 18;

class DoorStateSensorEnvironment : public ::testing::Environment {
  public:
    DoorStateSensorEnvironment() {}
    ~DoorStateSensorEnvironment() {}
    void SetUp() {
      ASSERT_NE(wiringPiSetupGpio(), -1);
    }

    void TearDown() {

    }
};

::testing::Environment* const buzzerEnv =
::testing::AddGlobalTestEnvironment(new DoorStateSensorEnvironment());

namespace roomsec {

  using namespace ::testing;

  TEST(DoorStateSensorTest, StateReadTest) {
    DoorStateSensor sen (pin);
    sen.getDoorState();
    std::clog << "Door State: " << (int) sen.getDoorState();
    return;
  }

  char change = 0;
  void test (void) {
    change ++;
    return;
  }
  TEST(DoorStateSensorTest, InterruptTest) {
    DoorStateSensor sen (pin);

    sen.waitForChange(&test);
    while (change == 0) {
      delay(50);
    }

  }
}
