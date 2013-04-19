/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_UI_H_
#define _ROOMSEC_UI_H_

#include <boost/shared_ptr.hpp>
#include "uimessage.h"

namespace roomsec {

  class Display;
  class Buzzer;

  /**
   * @class Ui
   *
   * The Ui class is responsible for putting message to the lcd screen
   * of the gateway.  The Ui is a functor, and operates in a dedicated
   * thread.  Messages may be queued to be displayed by using one of
   * the public message functions.
   */
  class Ui {
  public:

    Ui(boost::shared_ptr<Display> display,
       boost::shared_ptr<Buzzer> buzzer);

    Ui(Ui const& ui) = delete;
    Ui(Ui && ui)     = delete;

    ~Ui();

    void operator()();

    /* Messaging Functions */

    /**
     * Display a UI message.  The object is passed by reference, and
     * is not copied.  Use this method, and preconstructed UiMessage
     * objects, when a standard set of frequently shown messages is
     * used.
     */
    int message(UiMessage const& message);

    /**
     * Copy the string, construct a UiMessage, and enqueue that
     * message to be displayed.
     */
    int message(UiMessage::Type t, std::string const& str);

    /* Passive State and Alarm Functions */

    int startAlarm(std::string const& message);
    int stopAlarm();

  private:

    class Impl;
    boost::shared_ptr<Impl> impl;
  };
}

#endif
