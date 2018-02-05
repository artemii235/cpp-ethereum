#include "etomiccurl.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <memory.h>
#include <stdlib.h>

static char* ethRpcUrl = "https://ropsten.infura.io/y07GHxUyTgeN2mdfOonu";

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

char* sendRequest(char* request)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    if (curl) {
        struct string s;
        init_string(&s);

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_URL, ethRpcUrl);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        return s.ptr;
    } else {
        return NULL;
    }
}

char* sendRawTx(char* rawTx)
{
    char* string;
    cJSON *request = cJSON_CreateObject();
    cJSON *params = cJSON_CreateArray();
    cJSON_AddItemToObject(request, "jsonrpc", cJSON_CreateString("2.0"));
    cJSON_AddItemToObject(request, "method", cJSON_CreateString("eth_sendRawTransaction"));
    cJSON_AddItemToArray(params, cJSON_CreateString(rawTx));
    cJSON_AddItemToObject(request, "params", params);
    cJSON_AddItemToObject(request, "id", cJSON_CreateNumber(2));
    string = cJSON_PrintUnformatted(request);
    char* requestResult = sendRequest(string);
    cJSON *json = cJSON_Parse(requestResult);
    cJSON_Delete(request);
    char* tmp = cJSON_GetObjectItemCaseSensitive(json, "result")->valuestring;
    char* txId = (char*)malloc(strlen(tmp) + 1);
    strcpy(txId, tmp);
    free(requestResult);
    return txId;
}

int getNonce(char* address)
{
    char* string;
    cJSON *request = cJSON_CreateObject();
    cJSON *params = cJSON_CreateArray();
    cJSON_AddItemToObject(request, "jsonrpc", cJSON_CreateString("2.0"));
    cJSON_AddItemToObject(request, "method", cJSON_CreateString("eth_getTransactionCount"));
    cJSON_AddItemToArray(params, cJSON_CreateString(address));
    cJSON_AddItemToArray(params, cJSON_CreateString("pending"));
    cJSON_AddItemToObject(request, "params", params);
    cJSON_AddItemToObject(request, "id", cJSON_CreateNumber(2));
    string = cJSON_PrintUnformatted(request);
    char* requestResult = sendRequest(string);
    cJSON_Delete(request);
    cJSON *json = cJSON_Parse(requestResult);
    int nonce = (int)strtol(cJSON_GetObjectItemCaseSensitive(json, "result")->valuestring, NULL, 0);
    cJSON_Delete(json);
    free(requestResult);
    return nonce;
}
