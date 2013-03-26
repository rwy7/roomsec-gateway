/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_STDGATEWAY_H_
#define _ROOMSEC_STDGATEWAY_H_

namespace roomsec {
  /*
   * Todo: complete this interface!
   */

  /**
   * The standard operations gateway controller.  This controller
   * should be used when the system is in standard autonomous
   * operations.  This gateway implements the neccesary high level
   * logic to ensure that all subsystem compose a secure and effective
   * gateway system.
   */
  class StdGateway {
  public:

    class Builder : Gateway::Builder<StdGateway::Builder> {
    };

    friend class Builder;


  };
  

  
}

#endif /* _ROOMSEC_STDGATEWAY_H_ */
