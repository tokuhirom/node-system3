node-system3
============

Executes shell commands synchronously.

SYNOPSIS
--------

    var system3 = require('system3');

    system3.system('ls');

    // capture stdout
    var out = system3.qx('ls');
    console.log(out);

INSTALLATION
------------

    npm install system3

PORTABILITY
-----------

This module uses popen(3), pclose(3). It's in POSIX.1-2001.

SEE ALSO
--------

### https://github.com/hecomi/node-execSync

It does not contain english docs.

### https://github.com/mgutz/execSync

It works. But it depended on ffi. And .code does not works.

