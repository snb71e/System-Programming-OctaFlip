gcc -Iinclude main.c src/*.c libs/cJSON.c -o hw3 -lpthread
./hw3 server
./hw3 client -i 127.0.0.1 -p 6000 -u username1
./hw3 client -i 127.0.0.1 -p 6000 -u username2