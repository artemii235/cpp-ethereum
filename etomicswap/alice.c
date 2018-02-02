//
// Created by artem on 24.01.18.
//
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "etomiclib.h"
#include <cjson/cJSON.h>

char* aliceContractAddress = "0xe1D4236C5774D35Dc47dcc2E5E0CcFc463A3289c";
char* aliceAddress = "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a";
char* bobAddress = "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41";
char* tokenAddress = "0xc0eb7AeD740E1796992A08962c15661bDEB58003";

int main(int argc, char** argv) {
    enum { INIT_ETH, INIT_ERC20, ALICE_CLAIMS, BOB_CLAIMS, ALICE_APPROVES_ERC20 };

    if (argc < 2) {
        return 1;
    }

    int action = atoi(argv[1]);
    char* signedTx;
    BasicTxData txData;
    switch (action)
    {
        case INIT_ETH:
            txData.amount = "1000000000000000000";
            txData.from = aliceAddress;
            txData.to = aliceContractAddress;
            txData.secretKey = getenv("ALICE_PK");
            txData.nonce = atoi(argv[2]);

            AliceSendsEthPaymentInput input = {
                .dealId = argv[3],
                .bobAddress = bobAddress,
                .aliceHash = argv[4],
                .bobHash = argv[5]
            };

            signedTx = aliceSendsEthPayment(input, txData);
            break;
        case INIT_ERC20:
            txData.amount = "0";
            txData.from = aliceAddress;
            txData.to = aliceContractAddress;
            txData.secretKey = getenv("ALICE_PK");
            txData.nonce = atoi(argv[2]);

            AliceSendsErc20PaymentInput input1 = {
                .dealId = argv[3],
                .bobAddress = bobAddress,
                .aliceHash = argv[4],
                .bobHash = argv[5],
                .amount = "1000000000000000000",
                .tokenAddress = tokenAddress
            };

            signedTx = aliceSendsErc20Payment(input1, txData);
            break;
        case ALICE_CLAIMS:
            txData.amount = "0";
            txData.from = aliceAddress;
            txData.to = aliceContractAddress;
            txData.secretKey = getenv("ALICE_PK");
            txData.nonce = atoi(argv[2]);

            AliceReclaimsAlicePaymentInput input2 = {
                .dealId = argv[3],
                .bobAddress = bobAddress,
                .aliceHash = argv[4],
                .bobSecret = argv[5],
                .tokenAddress = argv[6],
                .amount = "1000000000000000000"
            };

            signedTx = aliceReclaimsAlicePayment(input2, txData);
            break;
        case BOB_CLAIMS:
            txData.amount = "0";
            txData.from = bobAddress;
            txData.to = aliceContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobSpendsAlicePaymentInput input3 = {
                .dealId = argv[3],
                .aliceAddress = aliceAddress,
                .aliceSecret = argv[4],
                .bobHash = argv[5],
                .tokenAddress = argv[6],
                .amount = "1000000000000000000"
            };

            signedTx = bobSpendsAlicePayment(input3, txData);
            break;
        case ALICE_APPROVES_ERC20:
            signedTx = approveErc20(
                    "1000000000000000000",
                    "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a",
                    getenv("ALICE_PK"),
                    atoi(argv[1])
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
    cJSON_AddItemToArray(params, cJSON_CreateString(signedTx));
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
    free(signedTx);
    cJSON_Delete(request);
    return 0;
}
