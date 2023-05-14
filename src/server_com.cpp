#include "die.h"
#include "server_com.h"
#include "requests.h"
#include "helpers.h"

#include <unistd.h>
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
#define NOT_FOUND 404

static int sockfd;

static char *log_cookie = NULL;

static char *jwt_auth;

static int parse_response(char *response, char *cookie, char *auth,
	nlohmann::json *books, nlohmann::json *book) {

	int status = BAD_REQUEST;
	sscanf(response, "HTTP/1.1 %d", &status);

	if (status >= 400)
		return status;

	if (cookie) {
		response = strstr(response, "Set-Cookie: ");
		sscanf(response, "Set-Cookie: %[^\r]", cookie);
	}

	if (auth) {
		response = strstr(response, "token\":\"");
		sscanf(response, "token\":\"%[^\"]", auth);
	}

	if (books) {
		char books_buf[4096];
		response = strstr(response, "[");
		sscanf(response, "%[^\r ]", books_buf);
		*books = nlohmann::json::parse(books_buf);
	}

	if (book) {
		char book_buf[4096];
		response = strstr(response, "{");
		sscanf(response, "%[^\r ]", book_buf);
		*book = nlohmann::json::parse(book_buf);
	}

	return status;
}

static char *send_and_receive(char *message) {
	char *response;

send:
	send_to_server(sockfd, message);
	response = receive_from_server(sockfd);

	if (!strlen(response)) {
		close(sockfd);
		free(response);
		start_connection();
		goto send;
	}

	return response;
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
	if (!credentials)
		return;

	char *credentials_buf = new char[MAX_STR_LEN];
	strcpy(credentials_buf, credentials->dump().c_str());
	char *message = compute_post_request(SERVER_IP, "/api/v1/tema/auth/register", "application/json",
		NULL, &credentials_buf, 1, NULL, 0);

	char *response = send_and_receive(message);
	int status = parse_response(response, NULL, NULL, NULL, NULL);

	if (status != HTTP_CREATED) {
		printf("--User already register--\n");
	} else {
		printf("--Registered successfully--\n");
	}

	free(response);
	delete[] credentials_buf;
	delete[] message;
	delete credentials;
}

void send_login_request(nlohmann::json *credentials) {
	if (!credentials)
		return;

	char *credentials_buf = new char[MAX_STR_LEN];
	strcpy(credentials_buf, credentials->dump().c_str());
	char *message = compute_post_request(SERVER_IP, "/api/v1/tema/auth/login", "application/json",
		NULL, &credentials_buf, 1, NULL, 0);

	char *response = send_and_receive(message);
	if (log_cookie)
		logout();
	log_cookie = new char[MAX_STR_LEN];
	int status = parse_response(response, log_cookie, NULL, NULL, NULL);

	if (status != HTTP_OK) {
		printf("--Wrong credentials--\n");
		delete[] log_cookie;
		log_cookie = NULL;
	} else {
		printf("--Logged in--\n");
	}

	free(response);
	delete[] credentials_buf;
	delete[] message;
	delete credentials;
}

void send_access_request() {
	if (!log_cookie) {
		printf("--Not Logged in--\n");
		return;
	}

	char *message = compute_get_request(SERVER_IP, "/api/v1/tema/library/access", NULL, &log_cookie, 1);

	char *response = send_and_receive(message);
	jwt_auth = new char[MAX_STR_LEN];
	int status = parse_response(response, NULL, jwt_auth, NULL, NULL);

	if (status != HTTP_OK) {
		printf("--Wrong credentials--\n");
		delete[] jwt_auth;
		jwt_auth = NULL;
	} else {
		printf("--Received library access--\n");
	}

	free(response);
	delete[] message;
}

void send_books_request() {
	if (!log_cookie) {
		printf("--Not Logged in--\n");
		return;
	}
	if (!jwt_auth) {
		printf("--No library access--\n");
		return;
	}

	char *message = compute_get_request(SERVER_IP, "/api/v1/tema/library/books", jwt_auth, NULL, 1);

	char *response = send_and_receive(message);
	nlohmann::json books;
	int status = parse_response(response, NULL, NULL, &books, NULL);

	if (status != HTTP_OK) {
		printf("--Access denied--\n");
	} else {
		printf("--Books details--\n");
		printf("%s\n", books.dump(4).c_str());
	}

	free(response);
	delete[] message;
}

void send_book_request(int id) {
	if (id == -1)
		return;
	if (!log_cookie) {
		printf("--Not Logged in--\n");
		return;
	}
	if (!jwt_auth) {
		printf("--No library access--\n");
		return;
	}

	char url[MAX_STR_LEN] = { 0 };
	strcat(url, "/api/v1/tema/library/books/");
	sprintf(url + strlen(url), "%d", id);
	char *message = compute_get_request(SERVER_IP, url, jwt_auth, NULL, 1);

	char *response = send_and_receive(message);
	nlohmann::json book;

	int status = parse_response(response, NULL, NULL, NULL, &book);

	if (status == NOT_FOUND) {
		printf("--Book not found--\n");
	} else {
		printf("--Requested book--\n");
		printf("%s\n", book.dump(4).c_str());
	}

	free(response);
	delete[] message;
}

void send_add_book_request(nlohmann::json *book) {
	if (!book)
		return;
	if (!log_cookie) {
		printf("--Not Logged in--\n");
		delete book;
		return;
	}
	if (!jwt_auth) {
		printf("--No library access--\n");
		delete book;
		return;
	}

	char *book_buf = new char[MAX_STR_LEN];
	strcpy(book_buf, book->dump().c_str());
	char *message = compute_post_request(SERVER_IP, "/api/v1/tema/library/books", "application/json",
		jwt_auth, &book_buf, 1, NULL, 0);

	char *response = send_and_receive(message);
	int status = parse_response(response, NULL, NULL, NULL, NULL);

	if (status != HTTP_OK) {
		printf("--Error adding book--\n");
	} else {
		printf("--Added book--\n");
	}

	free(response);
	delete[] book_buf;
	delete[] message;
	delete book;
}

void send_delete_book_request(int id) {
	if (id == -1)
		return;
	if (!log_cookie) {
		printf("--Not Logged in--\n");
		return;
	}
	if (!jwt_auth) {
		printf("--No library access--\n");
		return;
	}

	char url[MAX_STR_LEN] = { 0 };
	strcat(url, "/api/v1/tema/library/books/");
	sprintf(url + strlen(url), "%d", id);
	char *message = compute_delete_request(SERVER_IP, url, jwt_auth);

	char *response = send_and_receive(message);
	int status = parse_response(response, NULL, NULL, NULL, NULL);

	if (status != HTTP_OK) {
		printf("--Error deleting book--\n");
	} else {
		printf("--Deleted book successfully--\n");
	}

	free(response);
	delete[] message;
}

void logout() {
	if (!log_cookie) {
		printf("--Not Logged in--\n");
		return;
	}

	char *message = compute_get_request(SERVER_IP, "/api/v1/tema/auth/logout", NULL, &log_cookie, 1);

	char *response = send_and_receive(message);
	int status = parse_response(response, NULL, NULL, NULL, NULL);

	if (status > 400) {
		printf("--Not Logged in--\n");
	} else {
		printf("--Logged out--\n");
	}

	free(response);
	delete[] message;
	delete[] jwt_auth;
	jwt_auth = NULL;
	delete[] log_cookie;
	log_cookie = NULL;
}
