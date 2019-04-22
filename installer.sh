# commands for ubuntu
# prerequisites
# sudo apt install libwxgtk3.0-dev
# sudo apt install libpango1.0-dev
# sudo apt install libreadline5-dev

gcc -I/usr/local/lib/swipl/include/ -fpic -c plplot.c &&
# gcc -I/usr/lib/swi-prolog/include/ -fpic -c plplot.c &&
gcc -shared -o plplot.so plplot.o