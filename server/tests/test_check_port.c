#include "server.h"
#include "macro.h"
#include <criterion/criterion.h>

Test(unit_tests, test_valid_function_port) {
    params_t params;
    
    char *av[2] = {"8080", NULL}; 
    size_t idx = 0;
    check_port(&params, av, &idx);
    cr_assert_eq(params.port, 8080);
}

Test(unit_tests, test_no_portuence) {
    params_t params;
    
    char *av[2] = {"0", NULL}; 
    size_t idx = 0;
    int res = check_port(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_numbers_portuence) {
    params_t params;
    
    char *av[2] = {"hello", NULL}; 
    size_t idx = 0;
    int res = check_port(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_too_big_port) {
    params_t params;
    
    char *av[2] = {"888888", NULL}; 
    size_t idx = 0;
    int res = check_port(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_negative_portuence) {
    params_t params;
    
    char *av[2] = {"-1", NULL}; 
    size_t idx = 0;
    int res = check_port(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}
