#include <stdlib.h>
#include <stdio.h>

#include <SWI-Prolog.h>
// #ifdef _WIN32
// #include <Windows.h>
// #endif

static foreign_t plot_point(term_t title, term_t list_x_vals, term_t list_y_vals);
static foreign_t plot_histogram(term_t title, term_t list_x_vals, term_t list_y_vals);
int write_data_to_file(term_t list_x_vals, term_t list_y_vals, int len);

int write_data_to_file(term_t list_x_vals, term_t list_y_vals, int len) {
    int i;
    double current_value;
    term_t val = PL_new_term_ref();
    FILE * temp = fopen("data.temp", "w");

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

        if(PL_get_list(list_y_vals,val,list_y_vals) <= 0) {
            printf("error reading head x list.");
            return -1;
        }
        if(PL_get_float(val,&current_value) <= 0) {
            printf("error converting x value.");
            return -1;
        }

        fprintf(temp, "%lf\n",current_value);
    }

    fclose(temp);

    return 0;
}

static foreign_t plot_point(term_t title, term_t ref_x, term_t ref_y) {
    int i;
    char *title_plot;
    double current_value;
    term_t len_x, len_y, list_x_vals, list_y_vals;
    term_t val = PL_new_term_ref();

    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    
    if(popen("gnuplot -p","w") == NULL) {
        return PL_warning("Unable to open gnuplot terminal.");
    }
    
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

    if(write_data_to_file(list_x_vals,list_y_vals,(int)len_x) < 0) {
        return PL_warning("error in processing data.");
    }
    
    fprintf(gnuplotPipe, "set title \"%s\" \n", title_plot); 
    fprintf(gnuplotPipe, "plot 'data.temp'\n");

    PL_succeed;
}

static foreign_t plot_histogram(term_t title, term_t ref_x, term_t ref_y) {
    int i;
    char *title_plot;
    double current_value;
    term_t len_x, len_y, list_x_vals, list_y_vals;
    term_t val = PL_new_term_ref();

    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    
    if(popen("gnuplot -p","w") == NULL) {
        return PL_warning("Unable to open gnuplot terminal.");
    }
    
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

    if(write_data_to_file(list_x_vals,list_y_vals,(int)len_x) < 0) {
        return PL_warning("error in processing data.");
    }
    
    fprintf(gnuplotPipe, "set title \"%s\" \n", title_plot); 
    fprintf(gnuplotPipe, "set style fill solid 0.5 border lt -1 \n"); 
    fprintf(gnuplotPipe, "plot 'data.temp' smooth freq with boxes \n"); 

    PL_succeed;
}



install_t install() {
    PL_register_foreign("plot_point",3,plot_point,0);
    PL_register_foreign("plot_histogram",3,plot_histogram,0);
}
