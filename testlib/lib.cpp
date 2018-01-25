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

void signTx(TransactionSkeleton& tx, char* secret, char* buffer) {
    Secret* secret1 = new Secret(secret);
    Secret& secretRef = *secret1;
    auto baseTx = new TransactionBase(tx, secretRef);
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

void aliceApprovesErc20(char* amount, char* secret, char* buffer, int nonce) {
    TransactionSkeleton tx;
    tx.from = jsToAddress("0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a");
    tx.to = jsToAddress("0xc0eb7AeD740E1796992A08962c15661bDEB58003");
    tx.value = 0; // exp10<18>();
    tx.gas = 300000;
    tx.gasPrice = 100 * exp10<9>();
    tx.nonce = nonce;
    std::stringstream ss;
    ss << "0x095ea7b3"
       << "000000000000000000000000"
       << toHex(jsToAddress("0x9387Fd3a016bB0205e4e131Dde886B9d2BC000A2"))
       << toHex(toBigEndian(jsToU256(amount)));
    tx.data = jsToBytes(ss.str());
    signTx(tx, secret, buffer);
    std::cout << tx.data << std::endl;
}

void aliceInitsEthDeal(char* dealId, char* amount, char* secret, char* buffer, char* bob, char* aliceHash, char* bobHash, int nonce) {
    TransactionSkeleton tx;
    tx.from = jsToAddress("0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a");
    tx.to = jsToAddress("0x2d379DCCDabBAB4BE6B8bEfA7352CBb88af705C1");
    tx.value = jsToU256(amount); // exp10<18>();
    tx.gas = 300000;
    tx.gasPrice = 100 * exp10<9>();
    tx.nonce = nonce;
    std::stringstream ss;
    ss << "0x47c7b6e2"
       << toHex(jsToBytes(dealId))
       << "000000000000000000000000"
       << toHex(jsToAddress(bob))
       << toHex(jsToBytes(aliceHash))
       << "000000000000000000000000"
       << toHex(jsToBytes(bobHash))
       << "000000000000000000000000";
    tx.data = jsToBytes(ss.str());
    signTx(tx, secret, buffer);
    std::cout << tx.data << std::endl;
}

void aliceClaimsPayment(char* dealId, char* amount, char* tokenAddress, char* secret, char* buffer, char* bob, char* aliceHash, char* bobSecret, int nonce) {
    TransactionSkeleton tx;
    tx.from = jsToAddress("0x485d2cc2d13a9e12E4b53D606DB1c8adc884fB8a");
    tx.to = jsToAddress("0x2d379DCCDabBAB4BE6B8bEfA7352CBb88af705C1");
    tx.value = 0; // exp10<18>();
    tx.gas = 300000;
    tx.gasPrice = 100 * exp10<9>();
    tx.nonce = nonce;
    std::stringstream ss;
    ss << "0x8b9a167a"
       << toHex(jsToBytes(dealId))
       << toHex(toBigEndian(jsToU256(amount)))
       << "000000000000000000000000"
       << toHex(jsToAddress(tokenAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(bob))
       << toHex(jsToBytes(aliceHash))
       << "000000000000000000000000"
       << "00000000000000000000000000000000000000000000000000000000000000c0"
       << "0000000000000000000000000000000000000000000000000000000000000020"
       << toHex(jsToBytes(bobSecret));
    tx.data = jsToBytes(ss.str());
    signTx(tx, secret, buffer);
    std::cout << tx.data << std::endl;
}
