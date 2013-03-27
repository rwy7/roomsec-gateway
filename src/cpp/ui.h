/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_UI_H_
#define _ROOMSEC_UI_H_

#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>

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
     * translate to different Ui events (IE sounds, animations), when a
     * message is output with a Ui object.
     */
    enum class Type : unsigned char {
      alert = 0, warning, request, error
    };

    /**
     * Construct a message object, which may then be output to the
     * screen.
     */
    UiMessage(Type type, boost::shared_ptr<const std::string> message);
    UiMessage(UiMessage const& that);

    Type getType() const;
    boost::shared_ptr<const std::string> getMessage() const;

  private:
    static log4cxx::LoggerPtr logger;

    Type type;
    boost::shared_ptr<const std::string> message;
  };


  class Ui {
  public:
    Ui(boost::shared_ptr<Display> display, boost::shared_ptr<Buzzer> buzzer);
    virtual ~Ui() {};

    int message(UiMessage const& that);
    int message(UiMessage::Type t, std::string const& str);
    int message(boost::shared_ptr<const UiMessage> message);
    int message(UiMessage::Type t);
    int message(std::string const& str);

  private:
    static log4cxx::LoggerPtr logger;

    boost::shared_ptr<Display> display;
    boost::shared_ptr<Buzzer> buzzer;
  };
}

#endif
