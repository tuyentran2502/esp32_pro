#ifndef HTTP_SERVER_APP_H
#define HTTP_SERVER_APP_H

typedef void (*http_handle_data_t)(char *buf);
void start_webserver(void);
void stop_webserver(void);
void http_setCallback(void* callback);
int json_login_deserialize(char *input_data, char *user, char *password);

#endif /* HTTP_SERVER_APP_H */
