//
// Created by artem on 24.01.18.
//
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "lib.h"
#include <cjson/cJSON.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        return 1;
    }

    char tx[1000];
    signTx(atoi(argv[1]), argv[2], tx);
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    char* string;
    cJSON *request = cJSON_CreateObject();
    cJSON *params = cJSON_CreateArray();
    cJSON_AddItemToObject(request, "jsonrpc", cJSON_CreateString("2.0"));
    cJSON_AddItemToObject(request, "method", cJSON_CreateString("eth_sendRawTransaction"));
    cJSON_AddItemToArray(params, cJSON_CreateString(tx));
    cJSON_AddItemToObject(request, "params", params);
    cJSON_AddItemToObject(request, "id", cJSON_CreateNumber(2));
    string = cJSON_PrintUnformatted(request);
    fprintf(stdout, "%s\n", string);
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://ropsten.infura.io/y07GHxUyTgeN2mdfOonu");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, string);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    cJSON_Delete(request);
    return 0;
}
