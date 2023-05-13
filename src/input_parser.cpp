#include "input_parser.h"

#include <stdio.h>
#include <string.h>
#include <nlohmann/json.hpp>

#define REG_CMD "register"
#define LOG_CMD "login"
#define ENTER_LIB_CMD "enter_library"
#define GET_BOOKS_CMD "get_books"
#define GET_BOOK_CMD "get_book"
#define ADD_BOOK_CMD "add_book"
#define DELETE_BOOK_CMD "delete_book"
#define LOGOUT_CMD "logout"
#define EXIT_CMD "exit"

#define INVALID_FORMAT(correct_format) do 					\
{ 															\
	printf("Invalid input should be:%s\n", correct_format);	\
} while (0);												\

#define MAX_LINE 512

cmd_type get_command_type(char *command) {
	if (!strncmp(command, REG_CMD, strlen(REG_CMD)))
		return REGISTER;

	if (!strncmp(command, LOG_CMD, strlen(LOG_CMD)))
		return LOGIN;

	if (!strncmp(command, ENTER_LIB_CMD, strlen(ENTER_LIB_CMD)))
		return ENTER_LIBRARY;

	if (!strncmp(command, GET_BOOKS_CMD, strlen(GET_BOOKS_CMD)))
		return GET_BOOKS;

	if (!strncmp(command, GET_BOOK_CMD, strlen(GET_BOOK_CMD)))
		return GET_BOOK;

	if (!strncmp(command, ADD_BOOK_CMD, strlen(ADD_BOOK_CMD)))
		return ADD_BOOK;

	if (!strncmp(command, DELETE_BOOK_CMD, strlen(DELETE_BOOK_CMD)))
		return DELETE_BOOK;

	if (!strncmp(command, LOGOUT_CMD, strlen(LOGOUT_CMD)))
		return LOGOUT;

	if (!strncmp(command, EXIT_CMD, strlen(EXIT_CMD)))
		return EXIT;

	printf("Invalid input\n");
	return INVALID_CMD;
}

nlohmann::json *parse_credentials() {
	int rc;
	bool wrong_format = false;
	char line[MAX_LINE];
	char username[MAX_LINE];
	char password[MAX_LINE];
	nlohmann::json *result = new nlohmann::json;

	printf("username=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%s", username);
	if (rc != 1) {
		INVALID_FORMAT("A SINGLE STRING");
		wrong_format = true;
	}
	(*result)["username"] = username;

	printf("password=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%s", password);
	if (rc != 1) {
		INVALID_FORMAT("A SINGLE STRING");
		wrong_format = true;
	}
	(*result)["password"] = password;

	if (wrong_format) {
		delete result;
		return NULL;
	}

	return result;
}

nlohmann::json *parse_book_details() {
	int rc;
	bool wrong_format = false;
	char line[MAX_LINE];
	char title[MAX_LINE];
	char author[MAX_LINE];
	char genre[MAX_LINE];
	char publisher[MAX_LINE];
	int page_count;
	nlohmann::json *result = new nlohmann::json;

	printf("title=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%[^\n]", title);
	if (rc != 1) {
		INVALID_FORMAT("A SINGLE OR MULTIPLE STRINGS");
		wrong_format = true;
	}
	(*result)["title"] = title;

	printf("author=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%s\n", author);
	if (rc != 1) {
		INVALID_FORMAT("A SINGLE STRING");
		wrong_format = true;
	}
	(*result)["author"] = author;

	printf("genre=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%s", genre);
	if (rc != 1) {
		INVALID_FORMAT("A SINGLE STRING");
		wrong_format = true;
	}
	(*result)["genre"] = genre;

	printf("publisher=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%s", publisher);
	if (rc != 1) {
		INVALID_FORMAT("A SINGLE STRING");
		wrong_format = true;
	}
	(*result)["publisher"] = publisher;

	printf("page_count=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%d", &page_count);
	if (rc != 1) {
		INVALID_FORMAT("NUMBER");
		wrong_format = true;
	}
	(*result)["page_count"] = page_count;

	if (wrong_format) {
		delete result;
		return NULL;
	}

	return result;
}

int parse_book_id() {
	int rc;
	char line[MAX_LINE];
	int book_id;

	printf("id=");
	fgets(line, MAX_LINE, stdin);
	rc = sscanf(line, "%d", &book_id);
	if (rc != 1) {
		INVALID_FORMAT("NUMBER");
		return -1;
	}

	return book_id;
}
