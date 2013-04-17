/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_UIMESSAGE_H_
#define _ROOMSEC_UIMESSAGE_H_

namespace roomsec {

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

    Type type;
    std::string message;
  };
}

#endif /* _ROOMSEC_UIMESSAGE_H_ */
