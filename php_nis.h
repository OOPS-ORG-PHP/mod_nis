/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: JoungKyun.Kim <http://www.oops.org>                          |
  +----------------------------------------------------------------------+

  $Id$
*/

#ifndef PHP_NIS_H
#define PHP_NIS_H

#ifdef PHP_WIN32
#define PHP_NIS_API __declspec(dllexport)
#else
#define PHP_NIS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry nis_module_entry;
#define nis_module_ptr &nis_module_entry

PHP_MINIT_FUNCTION(nis);
PHP_MSHUTDOWN_FUNCTION(nis);
PHP_RINIT_FUNCTION(nis);
PHP_RSHUTDOWN_FUNCTION(nis);
PHP_MINFO_FUNCTION(nis);

PHP_FUNCTION(nis_version);
PHP_FUNCTION(nis_auth);
PHP_FUNCTION(nis_error);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(nis)
	int   global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(nis)
*/

/* In every utility function you add that needs to use variables 
   in php_nis_globals, call TSRM_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMG_CC
   after the last function argument and declare your utility function
   with TSRMG_DC after the last declared argument.  Always refer to
   the globals in your function as NIS_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define NIS_G(v) TSRMG(nis_globals_id, zend_nis_globals *, v)
#else
#define NIS_G(v) (nis_globals.v)
#endif

#define BUILDNO "1.0.0"

#define phpext_nis_ptr nis_module_ptr


/*
 * NIS library header
 */
#define YPERR_SUCCESS	0
#define YPERR_KEY		5
#define MAXUSERLEN		64
#define MAXSTRING		256

#define U_USER			0
#define U_PASS			1
#define U_UID			2
#define U_GID			3
#define U_NAME			4
#define U_HOME			5
#define U_SHELL			6

#define G_GROUP			0
#define G_PASSWD		1
#define G_GID			2
#define G_MEMBER		3

extern char niserr[1024];

typedef struct nis_rq_entry {
	char user[MAXUSERLEN];
	char passwd[MAXUSERLEN];
	char domain[MAXSTRING];
	char map[MAXUSERLEN];
} nisRqEntry;

typedef struct nis_ur_entry {
	char user[MAXUSERLEN];
	char passwd[MAXUSERLEN];
	__uid_t uid;
	__gid_t gid;
	char name[MAXSTRING];
	char home[MAXSTRING];
	char shell[MAXSTRING];
} nisUrEntry;

typedef struct nis_gr_engry {
	char group[MAXUSERLEN];
	char passwd[MAXUSERLEN];
	__gid_t gid;
	char **member;
} nisGrEntry;

int yp_get_default_domain (char **);
int yp_match (const char *, const char *, const char *,
					const int, char **, int *);
const char *yperr_string (const int);
nisUrEntry * nis_parse_urentry (char *);
char * nis_get_passwd_entry (nisRqEntry *);
void cstrcpy (char *, char *, size_t);

#endif	/* PHP_NIS_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
