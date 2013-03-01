ACLOCAL_AMFLAGS = -I m4
dist_doc_DATA = README
EXTRA_DIST = autogen.sh \
	     m4/gnulib-cache.m4 \
	     	     scripts/cpplint.py \
	     $(DX_DOCDIR)

AM_CPPFLAGS = \
	      -I$(top_srcdir)/src

pkginclude_HEADERS =
bin_PROGRAMS = roomsecgateway
lib_LTLIBRARIES = libroomsecgateway.la

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
	src/cpp/gen-cpp/Validator.cpp \
	src/cpp/gen-cpp/Validator.h

# generate thrift interfaces
$(thrift_cpp) : authorize/src/main/thrift/authorize.thrift 
	mkdir -p src/cpp/gen-cpp
	thrift --gen cpp -r -I ./authorize/src/main/thrift -o ./src/cpp/  \
		./authorize/src/main/thrift/authorize.thrift

# main library sources
libroomsecgateway_la_SOURCES = \
	$(thrift_cpp) \
	src/cpp/gateway.cpp \
	src/cpp/authorityadapter.cpp \
	src/cpp/thriftauthorityadapter.cpp

libroomsecgateway_la_CPPFLAGS = \
	-I/usr/include/thrift $(DEBUG) -Wall -W -Wwrite-strings \
	-std=c++11 -pedantic -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H \
	-I. -I./wiringPi

libroomsecgateway_la_LIBADD = \
	-lstdc++ -lthrift -lboost_system -lboost_thread


###
### roomsecgateway
###

roomsecgateway_SOURCES = \
	src/cpp/main.cpp

roomsecgateway_CPPFLAGS = \
	-I/usr/include/thrift $(DEBUG) -Wall -W -Wwrite-strings \
	-std=c++11 -pedantic -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H \
	-I./wiringPi

roomsecgateway_LDADD = \
	-lstdc++ -lthrift -lboost_system -lboost_thread -lroomsecgateway


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

# Define unit tests with gtest
UNIT_TESTS += test/roomsecgateway_test
check_PROGRAMS += test/roomsecgateway_test

test_roomsecgateway_test_CPPFLAGS = -I$(top_srcdir)/src/cpp $(GTEST_CPPFLAGS)
test_roomsecgateway_test_CXXFLAGS = $(GTEST_CXXFLAGS)
test_roomsecgateway_test_LDFLAGS = $(GTEST_LDFLAGS)
test_roomsecgateway_test_LDADD = $(top_builddir)/libroomsecgateway.la \
				   $(GTEST_LIBS) \
				   $(top_builddir)/lib/gmock/libgmock_main.la \
				   $(top_builddir)/lib/gmock/libgmock.la
test_roomsecgateway_test_SOURCES = 

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

