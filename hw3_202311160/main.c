#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/server.h"
#include "include/client.h"

#define DEFAULT_PORT "8080"

static void print_usage(const char *progname) {
    fprintf(stderr,
        "Usage:\n"
        "  %s server [-p port]\n"
        "  %s client -i ip -p port -u username\n",
        progname, progname);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "server") == 0) {
        const char *port = DEFAULT_PORT;
        int opt;
        opterr = 0;
        // parse server options
        optind = 2;
        while ((opt = getopt(argc, argv, "p:")) != -1) {
            switch (opt) {
                case 'p':
                    port = optarg;
                    break;
                default:
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
            }
        }
        return server_run(port);
    }
    else if (strcmp(argv[1], "client") == 0) {
        char *ip = NULL;
        char *port = NULL;
        char *username = NULL;
        int opt;
        opterr = 0;
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
                    return EXIT_FAILURE;
            }
        }
        if (!ip || !port || !username) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        return client_run(ip, port, username);
    }
    else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
}
