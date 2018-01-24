//
// Created by artem on 24.01.18.
//
#include "lib.h"
#include <iostream>
#include <libethcore/Common.h>
#include <libethcore/CommonJS.h>
#include <libethcore/TransactionBase.h>
#include <libdevcore/Common.h>
using namespace dev;
using namespace dev::eth;

void signTx(int nonce, char* secret, char* buffer) {
    TransactionSkeleton tx;
    tx.to = jsToAddress("0xA7EF3f65714AE266414C9E58bB4bAa4E6FB82B41");
    tx.from = jsToAddress("0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a");
    tx.value = exp10<18>();
    tx.nonce = nonce;
    tx.gas = 300000;
    tx.gasPrice = 100 * exp10<9>();
    TransactionSkeleton& txRef = tx;
    Secret* secret1 = new Secret(secret);
    Secret& secretRef = *secret1;
    auto baseTx = new TransactionBase(txRef, secretRef);
    auto rlpStream = new RLPStream();
    RLPStream& rlpStream1 = *rlpStream;
    baseTx->streamRLP(rlpStream1);
    std::stringstream ss;
    ss << rlpStream1.out();
    const std::string tmp = ss.str();
    if (strlen(tmp.c_str()) < 1000) {
        strncpy(buffer, tmp.c_str(), 1000);
    }
}
