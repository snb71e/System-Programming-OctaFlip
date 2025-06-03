#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "client.h"
#include "board.h"


void print_usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s server -p <port>\n", prog);
    printf("  %s client -i <ip> -p <port> -u <username>\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "server") == 0) {
        // ---- SERVER  ----
        int port = 8080;  // 기본값
	    char port_str[16];
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
                port = atoi(argv[++i]);
        }
    	snprintf(port_str, sizeof(port_str), "%d", port); 

        /*
        if (init_led_matrix(&argc, &argv) < 0) {
            fprintf(stderr, "Failed to initialize LED Matrix.\n");
            return EXIT_FAILURE;
        }
        */
        int ret = server_run(port_str);
        //close_led_matrix();
        return ret;

    } else if (strcmp(argv[1], "client") == 0) {
        // ---- CLIENT  ----
        char *ip = NULL;
        int port = 8080;
	    char port_str[16];
        char *username = NULL;
        int use_led = 0;  // LED 사용 여부

        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
                ip = argv[++i];
            else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
                port = atoi(argv[++i]);
            else if (strcmp(argv[i], "-u") == 0 && i + 1 < argc)
                username = argv[++i];
            else if (strcmp(argv[i], "--led") == 0)
                use_led = 1;
        }
    	snprintf(port_str, sizeof(port_str),"%d", port);
        if (!ip || !username) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        if (use_led && init_led_matrix(&argc, &argv) < 0) {
            fprintf(stderr, "Failed to initialize LED Matrix.\n");
            return EXIT_FAILURE;
        }

        int ret = client_run(ip, port_str, username, use_led);
        close_led_matrix();
        return ret;

    } else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
}
