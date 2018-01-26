//
// Created by artem on 24.01.18.
//
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "lib.h"
#include <cjson/cJSON.h>

int main(int argc, char** argv) {
    enum {
        BOB_ETH_DEPOSIT,
        BOB_ERC20_DEPOSIT,
        BOB_CLAIMS_DEPOSIT,
        ALICE_CLAIMS_DEPOSIT,
        BOB_ETH_PAYMENT,
        BOB_ERC20_PAYMENT,
        BOB_CLAIMS_PAYMENT,
        ALICE_CLAIMS_PAYMENT,
        BOB_APPROVES_ERC20
    };
    if (argc < 3) {
        return 1;
    }
    int action = atoi(argv[1]);
    char tx[1000];
    switch (action)
    {
        case BOB_ETH_DEPOSIT:
            bobSendsEthDeposit(
                    argv[2],
                    "1000000000000000000",
                    getenv("BOB_PK"),
                    tx,
                    "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a",
                    argv[3],
                    atoi(argv[4])
            );
            break;
        case BOB_ERC20_DEPOSIT:
            aliceInitsErc20Deal(
                    argv[2],
                    "1000000000000000000",
                    "0xc0eb7AeD740E1796992A08962c15661bDEB58003",
                    getenv("ALICE_PK"),
                    tx,
                    "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41",
                    argv[3],
                    argv[4],
                    atoi(argv[5])
            );
            break;
        case BOB_CLAIMS_DEPOSIT:
            bobClaimsDeposit(
                    argv[2],
                    "1000000000000000000",
                    argv[3],
                    getenv("BOB_PK"),
                    tx,
                    "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a",
                    argv[4],
                    argv[5],
                    atoi(argv[6])
            );
            break;
        case ALICE_CLAIMS_DEPOSIT:
            bobClaimsPayment(
                    argv[2],
                    "1000000000000000000",
                    argv[3],
                    getenv("BOB_PK"),
                    tx,
                    "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a",
                    argv[4],
                    argv[5],
                    atoi(argv[6])
            );
            break;
        case BOB_ETH_PAYMENT:
            aliceInitsEthDeal(
                    argv[2],
                    "1000000000000000000",
                    getenv("ALICE_PK"),
                    tx,
                    "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41",
                    argv[3],
                    argv[4],
                    atoi(argv[5])
            );
            break;
        case BOB_ERC20_PAYMENT:
            aliceInitsErc20Deal(
                    argv[2],
                    "1000000000000000000",
                    "0xc0eb7AeD740E1796992A08962c15661bDEB58003",
                    getenv("ALICE_PK"),
                    tx,
                    "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41",
                    argv[3],
                    argv[4],
                    atoi(argv[5])
            );
            break;
        case BOB_CLAIMS_PAYMENT:
            aliceClaimsPayment(
                    argv[2],
                    "1000000000000000000",
                    "0xc0eb7AeD740E1796992A08962c15661bDEB58003",
                    getenv("ALICE_PK"),
                    tx,
                    "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41",
                    argv[3],
                    argv[4],
                    atoi(argv[5])
            );
            break;
        case ALICE_CLAIMS_PAYMENT:
            bobClaimsPayment(
                    argv[2],
                    "1000000000000000000",
                    argv[3],
                    getenv("BOB_PK"),
                    tx,
                    "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a",
                    argv[4],
                    argv[5],
                    atoi(argv[6])
            );
            break;
        case BOB_APPROVES_ERC20:
            approveErc20(
                    "10000000000000000000",
                    "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41",
                    getenv("BOB_PK"),
                    tx,
                    atoi(argv[2])
            );
            break;
        default:
            return 1;
    }
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
