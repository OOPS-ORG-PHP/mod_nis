dnl $Id: config.m4,v 1.1.1.1 2007-04-22 17:39:33 oops Exp $
dnl config.m4 for extension nis

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(nis, for nis support,
[  --with-nis              Include nis support])

if test "$PHP_NIS" != "no"; then
	AC_DEFINE(HAVE_NIS,1,[ ])

	NIS_PARAMETER="$CPPLAGS"
	PHP_SUBST(LDFLAGS)
	PHP_SUBST(CPPFLAGS)

	SEARCH_PATH="/usr /usr/local $PHP_NIS $LIBSEARCH"

	for i in $SEARCH_PATH
	do
		if test -f $i/lib/libnsl.$SHLIB_SUFFIX_NAME -o -f $i/lib/libnsl.a ; then
			NIS_LIB_DIR=$i/lib
			break
		fi
	done

	if test -z "$NIS_LIB_DIR"; then
		AC_MSG_ERROR([libnsl.(a|so) not found.])
	fi

	dnl Checks for header files
	AC_HEADER_STDC
	AC_CHECK_HEADERS(sys/types.h grp.h pwd.h)

	dnl Checks for library functions.
	PHP_CHECK_LIBRARY(nsl, yp_get_default_domain,
	[
		PHP_ADD_LIBRARY_WITH_PATH(nsl, $NIS_LIB_DIR, NIS_SHARED_LIBADD)
	],[
		AC_MSG_ERROR([wrong nsl library version or library not found])
	],[
		-L$NIS_LIB_DIR -lnsl
	])

	PHP_CHECK_LIBRARY(nsl, yp_match,
	[ ],[
		AC_MSG_ERROR([wrong nsl library version or library not found])
	],[
		-L$NIS_LIB_DIR -lnsl
	])

	PHP_CHECK_LIBRARY(nsl, yperr_string,
	[ ],[
		AC_MSG_ERROR([wrong nsl library version or library not found])
	],[
		-L$NIS_LIB_DIR -lnsl
	])

	PHP_CHECK_LIBRARY(crypt, crypt,
	[
		PHP_ADD_LIBRARY_WITH_PATH(crypt, $NIS_LIB_DIR, NIS_SHARED_LIBADD)
	],[
		AC_MSG_ERROR([wrong crypt library version or library not found])
	],[
		-L$NIS_LIB_DIR -lcrypt
	])

	PHP_SUBST(NIS_SHARED_LIBADD)
	PHP_NEW_EXTENSION(nis, php_nis.c, $ext_shared)
fi
