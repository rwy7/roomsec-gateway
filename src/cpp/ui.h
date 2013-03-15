#ifndef _ROOMSEC_UI_H_
#define _ROOMSEC_UI_H_

namespace roomsec {

  enum UiMessageType : unsigned char;

  /**
   * An object which may be output for the user.  Messages are
   * translated into Ui Events by a Ui object.
   */
  class UiMessage {

  public:
    /**
     * Construct a message object, which may then be output to the
     * screen.
     */
    UiMessage(UiMessageType t = 0, std::string & message = 0);
  };
}

#endif
