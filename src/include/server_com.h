#pragma once

#include <nlohmann/json.hpp>

#define MAX_HTTP_REQ_LEN 2048
#define MAX_BODY_LEN 512

void start_connection();

void send_register_request(nlohmann::json *credentials);
