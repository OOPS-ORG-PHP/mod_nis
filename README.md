PHP nis extension
==
[![PHP license](https://img.shields.io/badge/license-PHP-blue.svg)](https://raw.githubusercontent.com/php/php-src/master/LICENSE)
![GitHub download](https://img.shields.io/github/downloads/OOPS-ORG-PHP/mod_nis/total.svg)
[![GitHub last release](https://img.shields.io/github/release/OOPS-ORG-PHP/mod_nis.svg)](https://github.com/OOPS-ORG-PHP/mod_nis/releases)
[![GitHub closed issues](https://img.shields.io/github/issues-closed-raw/OOPS-ORG-PHP/mod_nis.svg)](https://github.com/OOPS-ORG-PHP/mod_nis/issues?q=is%3Aissue+is%3Aclosed)
[![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed-raw/OOPS-ORG-PHP/mod_nis.svg)](https://github.com/OOPS-ORG-PHP/mod_nis/pulls?q=is%3Apr+is%3Aclosed)

## 1. License

Copyright (c) 2020 JoungKyun.Kim &lt;http://oops.org&gt; All rights reserved.

This program is under ***PHP License***.

## 2. Description

mod_nis php 확장은 nis entry 를 이용하여 인증을 하기 위한 모듈이다. shadow system 에서는 mod_pam 의 경우, root 권한이 있어야 정상 작동을 하기 때문에, root 권한이 없는 apache module 로서의 사용시에 NIS 인증을 할 수 있도록 하기 위한 모듈이다.

단 단점으로는, NIS entry 에 존재하는 리스트만 인증이 가능하다. PAM 처럼 local account 까지 처리를 해 주지는 못한다.

## 3. Wanringin

* 이 확장은 매우 실험적인 버전이다. pruduct 에 적용하는 것을 권장하지 않는다.
* 1.0.x 부터는 PHP 7 이상만을 지원한다. PHP 7 미만에서는 0.x 버전을 사용 해야 한다. (기능적 차이는 없다.)
* 이 확장은 thread safe 하지 못하다!

## 4. Installation

```bash
[root@host nis]# phpize
[root@host nis]# ./configure
[root@host nis]# make
[root@host nis]# make install
```

## 5. Usage

### 5.1 Configurations

#### 5.1.1 module loading

If build with PHP dynamic extension, you will need to load ***nis.so*** file in ***php.ini***. Add follow configuration in th php.ini.

```ini
; for php 7.1 and before
extension = nis.so

; for php 7.2 and after
; default extension dir
extension = nis
; use absolute path
extension = /path/nis.so
```
