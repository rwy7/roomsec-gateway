/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_UI_H_
#define _ROOMSEC_UI_H_

#include <thread>
#include <mutex>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>

#include "actor.h"
#include "queue.h"

namespace roomsec {

  class Display;
  class Buzzer;

  /**
   * An object which may be output for the user.  Messages are
   * translated into Ui Events by a Ui object.
   */
  class UiMessage {

  public:
    /**
     * Represents the type of message.  Different message types
     * translate to different Ui events (IE sounds, colors), when a
     * message is output to the UI system with a UiMessage object.
     */
    enum class Type : unsigned char {
      info = 0, success, warning, error, prompt, alarm
    };

    /**
     * Construct a message object, which may then be output to the
     * screen.
     */
    UiMessage(Type type, std::string  message);
    UiMessage(UiMessage const& that);

    Type getType() const;
    std::string getMessage() const;

  private:
    static log4cxx::LoggerPtr logger;

    Type type;
    std::string message;
  };


  class Ui : public Actor {
  public:

    Ui(boost::shared_ptr<Display> display, boost::shared_ptr<Buzzer> buzzer);
    virtual void run();

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
