ACLOCAL_AMFLAGS = -I m4
dist_doc_DATA = README
EXTRA_DIST = \
	autogen.sh \
	m4/gnulib-cache.m4 \
	scripts/cpplint.py \
	$(DX_DOCDIR)

AM_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/libfprint \
	$(THRIFT_CFLAGS)

pkginclude_HEADERS =

bin_PROGRAMS = roomsecgateway

noinst_LTLIBRARIES = libroomsecgateway.la

.PHONY = thrift

###
### libroomsecgateway
###

# all thrift generated files should be listed here
thrift_cpp = \
	src/cpp/gen-cpp/Authority.cpp \
	src/cpp/gen-cpp/Authority.h \
	src/cpp/gen-cpp/authorize_constants.cpp \
	src/cpp/gen-cpp/authorize_constants.h \
	src/cpp/gen-cpp/authorize_types.cpp \
	src/cpp/gen-cpp/authorize_types.h \
	src/cpp/gen-cpp/FingerprintAuthn.cpp \
	src/cpp/gen-cpp/FingerprintAuthn.h \
	src/cpp/gen-cpp/fingerprintauthn_constants.cpp \
	src/cpp/gen-cpp/fingerprintauthn_constants.h \
	src/cpp/gen-cpp/fingerprintauthn_types.cpp \
	src/cpp/gen-cpp/fingerprintauthn_types.h \
	src/cpp/gen-cpp/Validator.cpp \
	src/cpp/gen-cpp/Validator.h

# generate thrift interfaces
$(thrift_cpp) : authorize/src/main/thrift/authorize.thrift \
	fingerprintauthn/src/thrift/fingerprintauthn.thrift
	mkdir -p $(top_builddir)/src/cpp/gen-cpp
	thrift --gen cpp -r -I $(top_srcdir)/authorize/src/main/thrift \
		-o $(top_builddir)/src/cpp/  \
		$(top_srcdir)/fingerprintauthn/src/thrift/fingerprintauthn.thrift

# main library sourcesX(
libroomsecgateway_la_SOURCES = \
	$(thrift_cpp) \
	src/cpp/gateway.cpp \
	src/cpp/authorityadapter.cpp \
	src/cpp/thriftauthorityadapter.cpp \
	src/cpp/fingerprintauthnadapter.cpp \
	src/cpp/thriftfingerprintauthnadapter.cpp \
	src/cpp/gateway.cpp \
	src/cpp/stdgateway.cpp \
	src/cpp/replgateway.cpp \
	src/cpp/tailgateanalyzer.cpp \
	src/cpp/blockanalyzer.cpp \
	src/cpp/blocksensor.cpp \
	src/cpp/doorstatesensor.cpp \
	src/cpp/ioexpander.cpp \
	src/cpp/lcddisplay.cpp \
	src/cpp/buzzer.cpp \
	src/cpp/lock.cpp
	src/cpp/ui.cpp \
	src/cpp/doorstatesensor.cpp

libroomsecgateway_la_CPPFLAGS = \
	$(THRIFT_CFLAGS) $(LOG4CXX_CFLAGS) \
	$(DEBUG) -Wall -W -Wwrite-strings \
	-pedantic -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H \
	-I. -I$(top_srcdir)/wiringPi -I$(top_srcdir)/libfprint

libroomsecgateway_la_LDFLAGS = 
libroomsecgateway_la_LIBADD = \
	-lstdc++ $(THRIFT_LIBS) $(LOG4CXX_LIBS) \
	-lboost_system -lboost_thread \
	$(top_srcdir)/libfprint/libfprint/libfprint.la \
	-lwiringPi


###
### roomsecgateway
###

roomsecgateway_SOURCES = \
	src/cpp/main.cpp

roomsecgateway_CPPFLAGS = \
	$(THRIFT_CFLAGS) $(DEBUG) -Wall -W -Wwrite-strings \
	-pedantic -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H \
	-I$(top_srcdir)/wiringPi -I$(top_srcdir)/libfprint

roomsecgateway_LDFLAGS = -static

roomsecgateway_LDADD = libroomsecgateway.la \
	-lstdc++ $(THRIFT_LIBS) -lboost_system -lboost_thread \
	$(top_srcdir)/libfprint/libfprint/libfprint.la -lboost_program_options

###
### Google Testing Framework
###

# Build rules for testing framework.
check_LTLIBRARIES = \
	lib/gmock/libgmock.la \
	lib/gmock/libgmock_main.la

lib_gmock_libgmock_la_SOURCES = \
	lib/gmock/gmock/gmock.h \
	lib/gmock/gtest/gtest.h \
	lib/gmock/gmock-gtest-all.cc

lib_gmock_libgmock_la_CPPFLAGS = $(GTEST_CPPFLAGS)
lib_gmock_libgmock_la_CXXFLAGS = $(GTEST_CXXFLAGS)
lib_gmock_libgmock_la_LDFLAGS = $(GTEST_LDFLAGS)
lib_gmock_libgmock_la_LIBADD = $(GTEST_LIBS)
lib_gmock_libgmock_main_la_SOURCES = \
	lib/gmock/gmock/gmock.h \
	lib/gmock/gtest/gtest.h \
	lib/gmock/gmock_main.cc

lib_gmock_libgmock_main_la_CPPFLAGS = $(GTEST_CPPFLAGS)
lib_gmock_libgmock_main_la_CXXFLAGS = $(GTEST_CXXFLAGS)
lib_gmock_libgmock_main_la_LDFLAGS = $(GTEST_LDFLAGS)
lib_gmock_libgmock_main_la_LIBADD = \
	$(GTEST_LIBS) \
	lib/gmock/libgmock.la


###
### Tests
###

# Build rules for tests.
# TESTS_ENVIRONMENT: Set environment variables for the test run
# TESTS: Define programs run automatically by "make check"
# UNIT_TESTS: Define programs run automatically by "make check-unit-tests"
# check_PROGRAMS: Define programs built by "make check" but not necessarily run
TESTS_ENVIRONMENT=
TESTS=
UNIT_TESTS=
check_PROGRAMS=

# Raspberry Pi specific hardware tests
if ENABLE_HARDWARE_TESTS
# Define unit tests with gtest
check_PROGRAMS += test/roomsecgateway_hardware_test
test_roomsecgateway_hardware_test_CPPFLAGS = -I$(top_srcdir)/wiringPi -I$(top_srcdir)/test -I$(top_srcdir)/src/cpp $(GTEST_CPPFLAGS)
test_roomsecgateway_hardware_test_CXXFLAGS = $(GTEST_CXXFLAGS)
test_roomsecgateway_hardware_test_LDFLAGS = $(GTEST_LDFLAGS)

test_roomsecgateway_hardware_test_LDADD = \
	$(top_builddir)/libroomsecgateway.la \
	$(GTEST_LIBS) \
	$(top_builddir)/lib/gmock/libgmock_main.la \
	$(top_builddir)/lib/gmock/libgmock.la

test_roomsecgateway_hardware_test_SOURCES = \
	test/lcddisplaytest.cpp \
	test/buzzertest.cpp \
	test/doorstatesensortest.cpp \
	test/locktest.cpp
endif

# Define unit tests with gtest
UNIT_TESTS += test/roomsecgateway_test
check_PROGRAMS += test/roomsecgateway_test

test_roomsecgateway_test_CPPFLAGS = -I$(top_srcdir)/test -I$(top_srcdir)/src/cpp $(GTEST_CPPFLAGS)
test_roomsecgateway_test_CXXFLAGS = $(GTEST_CXXFLAGS)
test_roomsecgateway_test_LDFLAGS = $(GTEST_LDFLAGS)

test_roomsecgateway_test_LDADD = \
	$(top_builddir)/libroomsecgateway.la \
	$(GTEST_LIBS) \
	$(top_builddir)/lib/gmock/libgmock_main.la \
	$(top_builddir)/lib/gmock/libgmock.la

test_roomsecgateway_test_SOURCES = \
    test/authorityadaptertest.cpp\
    test/blockanalyzertest.cpp

TESTS += $(UNIT_TESTS)

TESTS_ENVIRONMENT_WITH_VALGRIND="libtool --mode=execute valgrind --leak-check=full"

.PHONY: check-with-valgrind
check-with-valgrind:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT=$(TESTS_ENVIRONMENT_WITH_VALGRIND)

.PHONY: check-unit-tests
check-unit-tests:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS=$(UNIT_TESTS)

.PHONY: check-unit-tests-with-valgrind
check-unit-tests-with-valgrind:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS=$(UNIT_TESTS) TESTS_ENVIRONMENT=$(TESTS_ENVIRONMENT_WITH_VALGRIND)

include Makefile.am.coverage
include Makefile.am.checkstyle
include Makefile.am.doxygen
