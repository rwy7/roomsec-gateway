/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_MCP3008BLOCKSENSOR_H_
#define _ROOMSEC_MCP3008BLOCKSENSOR_H_

#include "blocksensor.h"

namespace roomsec{

  class MCP3008BlockSensor : public BlockSensor {

  protected:
    unsigned int MCPPin; //0-7
    int iValue, mValue;
    bool DEBUG;

  public:
    MCP3008BlockSensor(unsigned int pin, bool debug = false);
    virtual ~MCP3008BlockSensor();
    int getSensorValue();

  };

}

#endif /* _ROOMSEC_MCP3008BLOCKSENSOR_H_ */
