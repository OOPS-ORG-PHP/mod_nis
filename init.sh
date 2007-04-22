#!/bin/sh
# $Id: init.sh,v 1.2 2007-04-22 17:41:33 oops Exp $

if [ -f Makefile ]; then
	make distclean
fi
rm -rf autom4te.cache/ build/ include/ modules/
rm -f Makefile.global ac*.m4 config.guess config.h* config.nice config.sub configure*
rm -f install-sh ltmain.sh missing mkinstalldirs scan_makefile_in.awk run-tests.php .deps
