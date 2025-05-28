#include "server.h"
#include "macro.h"
#include <criterion/criterion.h>

Test(unit_tests, test_valid_function_freq) {
    params_t params;
    
    char *av[2] = {"8", NULL}; 
    size_t idx = 0;
    check_freq(&params, av, &idx);
    cr_assert_eq(params.frequence, 8);
}

Test(unit_tests, test_no_frequence) {
    params_t params;
    
    char *av[2] = {"0", NULL}; 
    size_t idx = 0;
    int res = check_freq(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_numbers_frequence) {
    params_t params;
    
    char *av[2] = {"hello", NULL}; 
    size_t idx = 0;
    int res = check_freq(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_negative_frequence) {
    params_t params;
    
    char *av[2] = {"-1", NULL}; 
    size_t idx = 0;
    int res = check_freq(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}
