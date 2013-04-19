#include "config.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <log4cxx/logger.h>
#include <boost/shared_ptr.hpp>
#include "buzzer.h"
#include "display.h"
#include "uimessage.h"
#include "queue.h"
#include "ui.h"

namespace roomsec {

  /*
   * Logging
   */

  static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("roomsec.ui"));


  /*
   * Display Constants
   */

  const std::chrono::milliseconds beepTime(550);
  const std::chrono::milliseconds messageTime(1000);


  /*
   * Implementation Class
   */
  
  class Ui::Impl {
    friend class Ui;

  protected:
    Impl(boost::shared_ptr<Display> display,
	 boost::shared_ptr<Buzzer> buzzer);

    std::mutex mutex;
    std::string alarmMessage;
    bool alarmOn;
    boost::shared_ptr<Display> display;
    boost::shared_ptr<Buzzer> buzzer;
    Queue<UiMessage> messageQueue;
    bool stop;

    UiMessage popMessage();
    void show(Display::Color c, std::string const& m);
    void showBeep(Display::Color c, std::string const& m);
    void showBeep2(Display::Color c, std::string const& m);
    void clearScreen();
    void enterDefaultState();
  };


  Ui::Impl::Impl(boost::shared_ptr<Display> display,
		 boost::shared_ptr<Buzzer> buzzer)
  : alarmMessage(""),
    alarmOn(false),
    display(display),
    buzzer(buzzer),
    stop(false)
  {
  }

  /* Convienience Functions 
     These functions represent common operations on the impl class. */

  UiMessage Ui::Impl::popMessage() {
    return messageQueue.front_pop();
  }

  void Ui::Impl::show(Display::Color c, std::string const& m) {
    clearScreen();
    display->setBacklightColor(c);
    display->putStr(m);
    std::this_thread::sleep_for(messageTime);
    enterDefaultState();
    return;
  }

  void Ui::Impl::showBeep(Display::Color c, std::string const& m) {
    clearScreen();
    display->setBacklightColor(c);
    display->putStr(m);
    buzzer->on();
    std::this_thread::sleep_for(beepTime);
    buzzer->off();
    std::this_thread::sleep_for(messageTime - beepTime);
    enterDefaultState();
    return;
  }

  void Ui::Impl::showBeep2(Display::Color c, std::string const& m) {
    clearScreen();
    display->setBacklightColor(c);
    display->putStr(m);
    buzzer->on();
    std::this_thread::sleep_for(beepTime);
    buzzer->off();
    std::this_thread::sleep_for(beepTime/2);
    buzzer->on();
    std::this_thread::sleep_for(beepTime);
    buzzer->off();
    std::this_thread::sleep_for(messageTime - 2.5*beepTime);
    enterDefaultState();
    return;
  }

  void Ui::Impl::clearScreen() {
    display->clear();
    display->home();
    return;
  }

  void Ui::Impl::enterDefaultState() {
    std::unique_lock<std::mutex> lock(mutex);
    if (alarmOn) {
      buzzer->on();
      clearScreen();
      display->setBacklightColor(Display::red);
      display->putStr(alarmMessage);
    }
    else {
      clearScreen();
      buzzer->off();
      display->setBacklightColor(Display::blue);
    }
    return;
  }

  /*
   * Ctor/Dtor
   */

  Ui::Ui(boost::shared_ptr<Display> display,
	 boost::shared_ptr<Buzzer> buzzer)
    : impl(new Impl(display, buzzer))
  {
    impl->enterDefaultState();
  }


  Ui::~Ui()
  {
    impl->clearScreen();
    // TODO: Disable backlight, clear screen on destruction
  }


  void
  Ui::operator()() {
    LOG4CXX_DEBUG(logger, "Ui running");

    while(!impl->stop) {

      impl->enterDefaultState();

      LOG4CXX_TRACE(logger, "Waiting for message");
      UiMessage message = impl->popMessage();
      LOG4CXX_TRACE(logger, "Writing Message: " << message.getMessage());

      switch(message.getType()) {

      case UiMessage::Type::info:
	impl->show(Display::blue, message.getMessage());
	break;

      case UiMessage::Type::error:
	impl->showBeep2(Display::red, message.getMessage());
	break;

      case UiMessage::Type::success:
	impl->showBeep(Display::green, message.getMessage());
	break;

      case UiMessage::Type::prompt:
      case UiMessage::Type::warning:
	impl->showBeep(Display::blue, message.getMessage());
	break;

      case UiMessage::Type::alarm:
	/* Do nothing.
	   The steady state defaults have changed. */
	break;

      default:
	impl->display->putStr("Unkwn Msg Type!");
	impl->buzzer->on();
	std::this_thread::sleep_for(messageTime);
	impl->buzzer->off();
	impl->enterDefaultState();
      }
    }

    LOG4CXX_DEBUG(logger, "Ui stopping");
    return;
  }


  /** Messaging Functions */

  int
    Ui::message(UiMessage const& msg) {
      LOG4CXX_TRACE(logger, "Queueing message: " << msg.getMessage());
      int retVal = 0;
      impl->messageQueue.push(msg);
      return retVal;
    }

  int
  Ui::message(UiMessage::Type type, std::string const& str) {
    UiMessage msg(type, str);
    return message(msg);
  }


  int
  Ui::startAlarm(std::string const& message) {
    int retVal = 0;
    std::unique_lock<std::mutex> lock(impl->mutex);
    impl->alarmOn = true;
    impl->alarmMessage = message;
    LOG4CXX_DEBUG(logger, "Starting alarm: " << impl->alarmMessage);
    this->message(UiMessage::Type::alarm, "");
    return retVal;
  }


  int
  Ui::stopAlarm() {
    std::unique_lock<std::mutex> lock(impl->mutex);
    impl->alarmOn = false;
    impl->alarmMessage = "";
    impl->messageQueue.push(UiMessage(UiMessage::Type::alarm, ""));
    return 0;
  }

}
