#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/server.h"
#include "include/client.h"
#include "include/board.h"

#define DEFAULT_PORT "8080"

// 사용법 출력 함수
static void print_usage(const char *progname) {
    fprintf(stderr,
        "Usage:\n"
        "  %s server [-p port]\n"
        "  %s client -i ip -p port -u username\n",
        progname, progname);
}

int main(int argc, char *argv[]) {
    // ──────────────────────────────────────────────────────────────────────────
    // 1) LED 매트릭스 초기화
    //    init_led_matrix() 호출 시 argv[]에서 "--led-…"로 시작하는 모든 플래그를 제거합니다.
    if (init_led_matrix(argc, argv) < 0) {
        fprintf(stderr, "Failed to initialize LED Matrix. Exiting.\n");
        return EXIT_FAILURE;
    }
    printf("[main] init_led_matrix() 성공\n");

    // (선택) 남은 argv[] 내용을 확인하기 위한 디버그 출력
    //    실제 동작만 확인하고 싶으면 이 블록 전체를 주석 처리해도 됩니다.
    {
        printf("[main] init_led_matrix 이후 남은 인자 (argc=%d):\n", argc);
        for (int i = 0; i < argc; ++i) {
            printf("  argv[%d] = '%s'\n", i, argv[i]);
        }
    }

    // ──────────────────────────────────────────────────────────────────────────
    // 2) LED 플래그가 모두 제거된 뒤, 남은 인자가 제대로 "server" 또는 "client"인지 검사
    if (argc < 2) {
        print_usage(argv[0]);
        close_led_matrix();
        return EXIT_FAILURE;
    }

    // ──────────────────────────────────────────────────────────────────────────
    // 3) "server" 모드일 때
    if (strcmp(argv[1], "server") == 0) {
        const char *port = DEFAULT_PORT;
        int opt;
        opterr = 0;
        // argv[0] : 프로그램 이름, argv[1] : "server"이므로
        // getopt을 위해 optind를 2로 설정
        optind = 2;
        while ((opt = getopt(argc, argv, "p:")) != -1) {
            switch (opt) {
                case 'p':
                    port = optarg;
                    break;
                default:
                    print_usage(argv[0]);
                    close_led_matrix();
                    return EXIT_FAILURE;
            }
        }
        printf("[main] 서버 모드로 실행 (port=%s)\n", port);
        int ret = server_run(port);
        close_led_matrix();
        return ret;

    // ──────────────────────────────────────────────────────────────────────────
    // 4) "client" 모드일 때
    } else if (strcmp(argv[1], "client") == 0) {
        char *ip = NULL;
        char *port = NULL;
        char *username = NULL;
        int opt;
        opterr = 0;
        // argv[0] : 프로그램 이름, argv[1] : "client"이므로
        // getopt을 위해 optind를 2로 설정
        optind = 2;
        while ((opt = getopt(argc, argv, "i:p:u:")) != -1) {
            switch (opt) {
                case 'i':
                    ip = optarg;
                    break;
                case 'p':
                    port = optarg;
                    break;
                case 'u':
                    username = optarg;
                    break;
                default:
                    print_usage(argv[0]);
                    close_led_matrix();
                    return EXIT_FAILURE;
            }
        }
        if (!ip || !port || !username) {
            print_usage(argv[0]);
            close_led_matrix();
            return EXIT_FAILURE;
        }
        printf("[main] 클라이언트 모드로 실행 (ip=%s, port=%s, username=%s)\n",
               ip, port, username);
        int ret = client_run(ip, port, username);
        close_led_matrix();
        return ret;

    // ──────────────────────────────────────────────────────────────────────────
    // 5) 그 외 인자가 들어왔을 때
    } else {
        print_usage(argv[0]);
        close_led_matrix();
        return EXIT_FAILURE;
    }
}
