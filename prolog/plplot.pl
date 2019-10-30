:- module(plplot,[
    plot_point/3,
    plot_line/3,
    plot_point_new/2
    ]).

:-use_foreign_library(foreign(plplot),install).