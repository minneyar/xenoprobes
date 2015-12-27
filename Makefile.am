ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	AUTHORS \
	COPYING \
	ChangeLog \
	INSTALL \
	NEWS \
	README.md \
	bootstrap \
	mira.dot \
	sample-inventory.csv \
	sites.csv

AM_CPPFLAGS = $(BOOST_CPPFLAGS)

AM_LDFLAGS = $(BOOST_LDFLAGS)


noinst_PROGRAMS = xenoprobes

xenoprobes_SOURCES = src/optimize.cpp

xenoprobes_LDADD = $(BOOST_PROGRAM_OPTIONS_LIB)

