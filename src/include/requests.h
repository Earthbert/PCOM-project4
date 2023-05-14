#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (auth
// and cookies can be set to NULL if not needed)
char *compute_get_request(const char *host, const char *url, char *auth,
	char **cookies, int cookies_count);

// computes and returns a POST request string (cookies and auth can be NULL if not needed)
char *compute_post_request(const char *host, const char *url, const char *content_type, char *auth, char **body_data,
    int body_data_fields_count, char **cookies, int cookies_count);

// computes and returns a DELETE request string
char *compute_delete_request(const char *host, const char *url, const char *auth);

#endif
