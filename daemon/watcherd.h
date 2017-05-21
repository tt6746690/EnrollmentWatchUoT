#ifdef _WATCHERD_H_
#define _WATCHERD_H_


#define MY_DAEMON_CONF_PATH "/Users/markwang/github/EnrollmentWatchUofT/daemon/.daemon.conf"

void job();
/*
 * Writer callback invoked as soon as data is received
 * -- buffer is pointer to saved data, where its size is size * nmemb
 * -- userp: data pointer, defaults to FILE * to stdout
 * Return
 * -- number of byte of data taken care of
 * -- difference in size of in/out will trigger error
 */
size_t writer_cb(void *buffer, size_t size, size_t nmemb, void *userp);


/*
 * Initializes curl connection, sets 
 * -- url
 * -- errorbuffer
 * -- writer callback function
 * -- write data
 */
static void curl_init(CURL *&conn, char* url, std::string *data);

#endif
