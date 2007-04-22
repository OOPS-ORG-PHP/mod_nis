#!/usr/bin/php
<?
# $Id: test.php,v 1.2 2007-04-22 17:41:33 oops Exp $

if ( ! extension_loaded ('nis') )
	dl ('nis.so');

$user = '';
$pass = '';
$domain = '';
$map = 'passwd.byname';

if ( nis_auth ($user, $pass, $domain, $map) === FALSE )
	echo "Auth Failed\n";
else
	echo "Auth Success\n";
?>
