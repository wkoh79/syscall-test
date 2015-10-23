#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "measure.h"

#define ITERATION 1000
#define BUFF_SIZE 1024

/* reference */
/* http://forum.falinux.com/zbxe/index.php?document_srl=441104&mid=C_LIB */

int main(int argc, char *argv[])
{
    long i;
    int client_socket;
    struct sockaddr_in server_addr;

    char buff[BUFF_SIZE + 5];
    int input_string_length;

    if (argc < 2) {
        fprintf(stderr, "usage: %s data_to_send\n", argv[0]);
        exit(1);
    }

    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if( client_socket == -1 ){
	printf("Create Socket Failure\n");
	exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family	= AF_INET;
    server_addr.sin_port	= htons(10000);
    server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");	//Loopback
    if( connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1 ){
    	printf("Connection Failure\n");
	exit(1);
    }

    input_string_length = strlen(argv[1]) + 1;

    for (i = 0; i < ITERATION; i++)
        MEASURE_ITER("send", i, {send(client_socket, argv[1], input_string_length, 0);});

    close(client_socket);

    return 0;
}
