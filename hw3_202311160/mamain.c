#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/server.h"
#include "include/client.h"
#include "include/board.h"

#define DEFAULT_PORT "8080"

static void print_usage(const char *progname) {
    fprintf(stderr,
        "Usage:\n"
        "  %s server [-p port]\n"
        "  %s client -i ip -p port -u username\n",
        progname, progname);
}

int main(int argc, char *argv[]) {
    // ──────────────────────────────────────────────────────────────────────────
    // 1) LED 매트릭스 초기화: init_led_matrix가 내부적으로 argv에서
    //    "--led-rows=...", "--led-cols=...", "--led-gpio-mapping=...", "--led-brightness=..."
    //    "--led-chain=...", "--led-no-hardware-pulse" 등을 모두 제거합니다.
    if (init_led_matrix(argc, argv) < 0) {
        fprintf(stderr, "Failed to initialize LED Matrix. Exiting.\n");
        return EXIT_FAILURE;
    }
    printf("LED Matrix Initialized.\n");

    // ──────────────────────────────────────────────────────────────────────────
    // 2) init_led_matrix() 호출 후에는 argv에서 LED 옵션이 모두 제거되어야 합니다.
    //    이 시점에서 남은 argv[1]이 "server" 또는 "client"여야만 올바르게 동작합니다.
    if (argc < 2) {
        print_usage(argv[0]);
        close_led_matrix();
        return EXIT_FAILURE;
    }

    // ──────────────────────────────────────────────────────────────────────────
    // 3) 남은 argv[1]이 "server"인지 "client"인지 판단
    if (strcmp(argv[1], "server") == 0) {
        // server 모드: 뒤에 "-p <port>" 옵션이 올 수 있음
        const char *port = DEFAULT_PORT;
        int opt;
        opterr = 0;
        // getopt을 쓰려면 optind를 2로 설정 (argv[0]=progname, argv[1]="server"이므로)
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
        printf("Starting server on port %s...\n", port);
        int ret = server_run(port);
        close_led_matrix();
        return ret;

    } else if (strcmp(argv[1], "client") == 0) {
        // client 모드: 뒤에 "-i <ip> -p <port> -u <username>" 옵션이 와야 함
        char *ip = NULL;
        char *port = NULL;
        char *username = NULL;
        int opt;
        opterr = 0;
        // optind를 2로 설정 (argv[0]=progname, argv[1]="client"이므로)
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
        printf("Starting client: ip=%s, port=%s, username=%s\n", ip, port, username);
        int ret = client_run(ip, port, username);
        close_led_matrix();
        return ret;

    } else {
        // argv[1]이 "server"도 "client"도 아닐 때
        print_usage(argv[0]);
        close_led_matrix();
        return EXIT_FAILURE;
    }
}
