#ifndef _ROOMSEC_UIMESSAGETYPE_H_
#define _ROOMSEC_UIMESSAGETYPE_H_

namespace roomsec {

  /**
   * Represents the type of message.  Different message types
   * translate to different Ui events (IE sounds, animations), when a
   * message is output with a Ui object.
   */
  enum class UiMessageType : unsigned char {
    alert = 0, warning, request, error
  };
}

#endif
