fortune.cgi
===========
Serverless [fortune(6)](https://man.openbsd.org/fortune) microservice.

Example
-------
    $ ./fortune-cgi/fortune.cgi  # or
    Content-Type: text/json
    
    {
        "id": 124,
        "fortune": "It may or may not be worthwhile, but it still has to be done.\n"
    }

Live demo environment: https://sjmulder.nl/fortune

~Installation~ Deployment
-------------------------
First compile:

    $ sudo apt install build-essential libsqlite3-dev libyajl-dev
    $ make

This yields `fortune.db` and `fortune-cgi/fortune.cgi`.

 - Place `fortune.cgi` in the `cgi-bin/` storage container of your
   serverless platform.
 - Place `fortune.db` next to it, or place it somewhere else and set
   `FORTUNE_DB` to its path in your environment's infrastructure-as-code
   configuration.

Author
------
By Sijmen J. Mulder (<ik@sjmulder.nl>). See LICENSE.md.
