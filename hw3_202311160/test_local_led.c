// test_local_led.c

#include "board.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // 1) LED 매트릭스 초기화
    if (init_led_matrix(argc, argv) < 0) {
        fprintf(stderr, "LED Matrix initialization failed.\n");
        return EXIT_FAILURE;
    }

    // 2) 터미널에서 직접 보드 입력받아 LED로 출력
    local_led_test();

    // 3) 테스트 종료 후 매트릭스 해제
    close_led_matrix();
    return EXIT_SUCCESS;
}
