//
// Created by artem on 24.01.18.
//
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "lib.h"
#include <cjson/cJSON.h>

char* bobContractAddress = "0x9387Fd3a016bB0205e4e131Dde886B9d2BC000A2";
char* aliceAddress = "0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a";
char* bobAddress = "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41";
char* tokenAddress = "0xc0eb7AeD740E1796992A08962c15661bDEB58003";

int main(int argc, char** argv)
{
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
    BasicTxData txData;
    char signedTx[1000];
    switch (action)
    {
        case BOB_ETH_DEPOSIT:
            txData.amount = "1000000000000000000";
            txData.from = bobAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobMakesEthDepositInput input = {
                .aliceAddress = aliceAddress,
                .depositId = argv[3],
                .bobHash = argv[4]
            };

            bobMakesEthDeposit(input, txData, signedTx);
            break;
        case BOB_ERC20_DEPOSIT:
            txData.amount = "0";
            txData.from = bobAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobMakesErc20DepositInput input1 = {
                .depositId = argv[3],
                .amount = "1000000000000000000",
                .aliceAddress = aliceAddress,
                .bobHash = argv[4],
                .tokenAddress = tokenAddress
            };

            bobMakesErc20Deposit(input1, txData, signedTx);
            break;
        case BOB_CLAIMS_DEPOSIT:
            txData.amount = "0";
            txData.from = bobAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobClaimsDepositInput input2 = {
                .depositId = argv[3],
                .amount = "1000000000000000000",
                .aliceAddress = aliceAddress,
                .tokenAddress = argv[4],
                .aliceCanClaimAfter = argv[5],
                .bobSecret = argv[6]
            };

            bobClaimsDeposit(input2, txData, signedTx);
            break;
        case ALICE_CLAIMS_DEPOSIT:
            txData.amount = "0";
            txData.from = aliceAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("ALICE_PK");
            txData.nonce = atoi(argv[2]);

            AliceClaimsBobDepositInput input3 = {
                .depositId = argv[3],
                .amount = "1000000000000000000",
                .bobAddress = bobAddress,
                .tokenAddress = argv[4],
                .aliceCanClaimAfter = argv[5],
                .bobHash = argv[6]
            };

            aliceClaimsBobDeposit(input3, txData, signedTx);
            break;
        case BOB_ETH_PAYMENT:
            txData.amount = "1000000000000000000";
            txData.from = bobAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobMakesEthPaymentInput input4 = {
                .paymentId = argv[3],
                .aliceHash = argv[4],
                .aliceAddress = aliceAddress
            };

            bobMakesEthPayment(input4, txData, signedTx);
            break;
        case BOB_ERC20_PAYMENT:
            txData.amount = "0";
            txData.from = bobAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobMakesErc20PaymentInput input5 = {
                .paymentId = argv[3],
                .amount = "1000000000000000000",
                .tokenAddress = tokenAddress,
                .aliceAddress = aliceAddress,
                .aliceHash = argv[4]
            };

            bobMakesErc20Payment(input5, txData, signedTx);
            break;
        case BOB_CLAIMS_PAYMENT:
            txData.amount = "0";
            txData.from = bobAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("BOB_PK");
            txData.nonce = atoi(argv[2]);

            BobClaimsBobPaymentInput input6 = {
                .paymentId = argv[3],
                .aliceAddress = aliceAddress,
                .amount = "1000000000000000000",
                .tokenAddress = argv[4],
                .bobCanClaimAfter = argv[5],
                .aliceHash = argv[6]
            };

            bobClaimsBobPayment(input6, txData, signedTx);
            break;
        case ALICE_CLAIMS_PAYMENT:
            txData.amount = "0";
            txData.from = aliceAddress;
            txData.to = bobContractAddress;
            txData.secretKey = getenv("ALICE_PK");
            txData.nonce = atoi(argv[2]);

            AliceClaimsBobPaymentInput input7 = {
                .paymentId = argv[3],
                .bobAddress = bobAddress,
                .amount = "1000000000000000000",
                .tokenAddress = argv[4],
                .bobCanClaimAfter = argv[5],
                .aliceSecret = argv[6]
            };

            aliceClaimsBobPayment(input7, txData, signedTx);
            break;
        case BOB_APPROVES_ERC20:
            approveErc20(
                    "10000000000000000000",
                    "0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41",
                    getenv("BOB_PK"),
                    signedTx,
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
    cJSON_Delete(request);
    return 0;
}
