#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <server.h>
#include <stdio.h>
#include <unistd.h>
#include "macro.h"

void setup(void) {
    fflush(stdout);
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(unit_test, test_help_flag_return, .init = setup) {
    cr_assert_eq(ERROR, help_flag());
}

Test(unit_test, test_help_flag_return_text, .init = setup) {
    int stdout_copy = dup(fileno(stdout));

    FILE *tmp = tmpfile();
    cr_assert_not_null(tmp, "Failed to create temp file");

    int tmp_fd = fileno(tmp);
    dup2(tmp_fd, fileno(stdout));

    help_flag();

    fflush(stdout);
    rewind(tmp);

    char buffer[128];
    fgets(buffer, sizeof(buffer), tmp);

    dup2(stdout_copy, fileno(stdout));
    close(stdout_copy);

    cr_assert_str_eq(buffer, "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
}
