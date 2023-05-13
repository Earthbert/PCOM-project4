#include "die.h"
#include "server_com.h"
#include "requests.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

#define SERVER_IP "34.254.242.81"
#define SERVER_PORT ((uint16_t)8080)

#define HTTP_CREATED 201
#define HTTP_OK 200
#define BAD_REQUEST 400

static int sockfd;

static char *log_cookie = NULL;

static int parse_response(char *response, char *cookie) {
	int status;
	sscanf(response, "HTTP/1.1 %d", &status);

	if (cookie != NULL) {
		response = strstr(response, "Set-Cookie: connect.sid=");
		sscanf(response, "Set-Cookie: connect.sid=%[^;]", cookie);
	}

	assert(status != BAD_REQUEST);
	return status;
}

void start_connection() {
	int ret;
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket()");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_IP, &serv_addr.sin_addr);

	ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	DIE(ret, "connect()");
}

void send_register_request(nlohmann::json *credentials) {
	char *message;
	char *credentials_buf = new char[MAX_BODY_LEN];
	strcpy(credentials_buf, credentials->dump().c_str());

	message = compute_post_request(SERVER_IP, "/api/v1/tema/auth/register", "application/json",
		&credentials_buf, 1, NULL, 0);

	send_to_server(sockfd, message);

	char *response = receive_from_server(sockfd);

	if (parse_response(response, NULL) != HTTP_CREATED) {
		printf("User already register\n");
	}

	free(response);
	delete[] credentials_buf;
	delete[] message;
	delete credentials;
}

void send_login_request(nlohmann::json *credentials) {
	char *message;
	char *credentials_buf = new char[MAX_BODY_LEN];
	strcpy(credentials_buf, credentials->dump().c_str());

	message = compute_post_request(SERVER_IP, "/api/v1/tema/auth/login", "application/json",
		&credentials_buf, 1, NULL, 0);

	send_to_server(sockfd, message);

	char *response = receive_from_server(sockfd);
	log_cookie = new char[MAX_BODY_LEN];

	if (parse_response(response, log_cookie) != HTTP_OK) {
		printf("Wrong credentials\n");
	}

	free(response);
	delete[] credentials_buf;
	delete[] message;
	delete credentials;
}