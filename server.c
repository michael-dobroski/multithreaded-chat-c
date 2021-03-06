#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#include "commands.h"

#define PORT 8080



void run_server(int sockfd, int num_threads) {
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
	int valread;
	char delim[] = " ";
	char out[4096] = { 0 };
	char packet_copy[256] = { 0 };

    while(1) {
        // Accept the data packet from client and verification
        int client_socket = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (client_socket < 0) {
            printf("server accept failed...\n");
            exit(0);
        } else {
            printf("server accept the client...\n");

			// read client packet
			char packet[256] = { 0 };
			valread = read(client_socket, packet, 256);
			strncpy(packet_copy, packet, sizeof(packet_copy) - 1);
			packet_copy[sizeof(packet_copy) - 1] = '\0';

			// print packet contents
			printf("Packet contents -> %s\n", packet);

			// parse and evaluate packet
			char *ptr = strtok(packet, delim);
			if (strcmp(ptr, "0") == 0) { // client requests message retrieval
				ptr = strtok(NULL, delim);
				const char *channel_name;
				channel_name = ptr;
				channel_t *c = get_channel(get_channels(), channel_name);
				if (c == NULL) {
					strcpy(out, "1");
				} else {
					ptr = strtok(NULL, delim);
					char *trash;
					long msg_id;
					msg_id = strtoul(ptr, &trash, 10);
					message_t *m = get_message(c, msg_id);
					if (m == NULL) {
						strcpy(out, "2");
					} else {
						strcpy(out, "0 ");
						strcat(out, m->text);
					}
				}
			} else if (strcmp(ptr, "1") == 0) { // client requests entire channel retreival
				ptr = strtok(NULL, delim);
				const char *channel_name;
				channel_name = ptr;
				channel_t *c = get_channel(get_channels(), channel_name);
				if (c == NULL) {
					strcpy(out, "1");
				} else {
					strcpy(out, "");
					int i = 0;
					char str[5];
					for (message_t *m = c->head; m != NULL; m = m->next) {
						strcat(out, "\t[");
						sprintf(str, "%d", i);
						i++;
						strcat(out, str);
						strcat(out, "]: ");
						strcat(out, m->text);
						strcat(out, "\n");
					}
				}
			} else if (strcmp(ptr, "2") == 0) {
				ptr = strtok(NULL, delim);
				const char *channel_name;
				channel_name = ptr;
				channel_t *c = get_channel(get_channels(), channel_name);
				if (c == NULL) {
					c = create_channel(get_channels(), channel_name);
				}
				memmove(packet_copy, packet_copy+strlen(channel_name)+3, strlen(packet_copy) - strlen(channel_name) - 3 + 1);
				const char *txt_contents;
				txt_contents = packet_copy;
				add_message(c, txt_contents);
				strcpy(out, "Successfully added message < ");
				strcat(out, txt_contents);
				strcat(out, " > to channel < ");
				strcat(out, channel_name);
				strcat(out, " >\n");
			}

			// return parsed packet
			send(client_socket, out, strlen(out), 0);

        }
    }
}




int main() {
	channel_list_t *channels = get_channels();
	channel_t *weather = create_channel(channels, "weather");
	channel_t *new = create_channel(channels, "news");

	add_message(weather, "the weather is going to be great!");
	add_message(weather, "sunny with a high of 70F");
	dump(channels);


	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	} else {
		printf("Socket successfully created..\n");
    }
    
	// assign IP, PORT
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	} else {
		printf("Socket successfully binded..\n");
    }

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	} else {
		printf("Server listening..\n");
    }
	
    run_server(sockfd, 10);



	// After chatting close the socket
	close(sockfd);
}
