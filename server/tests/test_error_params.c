#include "server.h"
#include "macro.h"
#include <criterion/criterion.h>
#include <unistd.h>

Test(unit_tests, test_help_flag_error_params) {
    params_t params;
    
    char *av[3] = {"./zappy_server","-h", NULL}; 
    size_t ac = 2;

    int stdout_copy = dup(fileno(stdout));

    FILE *tmp = tmpfile();
    cr_assert_not_null(tmp, "Failed to create temp file");

    int tmp_fd = fileno(tmp);
    dup2(tmp_fd, fileno(stdout));

    int res = check_params(&params, ac, av);

    fflush(stdout);
    rewind(tmp);

    char buffer[128];
    fgets(buffer, sizeof(buffer), tmp);

    dup2(stdout_copy, fileno(stdout));
    close(stdout_copy);

    cr_assert_str_eq(buffer, "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_enough_arguments) {
    params_t params;
    
    char *av[3] = {"./zappy_server","-p", "8080", NULL}; 
    size_t ac = 3;

    int stderr_copy = dup(fileno(stderr));
    
    FILE *tmp = tmpfile();
    cr_assert_not_null(tmp, "Failed to create temp file");
    int tmp_fd = fileno(tmp);
    
    dup2(tmp_fd, fileno(stderr));
    
    int res = check_params(&params, ac, av);
    
    fflush(stderr);
    rewind(tmp);
    
    char buffer[256] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, tmp);
    
    dup2(stderr_copy, fileno(stderr));
    close(stderr_copy);
    
    cr_assert_str_eq(buffer, "Error: Invalid number of parameters.\nYou need to have: -p, -x, -y, -n, -c, -f flags.\nCheck with ./zappy_server -h to understand how to use it.\n");
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_valid_arguments) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-p","8080", "-x", "15", "-y", "15", "-n", "a", "b", "-c", "15", "-f", "8", NULL}; 
    size_t ac = 14;
    int res = check_params(&params, ac, av);
    cr_assert_eq(res, SUCCESS);
}

Test(unit_tests, test_enough_arguments_but_missing_one) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-p","8080", "-x", "15", "-y", "15", "-n", "a", "b", "c", "d", "e","-f", "8", NULL}; 
    size_t ac = 14;
    int res = check_params(&params, ac, av);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_valid_argument) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-p","hello", "-x", "15", "-y", "15", "-n", "a", "b", "-c", "15", "-f", "8", NULL}; 
    size_t ac = 14;
    int stderr_copy = dup(fileno(stderr));
    
    FILE *tmp = tmpfile();
    cr_assert_not_null(tmp, "Failed to create temp file");
    int tmp_fd = fileno(tmp);
    
    dup2(tmp_fd, fileno(stderr));
    
    int res = check_params(&params, ac, av);
    
    fflush(stderr);
    rewind(tmp);
    
    char buffer[256] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, tmp);
    
    dup2(stderr_copy, fileno(stderr));
    close(stderr_copy);
    
    cr_assert_str_eq(buffer, "Error: Invalid parameter 'hello'\n");
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_enough_arguments_but_missing_width) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-p","8080", "-y", "15", "-n", "a", "b", "c","d","e", "-c", "8","-f", "8", NULL}; 
    size_t ac = 14;
    int res = check_params(&params, ac, av);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_enough_arguments_but_missing_height) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-p","8080", "-x", "15", "-n", "a", "b", "c", "d","e","-c", "8","-f", "8", NULL}; 
    size_t ac = 14;
    int res = check_params(&params, ac, av);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_enough_arguments_but_missing_port) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-x","15", "-y", "15", "-n", "a", "b", "c", "d","e","-c", "8","-f", "8", NULL}; 
    size_t ac = 14;
    int res = check_params(&params, ac, av);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_enough_arguments_but_frequence) {
    params_t params;
    
    init_params(&params);
    char *av[15] = {"./zappy_server","-x","15", "-y", "15", "-n", "a", "b", "c", "d","e","-c", "8","-p", "8080", NULL}; 
    size_t ac = 14;
    int res = check_params(&params, ac, av);
    cr_assert_eq(res, ERROR);
}
