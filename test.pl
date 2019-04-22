
:- load_foreign_library(plplot).

test_p:-
    shell(ls,0),
    plot_point('titolo',[1.0, 2.0, 3.0, 4.0, 5.0],[5.0 ,3.0, 1.0, 3.0, 5.0]),
    writeln('fine').

test:-
    thread_create(test_p, Id),
    writeln(Id),
    thread_join(Id),
    writeln('qui').
    