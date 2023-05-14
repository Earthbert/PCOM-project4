#pragma once

#include <nlohmann/json.hpp>

#define MAX_COMMAND_LEN 128

enum cmd_type {
	REGISTER,
	LOGIN,
	ENTER_LIBRARY,
	GET_BOOKS,
	GET_BOOK,
	ADD_BOOK,
	DELETE_BOOK,
	LOGOUT,
	EXIT,
	INVALID_CMD
};

cmd_type get_command_type();

nlohmann::json *parse_credentials();

nlohmann::json *parse_book_details();

int parse_book_id();