#!/usr/bin/php
<?php

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
