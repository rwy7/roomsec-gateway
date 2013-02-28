#ifndef _DOORSTATESENSOR_H_
#define _DOORSTATESENSOR_H_

/*  TODO: implement door sensor */
namespace roomsec {
  enum DoorState {open, closed};

  class DoorStateSensor {
    public:
      DoorStateSensor(int pin);
      getDoorState()
    private:
      DISALLOW_COPY_AND_ASSIGN(DoorStateSensor);

  };
}

#endif /* _DOORSTATESENSOR */

