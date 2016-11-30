noinst_PROGRAMS = test

# set the include path found by configure
INCLUDES = $(all_includes)

lib_LTLIBRARIES=libmjson.la

mjsondir=$(includedir)/mjson-$(MILESTONE)
mjson_HEADERS = json.h json_helper.h
libmjson_la_LDFLAGS=-release $(MILESTONE)
libmjson_la_SOURCES=\
	$(mjson_HEADERS) \
	json.c \
	json_helper.c \
	$(NULL)

# the library search path.
test_LDFLAGS = $(all_libraries)
test_LDADD = libmjson.la
test_SOURCES = test.c

EXTRA_DIST=.indent.pro

check-local:
	@for src in $(test_SOURCES) $(libmjson_la_SOURCES); do \
		echo "[TEST] $$src"; \
		INDENT_PROFILE="$(srcdir)/.indent.pro" indent -st $$src | diff -u $$src - || exit $?; \
	done

installcheck-local:
	cat $(srcdir)/test.c | \
		$(CC) -x c - -o /dev/null \
		$(shell PKG_CONFIG_PATH=$(pkgconfigdir):$$PKG_CONFIG_PATH pkg-config --cflags --libs mjson-@MILESTONE@)
