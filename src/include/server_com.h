#pragma once

#include <nlohmann/json.hpp>

#define MAX_HTTP_REQ_LEN 2048
#define MAX_STR_LEN 512

void start_connection();

void send_register_request(nlohmann::json *credentials);

void send_login_request(nlohmann::json *credentials);

void send_access_request();

void send_books_request();

void send_book_request(int id);

void send_add_book_request(nlohmann::json *book);

void send_delete_book_request(int id);

void logout();
