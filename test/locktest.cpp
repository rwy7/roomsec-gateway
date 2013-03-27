#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <boost/shared_ptr.hpp>
#include <wiringPi/wiringPi.h>
#include "lock.h"
#include "ioexpander.h"

/*  Pin number on the ioexpander */
static uint8_t pin = 0x08;
/*  Bank of the ioexpander to use */
static roomsec::IOExpander::GPIO gpio = roomsec::IOExpander::GPIOB;
static boost::shared_ptr<roomsec::IOExpander> expander (new roomsec::IOExpander());

class LockEnvironment : public ::testing::Environment {
  public:
    LockEnvironment() {}
    ~LockEnvironment() {}
    void SetUp() {
      ASSERT_NE(wiringPiSetup(), -1);
      expander->initialize(0x20);
    }

    void TearDown() {

    }
};

::testing::Environment* const lockEnv = ::testing::AddGlobalTestEnvironment(new LockEnvironment());

namespace roomsec {

  using namespace ::testing;

  TEST(LockTest, LockTest) {
    Lock lock(expander, gpio, pin);
    lock.setState(Lock::LockState::locked);
    delay(100);
    lock.setState(Lock::LockState::unlocked);
  }
}
