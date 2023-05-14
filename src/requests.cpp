#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(const char *host, const  char *url, char *auth, char **cookies, int cookies_count) {
    char *message = new char[BUFLEN]();
    char *line = new char[LINELEN]();
    char *body_data_buffer = new char[LINELEN]();

    sprintf(line, "GET %s HTTP/1.1", url);

    compute_message(message, line);

    if (cookies) {
        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(body_data_buffer, cookies[i]);
            strcat(body_data_buffer, "; ");
        }
        if (cookies_count)
            strcat(body_data_buffer, cookies[cookies_count - 1]);

        sprintf(line, "Cookie: %s", body_data_buffer);
        compute_message(message, line);
    }

    if (auth) {
        sprintf(line, "Authorization: Bearer %s", auth);
        compute_message(message, line);
    }

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    compute_message(message, "");

    delete[] line;
    delete[] body_data_buffer;

    return message;
}

char *compute_post_request(const char *host, const char *url, const char *content_type, char *auth, char **body_data,
    int body_data_fields_count, char **cookies, int cookies_count) {
    char *message = new char[BUFLEN]();
    char *line = new char[LINELEN]();
    char *body_data_buffer = new char[LINELEN]();

    for (int i = 0; i < body_data_fields_count - 1; i++) {
        strcat(body_data_buffer, body_data[i]);
        strcat(body_data_buffer, "&");
    }
    if (body_data_fields_count > 0) {
        strcat(body_data_buffer, body_data[body_data_fields_count - 1]);
    }

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);

    if (auth) {
        sprintf(line, "Authorization: Bearer %s", auth);
        compute_message(message, line);
    }

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
        delete[] cookies_buffer;
    }

    compute_message(message, "");
    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);
    compute_message(message, "");

    delete[] line;
    delete[] body_data_buffer;

    return message;
}

char *compute_delete_request(const char *host, const char *url, const char *auth) {
    char *message = new char[BUFLEN]();
    char *line = new char[LINELEN]();

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Authorization: Bearer %s", auth);
    compute_message(message, line);

    compute_message(message, "");

    delete[] line;

    return message;
}
