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
  | Author: JoungKyun.Kim <http://oops.org>                              |
  +----------------------------------------------------------------------+

  $Id: php_nis.c 13 2012-11-12 08:51:42Z oops $
*/

/*
 * PHP5 NIS library module "nis"
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include "php_nis.h"

/* If you declare any globals in php_nis.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(nis)
*/

/* True global resources - no need for thread safety here */
static int le_nis;
char niserr[1024] = { 0, };

ZEND_BEGIN_ARG_INFO_EX(arginfo_nis_auth, 0, 0, 2)
	ZEND_ARG_INFO(0, user)
	ZEND_ARG_INFO(0, pass)
	ZEND_ARG_INFO(0, domain)
	ZEND_ARG_INFO(0, map)
ZEND_END_ARG_INFO()

/* {{{ nis_functions[]
 *
 * Every user visible function must have an entry in nis_functions[].
 */
const zend_function_entry nis_functions[] = {
	PHP_FE(nis_version, NULL)
	PHP_FE(nis_auth,    arginfo_nis_auth)
	PHP_FE(nis_error,   NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ nis_module_entry
 */
zend_module_entry nis_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"nis",
	nis_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(nis),
#if ZEND_MODULE_API_NO >= 20010901
	BUILDNO, /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_NIS
ZEND_GET_MODULE(nis)
#endif

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(nis)
{
	php_info_print_table_start();
	php_info_print_table_colspan_header(2, "nis extension support");
	php_info_print_table_row(2, "Summary", "Authorization with YP/NIS");
	php_info_print_table_row(2, "URL", "http://devel.oops.org/");
	php_info_print_table_row(2, "Build version", BUILDNO);
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto char nis_buildver (void)
 *  print nis extension build number */
PHP_FUNCTION(nis_version)
{
	RETURN_STRING (BUILDNO, 1);
}
/* }}} */

/* {{{ proto bool nis_auth (char *user, char *pass, char *domain, char *map)
 *  return SUCCESS or FAILURE */
PHP_FUNCTION(nis_auth)
{
	char * user = NULL;
	int    user_len;
	char * pass = NULL;
	int    pass_len;
	char * domain = NULL;
	int    domain_len;
	char * map = NULL;
	int    map_len;

	nisRqEntry *rqEntry;
	char *nispass;
	char *crypts;

	if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "ss|ss", &user, &user_len, &pass, &pass_len, &domain, &domain_len, &map, &map_len) == FAILURE ) {
		efree (rqEntry);
		return;
	}

	if ( ! user_len ) {
		php_error (E_WARNING, "1st argument is empty or missing.");
		RETURN_FALSE;
	}

	rqEntry = (nisRqEntry *) emalloc (sizeof (nisRqEntry) + 1);
	memset (rqEntry->domain, 0, MAXSTRING);
	memset (rqEntry->map, 0, MAXUSERLEN);

	cstrcpy (rqEntry->user, user, MAXUSERLEN);
	cstrcpy (rqEntry->passwd, pass_len ? pass : "", MAXUSERLEN);

	if ( domain_len )
		cstrcpy (rqEntry->domain, domain, MAXSTRING);

	if ( map_len )
		cstrcpy (rqEntry->map, map, MAXUSERLEN);

	if ( ! strlen (rqEntry->domain) ) {
		char *domain_t;
		int r;
		if ( (r = yp_get_default_domain (&domain_t)) != 0 ) {
			memset (niserr, 0, 1024);
			strcpy (niserr, yperr_string (r));
			efree (rqEntry);
			RETURN_FALSE;
		}

		cstrcpy (rqEntry->domain, domain_t, MAXSTRING);
	}

	if ( ! strlen (rqEntry->map) )
		strcpy (rqEntry->map, "passwd.byname");

	if ( le_nis ) {
		php_printf ("DEBUG: USER   => %s\n", rqEntry->user);
		php_printf ("DEBUG: PASS   => %s\n", rqEntry->passwd);
		php_printf ("DEBUG: DOMAIN => %s\n", rqEntry->domain);
		php_printf ("DEBUG: MAP    => %s\n", rqEntry->map);
	}

	nispass = nis_get_passwd_entry (rqEntry);
	crypts = (char *) crypt (rqEntry->passwd, nispass);

	if ( le_nis ) {
		php_printf ("DEBUG: NIS    => %s\n", nispass);
		php_printf ("DEBUG: CRYPT  => %s\n", crypts);
	}

	if ( ! strcmp (crypts, nispass) ) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}

	efree (nispass);
	efree (rqEntry);
}
/* }}} */

/* {{{ proto char nis_error (void)
 */
PHP_FUNCTION(nis_error)
{
	RETURN_STRING (niserr, 1);
}

/* }}} */

void cstrcpy (char *tray, char *str, size_t size) {
	int l;

	memset (tray, 0, size);
	l = strlen (str);

	if ( l >= size )
		strncpy (tray, str, size - 1);
	else if ( l > 0 )
		strcpy (tray, str);
}

char * nis_get_passwd_entry (nisRqEntry *entry) {
	nisUrEntry *urentry;
	char *val, *ret;
	int r, vallen;

	r = yp_match (entry->domain, entry->map, entry->user,
		  		 	strlen (entry->user), &val, &vallen);
	if ( r == YPERR_KEY )
		r = yp_match (entry->domain, entry->map, entry->user,
			   			strlen (entry->user) + 1, &val, &vallen);

	if ( r == YPERR_SUCCESS )
		if ( val[vallen - 1] == 0 )
			--vallen;

	if ( le_nis > 1 )
		php_printf ("DEBUG: ENTRY  => %s\n", val);

	if ( vallen < 1 )
		return NULL;

	urentry = nis_parse_urentry (val);

	if ( le_nis ) {
		php_printf ("DEBUG: NUSER  => %s\n", urentry->user);
		php_printf ("DEBUG: NPASS  => %s\n", urentry->passwd);
		php_printf ("DEBUG: NUID   => %ld\n", urentry->uid);
		php_printf ("DEBUG: NGID   => %ld\n", urentry->gid);
		php_printf ("DEBUG: NNAME  => %s\n", urentry->name);
		php_printf ("DEBUG: NHOME  => %s\n", urentry->home);
		php_printf ("DEBUG: NSHELL => %s\n", urentry->shell);
	}

	ret = estrdup (urentry->passwd);
	efree (urentry);

	return ret;
}

nisUrEntry * nis_parse_urentry (char *str) {
	nisUrEntry *r;
	char *tmp;
	char ** sep, ** sep_t;
	int i, l, c = 0;

	r = (nisUrEntry *) emalloc (sizeof (nisUrEntry) + 1);

	sep = (char **) emalloc (sizeof (char *) * 8);
	sep_t = sep;
	i = 0;

    while ( (*sep = strsep (&str, ":") ) != NULL ) {
        c = ( ** sep == 0 ) ? 1 : 0;

        l = strlen (*sep);

        switch (i) {
            case U_USER :
                memset (r->user, 0, MAXUSERLEN);
                if ( c == 1 ) goto nextvalue;

                if ( l >= MAXUSERLEN )
                    strncpy (r->user, *sep, 31);
                else
                    strcpy (r->user, *sep);

                break;
            case U_PASS :
                memset (r->passwd, 0, MAXUSERLEN);
                if ( c == 1 ) goto nextvalue;

                if ( l >= MAXUSERLEN )
                    strncpy (r->passwd, *sep, 31);
                else
                    strcpy (r->passwd, *sep);

                break;
            case U_UID :
                if ( c == 1 ) goto nextvalue;
                r->uid = strtoul (*sep, (char **)NULL, 10);
                break;
            case U_GID :
                if ( c == 1 ) goto nextvalue;
                r->gid = strtoul (*sep, (char **)NULL, 10);
                break;
            case U_NAME :
                memset (r->name, 0, MAXSTRING);
                if ( c == 1 ) goto nextvalue;

                if ( l >= MAXSTRING )
                    strncpy (r->name, *sep, MAXSTRING - 1);
                else
                    strcpy (r->name, *sep);

                break;
            case U_HOME :
                memset (r->home, 0, MAXSTRING);
                if ( c == 1 ) goto nextvalue;

                if ( l >= MAXSTRING )
                    strncpy (r->home, *sep, MAXSTRING - 1);
                else
                    strcpy (r->home, *sep);

                break;
            default :
                memset (r->shell, 0, MAXSTRING);
                if ( c == 1 ) goto nextvalue;

                if ( l >= MAXSTRING )
                    strncpy (r->shell, *sep, MAXSTRING - 1);
                else
                    strcpy (r->shell, *sep);
        }
        sep++;
nextvalue:
        i++;
    }
    efree (sep_t);

    return r;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
