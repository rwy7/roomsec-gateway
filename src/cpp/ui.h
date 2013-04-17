/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_UI_H_
#define _ROOMSEC_UI_H_

#include <thread>
#include <mutex>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "uimessage.h"
#include "queue.h"

namespace roomsec {

  class Display;
  class Buzzer;

  /**
   * @class Ui
   *
   * The Ui class requires exclusive ownership of hardware devices.
   * Because of this, it's copy constructor is disallowed.  A move
   * constructor is defined, however, allowing ownership to transfer.
   * This is neccessary when operator() is invoked by std::thread,
   * which will moves the Ui class.
   */
  class Ui {
  public:

    Ui(boost::shared_ptr<Display> display,
       boost::shared_ptr<Buzzer> buzzer);

    Ui(Ui const& ui) = delete;
    Ui(Ui && ui) = delete;

    void operator()();

    /* Messaging Functions */

    int message(UiMessage const& message);
    int message(UiMessage::Type t, std::string const& str);

    /* Passive State Functions */

    int startAlarm(std::string const& message);
    int stopAlarm();
    void enterDefaultState();

  private:
    static log4cxx::LoggerPtr logger;

    std::mutex mutex;
    std::string alarmMessage;
    bool alarmOn;

    boost::shared_ptr<Display> display;
    boost::shared_ptr<Buzzer> buzzer;
    Queue<UiMessage> messageQueue;
    bool stop;
  };
}

#endif
