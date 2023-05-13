#include "input_parser.h"
#include "server_com.h"

#include <nlohmann/json.hpp>
#include <stdio.h>

int main() {
	char command[MAX_COMMAND_LEN];
	start_connection();

	for (;;) {
		fgets(command, MAX_COMMAND_LEN, stdin);

		switch (get_command_type(command)) {
		case REGISTER: {
			nlohmann::json *res = parse_credentials();
			break;
		}
		case LOGIN: {
			nlohmann::json *res = parse_credentials();
			break;
		}
		case ENTER_LIBRARY: {
			break;
		}
		case GET_BOOKS: {
			break;
		}
		case GET_BOOK: {
			int book_id = parse_book_id();
			break;
		}
		case ADD_BOOK: {
			nlohmann::json *res = parse_book_details();
			break;
		}
		case DELETE_BOOK: {
			int book_id = parse_book_id();
			break;
		}
		case LOGOUT: {
			break;
		}
		case EXIT: {
			goto exit;
		}
		default:
			break;
		}
	}

exit:

	return 0;
}
