#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
    char **cookies, int cookies_count) {
    char *message = new char[BUFLEN];
    char *line = new char[LINELEN];
    char *body_data_buffer = new char[LINELEN];

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    if (cookies != NULL) {
        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(body_data_buffer, cookies[i]);
            strcat(body_data_buffer, "; ");
        }
        if (cookies_count)
            strcat(body_data_buffer, cookies[cookies_count - 1]);

        sprintf(line, "Cookie: %s", body_data_buffer);
        compute_message(message, line);
    }

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char *content_type, char **body_data,
    int body_data_fields_count, char **cookies, int cookies_count) {
    char *message = new char[BUFLEN];
    char *line = new char[LINELEN];
    char *body_data_buffer = new char[LINELEN];

    for (int i = 0; i < body_data_fields_count - 1; i++) {
        strcat(body_data_buffer, body_data[i]);
        strcat(body_data_buffer, "&");
    }
    if (body_data_fields_count > 0) {
        strcat(body_data_buffer, body_data[body_data_fields_count - 1]);
    }

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);

    if (cookies != NULL) {
        char *cookies_buffer = new char[LINELEN];
        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(cookies_buffer, cookies[i]);
            strcat(cookies_buffer, "; ");
        }
        if (cookies_count)
            strcat(cookies_buffer, cookies[cookies_count - 1]);

        sprintf(line, "Cookie: %s", cookies_buffer);
        compute_message(message, line);
    }

    compute_message(message, "");
    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

    return message;
}
