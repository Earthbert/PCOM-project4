#include "input_parser.h"
#include "server_com.h"

#include <nlohmann/json.hpp>
#include <stdio.h>

int main() {
	char line[MAX_COMMAND_LEN];
	start_connection();

	for (;;) {
		fgets(line, MAX_COMMAND_LEN, stdin);

		switch (get_command_type(line)) {
		case REGISTER: {
			nlohmann::json *credentials = parse_credentials();
			send_register_request(credentials);
			break;
		}
		case LOGIN: {
			nlohmann::json *credentials = parse_credentials();
			send_login_request(credentials);
			break;
		}
		case ENTER_LIBRARY: {
			send_access_request();
			break;
		}
		case GET_BOOKS: {
			send_books_request();
			break;
		}
		case GET_BOOK: {
			int book_id = parse_book_id();
			send_book_request(book_id);
			break;
		}
		case ADD_BOOK: {
			nlohmann::json *book = parse_book_details();
			send_add_book_request(book);
			break;
		}
		case DELETE_BOOK: {
			int book_id = parse_book_id();
			send_delete_book_request(book_id);
			break;
		}
		case LOGOUT: {
			logout();
			break;
		}
		case EXIT: {
			logout();
			goto exit;
		}
		default:
			break;
		}
	}

exit:

	return 0;
}
