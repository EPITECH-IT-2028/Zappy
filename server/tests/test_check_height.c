#include "server.h"
#include "macro.h"
#include <criterion/criterion.h>

Test(unit_tests, test_valid_function_height) {
    params_t params;
    
    char *av[2] = {"15", NULL}; 
    size_t idx = 0;
    check_height(&params, av, &idx);
    cr_assert_eq(params.height, 15);
}

Test(unit_tests, test_no_heightuence) {
    params_t params;
    
    char *av[2] = {"0", NULL}; 
    size_t idx = 0;
    int res = check_height(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_numbers_heightuence) {
    params_t params;
    
    char *av[2] = {"hello", NULL}; 
    size_t idx = 0;
    int res = check_height(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_negative_heightuence) {
    params_t params;
    
    char *av[2] = {"-1", NULL}; 
    size_t idx = 0;
    int res = check_height(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}
