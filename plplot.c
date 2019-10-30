#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <SWI-Prolog.h>
// #ifdef _WIN32
// #include <Windows.h>
// #endif

#define OFFSET 1

static foreign_t plot_point(term_t title, term_t list_x_vals, term_t list_y_vals);
static foreign_t plot_line(term_t title, term_t list_x_vals, term_t list_y_vals);
static foreign_t plot_point_new(term_t title, term_t list);

int write_data_to_file(char *filename, term_t list_x_vals, term_t list_y_vals, int len, int *l_x, int *u_x, int *l_y, int *u_y);

int write_data_to_file(char *filename, term_t list_x_vals, term_t list_y_vals, int len, int *l_x, int *u_x, int *l_y, int *u_y) {
    int i;
    double current_value;
    term_t val = PL_new_term_ref();
    FILE * temp = fopen(filename, "w");

    for(i = 0; i < len; i++) {
        if(PL_get_list(list_x_vals,val,list_x_vals) <= 0) {
            printf("error reading head x list.");
            return -1;
        }
        if(PL_get_float(val,&current_value) <= 0) {
            printf("error converting x value.");
            return -1;
        }
        
        fprintf(temp, "%lf ",current_value);

        if((int)current_value > *u_x) {
            *u_x = (int)current_value;
        }
        if((int)current_value < *l_x) {
            *l_x = (int)current_value;
        }

        if(PL_get_list(list_y_vals,val,list_y_vals) <= 0) {
            printf("error reading head x list.");
            return -1;
        }
        if(PL_get_float(val,&current_value) <= 0) {
            printf("error converting x value.");
            return -1;
        }

        fprintf(temp, "%lf\n",current_value);

        if((int)current_value > *u_y) {
            *u_y = (int)current_value;
        }
        if((int)current_value < *l_y) {
            *l_y = (int)current_value;
        }
    }

    fclose(temp);

    return 0;
}

static foreign_t plot_point(term_t title, term_t ref_x, term_t ref_y) {
    int lower_x = INT_MAX,lower_y = INT_MAX,upper_x = INT_MIN,upper_y = INT_MIN;
    char *title_plot;
    term_t len_x, len_y, list_x_vals, list_y_vals;
    FILE *mycommands;
    char *filename = "data.temp";
    
    list_x_vals = PL_copy_term_ref(ref_x);
    list_y_vals = PL_copy_term_ref(ref_y);

    if(PL_skip_list(list_x_vals,0,&len_x) != PL_LIST) {
        return PL_warning("List of X is not a proper list.");
    }

    if(PL_skip_list(list_y_vals,0,&len_y) != PL_LIST) {
        return PL_warning("List of Y is not a proper list.");
    }

    if(len_x != len_y) {
        return PL_warning("len list x != len list y.");
    }

    if(!PL_get_atom_chars(title, &title_plot)) {
        return PL_warning("plot_test/3: instantiation fault title.\nAre you using \" instead of \' ?");
    }

    if(write_data_to_file(filename,list_x_vals,list_y_vals,(int)len_x,&lower_x,&upper_x,&lower_y,&upper_y) < 0) {
        return PL_warning("error in processing data.");
    }
    
    mycommands = fopen("commands.txt","wt");
    fprintf(mycommands,"set title \" %s \" font \",20\"\n",title_plot);
    fprintf(mycommands,"set xrange [%d:%d]\n",lower_x-OFFSET,upper_x+OFFSET);
    fprintf(mycommands, "set yrange [%d:%d]\n",lower_y-OFFSET,upper_y+OFFSET);
    fprintf(mycommands,"plot'%s'\n","data.temp");
    fclose(mycommands);

    if(system("gnuplot -p  <commands.txt") < 0) {
        return PL_warning("unable to open shell.");
    }
    PL_succeed;
}

static foreign_t plot_line(term_t title, term_t ref_x, term_t ref_y) {
    int lower_x = INT_MAX,lower_y = INT_MAX,upper_x = INT_MIN,upper_y = INT_MIN;
    char *title_plot;
    term_t list_x_vals, list_y_vals;
    size_t len_x, len_y;
    FILE *mycommands;
    char *filename = "data.temp";
    
    list_x_vals = PL_copy_term_ref(ref_x);
    list_y_vals = PL_copy_term_ref(ref_y);

    if(PL_skip_list(list_x_vals,0,&len_x) != PL_LIST) {
        return PL_warning("List of X is not a proper list.");
    }

    if(PL_skip_list(list_y_vals,0,&len_y) != PL_LIST) {
        return PL_warning("List of Y is not a proper list.");
    }

    if(len_x != len_y) {
        return PL_warning("len list x != len list y.");
    }

    if(!PL_get_atom_chars(title, &title_plot)) {
        return PL_warning("plot_test/3: instantiation fault title.\nAre you using \" instead of \' ?");
    }

    if(write_data_to_file(filename,list_x_vals,list_y_vals,(int)len_x,&lower_x,&upper_x,&lower_y,&upper_y) < 0) {
        return PL_warning("error in processing data.");
    }
    
    mycommands = fopen("commands.txt","wt");
    fprintf(mycommands,"set title \" %s \" font \",20\"\n",title_plot);
    fprintf(mycommands,"set xrange [%d:%d]\n",lower_x-OFFSET,upper_x+OFFSET);
    fprintf(mycommands, "set yrange [%d:%d]\n",lower_y-OFFSET,upper_y+OFFSET);
    fprintf(mycommands,"plot'%s' using 1:2 with linespoints\n","data.temp");
    fclose(mycommands);

    if(system("gnuplot -p  <commands.txt") < 0) {
        return PL_warning("unable to open shell.");
    }
    PL_succeed;
}

// ok, sostituire quello prima con questo
static foreign_t plot_point_new(term_t title, term_t list_in) {
    int lower_x = INT_MAX,lower_y = INT_MAX,upper_x = INT_MIN,upper_y = INT_MIN;
    char *title_plot;
    term_t list, list_pair = PL_new_term_ref(), list_x = PL_new_term_ref(), list_y = PL_new_term_ref();
    size_t len_list, len_x, len_y;
    FILE *mycommands;
    char filename[100];
    int i;
    
    list = PL_copy_term_ref(list_in);

    if(PL_skip_list(list,0,&len_list) != PL_LIST) {
        return PL_warning("List is not a proper list.");
    }

    if(!PL_get_atom_chars(title, &title_plot)) {
        return PL_warning("plot_test/3: instantiation fault title.\nAre you using \" instead of \' ?");
    }

    printf("Len list: %d\n",len_list);

    mycommands = fopen("commands.txt","wt");
    fprintf(mycommands,"set title \" %s \" font \",20\"\n",title_plot);
    // fprintf(mycommands,"set xrange [*:*] \n");
    // fprintf(mycommands,"set yrange [*:*] \n");
    // fprintf(mycommands,"set offset graph 0.10, 0.§10 \n");
    fprintf(mycommands,"plot ");
    
    
    for(i = 0; i < len_list; i++) {
        if(PL_get_list(list, list_pair, list) < 0) {
            fclose(mycommands);
            return PL_warning("Unable to open list.");
        }
        printf("list pair\n");
        if(PL_get_list(list_pair, list_x, list_pair) < 0) {
            fclose(mycommands);
            return PL_warning("Unable to open list.");
        }
        printf("list x\n");


        if(PL_get_list(list_pair, list_y, list_pair) < 0) {
            fclose(mycommands);
            return PL_warning("Unable to open list.");
        }
        printf("list y\n");


        if(PL_skip_list(list_x,0,&len_x) != PL_LIST) {
            fclose(mycommands);
            return PL_warning("List of X is not a proper list.");
        }

        if(PL_skip_list(list_y,0,&len_y) != PL_LIST) {
            fclose(mycommands);
            return PL_warning("List of Y is not a proper list.");
        }

        if(len_x != len_y) {
            fclose(mycommands);    
            return PL_warning("len list x != len list y.");
        }
        sprintf(filename,"data_%d.temp",i);
        printf("here\n");
        if(write_data_to_file(filename,list_x,list_y,(int)len_x,&lower_x,&upper_x,&lower_y,&upper_y) < 0) {
            fclose(mycommands); 
            return PL_warning("error in processing data.");
        }
        fprintf(mycommands,"'%s' using 1:2 title '%d'",filename,i);
        if(i < len_list - 1) {
            fprintf(mycommands,",");
        }
    }
    fprintf(mycommands,"\n");

    fclose(mycommands);

    if(system("gnuplot -p  <commands.txt") < 0) {
        return PL_warning("unable to open shell.");
    }

    PL_succeed;
}

install_t install() {
    PL_register_foreign("plot_point",3,plot_point,0);
    PL_register_foreign("plot_line",3,plot_line,0);
    PL_register_foreign("plot_point_new",2,plot_point_new,0);
}
