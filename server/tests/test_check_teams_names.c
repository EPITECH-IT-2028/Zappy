#include "server.h"
#include "macro.h"
#include <criterion/assert.h>
#include <criterion/criterion.h>

Test(unit_tests, test_valid_function_teams_names) {
    params_t params = {0};
    
    init_params(&params);
    char *av[4] = {"a","b","c", NULL}; 
    size_t idx = 0;
    check_teams_names(&params, av, &idx);
    cr_assert_str_eq(params.teams_names[0], "a");
    cr_assert_str_eq(params.teams_names[1], "b");
    cr_assert_str_eq(params.teams_names[2], "c");
    cr_assert_eq(params.teams_count, 3);
}

Test(unit_tests, test_break_condition) {
    params_t params;
    
    init_params(&params);
    char *av[4] = {"a","-c","c", NULL}; 
    size_t idx = 0;
    check_teams_names(&params, av, &idx);
    cr_assert_str_eq(params.teams_names[0], "a");
    cr_assert_null(params.teams_names[1]);
    cr_assert_eq(params.teams_count, 1);
}

Test(unit_tests, test_av_null) {
    params_t params;
    
    char **av = NULL; 
    size_t idx = 0;
    int res = check_teams_names(&params, av, &idx);
    cr_assert_eq(res, ERROR);
}

Test(unit_tests, test_idx_null) {
    params_t params;
    
    char *av[4] = {"a","b","c", NULL}; 
    int res = check_teams_names(&params, av, NULL);
    cr_assert_eq(res, ERROR);
}
