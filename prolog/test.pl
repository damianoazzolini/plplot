:- module(test_plplot, [test_plplot/0]).
:- use_module(library(plunit)).
:- ensure_loaded(library(plplot)).

test_plplot:-
    run_tests([basic]).

:- begin_tests(basic, []).

:-ensure_loaded(library(plplot)).

test(plot_point):-
  plot_point('Title',[1.0, 2.0, 3.0, 4.0, 5.0],[5.0 ,3.0, 1.0, 3.0, 5.0]).

:- end_tests(basic).