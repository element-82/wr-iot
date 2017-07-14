Building And Running With Telit
===============================

TR50 Source
-----------
This is the only dependency required:

```sh
	git clone https://github.com/inhedron/libtr50.git
	cd libtr50
	./configure
	make
	sudo make install
```
  * this will install into /usr/local/lib
  * make sure your system looks in /usr/local/lib for LD paths.  E.g.
    * /etc/ld.so.conf.d/libc.conf contains /usr/local/lib

Run Example Code
----------------
```sh
	cd telit-examples/C-example
	make
```

  * app_demo is the only executable
  * requires a thing_key and app_token on the command line
  * e.g. ./app_demo <thing_key> <app_token>
  * For this example:
    * thing_key is automatically assigned, e.g. YOUR INITIALS_thing_demo
    * app_token is generated when you create an application in the
    cloud.  Use mine for now: 1yqBX0sOhLB8Dtxo
  *

```sh
	./app_demo XX_thing_demo 1yqBX0sOhLB8Dtxo
```

Example Details
---------------
  * About 126 lines of C code
  * connects to cloud
  * defines 2 methods
    * method_1 returns a successful result
    * method_2 returns a failure
  * defines a property and sends a random sample every second
  * defines an alarm and triggers based on random sample conditional
