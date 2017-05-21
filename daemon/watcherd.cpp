#include <iostream>
#include <chrono>
#include <thread>
#include <curl/curl.h>

#include "watcherd.h"
#include "daemon.h"


size_t writer_cb(void *buffer, size_t size, size_t nmemb, void *writer_data)
{
    size_t real_size = size * nmemb;

    if(writer_data == NULL){ return 0; }
    ((std::string *)writer_data)->append((char *)buffer, real_size);

    return real_size;
}


/* 
 * libcurl variable for error / returned data
 */
static char errorBuffer[CURL_ERROR_SIZE];

static void curl_init(CURL *&conn, std::string& url, std::string *data)
{
    CURLcode code;
    
    if((conn = curl_easy_init()) == NULL) { throw std::runtime_error("Failed to connect"); }

    code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
    if(code != CURLE_OK) { throw std::runtime_error("Failed to set error buffer"); }

    code = curl_easy_setopt(conn, CURLOPT_URL, url.c_str());
    if(code != CURLE_OK) { throw std::runtime_error("Failed to set url"); }

    code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer_cb);
    if(code != CURLE_OK) { throw std::runtime_error("Failed to set writer"); }

    code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, (void *)data);
    if(code != CURLE_OK) { throw std::runtime_error("Failed to set write data"); }
}

void job()
{

    std::string data("");

    CURL *conn = NULL;
    std::string url("https://timetable.iit.artsci.utoronto.ca/api/lastupdated");
    CURLcode code = CURLE_OK;

    curl_init(conn, url, &data);

    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if(code != CURLE_OK)
    {
        throw std::runtime_error("Failed to get response");
    }

    std::cout << data << std::endl;
    curl_global_cleanup();
}


// void job()
// {
//     int i = 0;
//     while(i++ < 100)
//     {
//         syslog(LOG_NOTICE, "Loop #%d", i);
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
// }

int main(int argc, char **argv)
{
    unused(argc, argv);

    try
    {
        job();
    } catch(const std::runtime_error& e){
        std::cout << e.what() <<std::endl;
    }

    // setlogmask(LOG_UPTO(LOG_INFO));
    // openlog(DEFAULT_DCONFIG_NAME, LOG_PID, LOG_DAEMON);
    //
    // try
    // {
    //     std::string conf_path("/Users/markwang/github/EnrollmentWatchUofT/daemon/.daemon.conf");
    //     Daemon& d = Daemon::get_daemon(conf_path);
    //     d.work_on(job);
    // }
    // catch(const DaemonRuntimeException& e)
    // {
    //     syslog(LOG_ERR, "%s", e.what());
    // }

    exit(EXIT_SUCCESS);
}
