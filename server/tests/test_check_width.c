#include "server.h"
#include "macro.h"
#include <criterion/criterion.h>

Test(unit_tests, test_valid_function_width) {
    params_t params;
    
    char *av[2] = {"15", NULL}; 
    size_t idx = 0;
    check_width(&params, av, &idx);
    cr_assert_eq(params.width, 15);
}

Test(unit_tests, test_no_widthuence) {
    params_t params;
    
    char *av[2] = {"0", NULL}; 
    size_t idx = 0;
    int res = check_width(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_numbers_widthuence) {
    params_t params;
    
    char *av[2] = {"hello", NULL}; 
    size_t idx = 0;
    int res = check_width(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_negative_widthuence) {
    params_t params;
    
    char *av[2] = {"-1", NULL}; 
    size_t idx = 0;
    int res = check_width(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}
