plplot
=====
Plot graphs using prolog and *gnuplot*.

Requirements
-----
gnuplot must be installed and in the path.

To check if gnuplot is installed type: `command -v gnuplot`. If nothing is printed, this means that *gnuplot* is missing.

You can download it from http://www.gnuplot.info/download.html and then follow the instructions in the folder. 

Usually you need other libraries to install gnuplot as `libwxgtk3.0-dev` `libpango1.0-dev` and `libreadline5-dev`.

Installation (STILL TO DO)
----
plplot can be installed using `pack_install/1`

    $ swipl
    ?- pack_install(plplot).

Still does not work on Windows

Example (STILL TO DO)
-----
    $ swipl
    ?- use_module(library(plplot)).
    ?- plot_point('Title',[1.0, 2.0, 3.0, 4.0, 5.0],[5.0 ,3.0, 1.0, 3.0, 5.0]).

The result should be something like:
![Screenshot](img/sample_plot.png)

How it works
----
It's simple: the commands are saved in a file named `commands.txt`. 
The data are saved in a file named `data.temp`.
Then *gnuplot* is called passing those two files.

How to test without using `pack_install/1` (THE ONLY METHOD NOW)
---
To use the program without downloading the package with `pack_install/1` you can do:
* Find the file `<SWI-Prolog.h>`. Usually is `/usr/local/lib/swipl/include/` or `/usr/lib/swi-prolog/include/`.
* Generate the `.o` file with gcc passing the location of `<SWI-Prolog.h>`. If it's in `/usr/local/lib/swipl/include/` then the command is:
    * `gcc -I/usr/local/lib/swipl/include/ -fpic -c plplot.c`
* Generate the `.so` file with:
    * `gcc -shared -o plplot.so plplot.o`
* Now you can create a file `test.pl` like this:
```
:- load_foreign_library(plplot).

test_p:-
    plot_point('Title',[1.0, 2.0, 3.0, 4.0, 5.0],[5.0 ,3.0, 1.0, 3.0, 5.0]).

```
* To test use: 
```
    $ swipl

    ?- test_p.
```

Contribute and suggestions
---
Pull requests, issues or whatever you prefer. 