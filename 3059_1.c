#include <stdio.h>
#include <curl/curl.h>

int vulnerable_server_request(const char *url) {
    CURL *curl = curl_easy_init();
    if (!curl) return -1;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    CURLcode res_curl = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res_curl != CURLE_OK) return -1;
    return 0;
}