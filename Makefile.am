ACLOCAL_AMFLAGS = -I m4
SUBDIRS = libauthorize src .
dist_doc_DATA = README
EXTRA_DIST = autogen.sh m4/gnulib-cache.m4

.PHONY = thrift

thrift:
	mkdir -p ./libauthorize
	thrift -v --gen cpp -o $(top_dir)./libauthorize $(top_srcdir)/authorize/src/main/thrift/authorize.thrift 
