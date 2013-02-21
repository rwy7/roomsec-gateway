bin_PROGRAMS = roomsec-gateway

## RoomSec Gateway
roomsec_gateway_SOURCES = main.cpp \
	gateway.cpp

roomsec_gateway_CFLAGS = -O0 -I$(top_srcdir)/libauthorize
roomsec_gateway_LDADD = $(top_srcdir)/libauthorize/libauthorize.a
