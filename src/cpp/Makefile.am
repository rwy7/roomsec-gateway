bin_PROGRAMS = roomsec-gateway

## RoomSec Gateway
roomsec_gateway_SOURCES = main.cpp \
	gateway.cpp

roomsec_gateway_CPPFLAGS = -O0 -I$(top_srcdir)/libauthorize \
	-I/usr/include/thrift -I$(top_srcdir)/wiringPi \
	$(DEBUG) -Wall -W -Wwrite-strings

roomsec_gateway_LDADD = $(top_srcdir)/libauthorize/libauthorize.a \
	-lthrift -lboost_system -lboost_thread
