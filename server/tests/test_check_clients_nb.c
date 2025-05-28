#include "server.h"
#include "macro.h"
#include <criterion/criterion.h>

Test(unit_tests, test_valid_function) {
    params_t params;
    
    char *av[2] = {"8", NULL}; 
    size_t idx = 0;
    check_clients_nb(&params, av, &idx);
    cr_assert_eq(params.client_per_team, 8);
}

Test(unit_tests, test_no_clients) {
    params_t params;
    
    char *av[2] = {"0", NULL}; 
    size_t idx = 0;
    int res = check_clients_nb(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_not_numbers_clients) {
    params_t params;
    
    char *av[2] = {"hello", NULL}; 
    size_t idx = 0;
    int res = check_clients_nb(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_negative_clients) {
    params_t params;
    
    char *av[2] = {"-1", NULL}; 
    size_t idx = 0;
    int res = check_clients_nb(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}
