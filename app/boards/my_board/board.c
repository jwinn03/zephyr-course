#include <zephyr/init.h>
#include <zephyr/kernel.h>
static int board_my_board_init(void) {
    printf("Board Initialized");
    return 0;
}