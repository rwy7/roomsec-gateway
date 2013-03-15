/*  Main Header File */

#ifndef _MAIN_H_
#define _MAIN_H_

 /**
 * @mainpage RoomSec Gateway
 *
 * @section  intro_sec Introduction
 *
 * 
 * This is the RoomSec Gateway.  It represents the software that runs at each
 * gateway.
 */


/****************************/
/*  Namespace Documentation */
/****************************/

/**
 * @namespace roomsec
 *
 * Base roomsec interface.  All roomsec code goes into this namespace.
 */

/**
 * @namespace apache
 * 
 * Base Apache namespace.  Apache is a open source organization.  Code
 * generated by Apache Thrift depends on this namespace.
 */


/**
 * @namespace apache::thrift
 *
 * Base Thrift namespace.  This namespace contains base server and client
 * functionality which our code depends on.  It is used by all code generated
 * by Apache Thrift.
 */


/**
 * @namespace apache::thrift::transport
 *
 * Transportation protocols for client and server connections.  Contains
 * different transportation methods, such as buffered transports, and sockets.
 * It also contains all SSL handling code.
 */


/**
 * @namespace apache::thrift::protocol Protocols which specify the encoding of
 * transferred data.  Contains options such as HTTP, binary, or JSon
 * transportation. 
 */

#endif /* _MAIN_H_ */

