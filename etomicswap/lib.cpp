//
// Created by artem on 24.01.18.
//
#include "lib.h"
#include <iostream>
#include <libethcore/Common.h>
#include <libethcore/CommonJS.h>
#include <libethcore/TransactionBase.h>
using namespace dev;
using namespace dev::eth;

TransactionSkeleton txDataToSkeleton(BasicTxData txData)
{
    TransactionSkeleton tx;
    tx.from = jsToAddress(txData.from);
    tx.to = jsToAddress(txData.to);
    tx.value = jsToU256(txData.amount);
    tx.gas = 300000;
    tx.gasPrice = 100 * exp10<9>();
    tx.nonce = txData.nonce;
    return tx;
}

void signTx(TransactionSkeleton& tx, char* secret, char* buffer)
{
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

void approveErc20(char* amount, char* from, char* secret, char* buffer, int nonce)
{
    TransactionSkeleton tx;
    tx.from = jsToAddress(from);
    tx.to = jsToAddress("0xc0eb7AeD740E1796992A08962c15661bDEB58003");
    tx.value = 0; // exp10<18>();
    tx.gas = 300000;
    tx.gasPrice = 100 * exp10<9>();
    tx.nonce = nonce;
    std::stringstream ss;
    ss << "0x095ea7b3"
       << "000000000000000000000000"
       << toHex(jsToAddress("0xe1D4236C5774D35Dc47dcc2E5E0CcFc463A3289c"))
       << toHex(toBigEndian(jsToU256(amount)));
    tx.data = jsToBytes(ss.str());
    signTx(tx, secret, buffer);
    std::cout << tx.data << std::endl;
}

void aliceInitsEthDeal(AliceInitEthInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x47c7b6e2"
       << toHex(jsToBytes(input.dealId))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.bobAddress))
       << toHex(jsToBytes(input.aliceHash))
       << "000000000000000000000000"
       << toHex(jsToBytes(input.bobHash))
       << "000000000000000000000000";
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void aliceInitsErc20Deal(AliceInitErc20Input input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x184db3bf"
       << toHex(jsToBytes(input.dealId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.bobAddress))
       << toHex(jsToBytes(input.aliceHash))
       << "000000000000000000000000"
       << toHex(jsToBytes(input.bobHash))
       << "000000000000000000000000"
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void aliceClaimsAlicePayment(AliceClaimsAlicePaymentInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x8b9a167a"
       << toHex(jsToBytes(input.dealId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.bobAddress))
       << toHex(jsToBytes(input.aliceHash))
       << "000000000000000000000000"
       << "00000000000000000000000000000000000000000000000000000000000000c0"
       << "0000000000000000000000000000000000000000000000000000000000000020"
       << toHex(jsToBytes(input.bobSecret));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobClaimsAlicePayment(BobClaimsAlicePaymentInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x392ec66b"
       << toHex(jsToBytes(input.dealId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << toHex(jsToBytes(input.bobHash))
       << "000000000000000000000000"
       << "00000000000000000000000000000000000000000000000000000000000000c0"
       << "0000000000000000000000000000000000000000000000000000000000000020"
       << toHex(jsToBytes(input.aliceSecret));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobMakesEthDeposit(BobMakesEthDepositInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0xc2c5143f"
       << toHex(jsToBytes(input.depositId))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << toHex(jsToBytes(input.bobHash))
       << "000000000000000000000000";
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobMakesErc20Deposit(BobMakesErc20DepositInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0xce8bbe4b"
       << toHex(jsToBytes(input.depositId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << toHex(jsToBytes(input.bobHash))
       << "000000000000000000000000"
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobClaimsDeposit(BobClaimsDepositInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x1dbe6508"
       << toHex(jsToBytes(input.depositId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << toHex(toBigEndian(jsToU256(input.aliceCanClaimAfter)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress))
       << "00000000000000000000000000000000000000000000000000000000000000c0"
       << "0000000000000000000000000000000000000000000000000000000000000020"
       << toHex(jsToBytes(input.bobSecret));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void aliceClaimsBobDeposit(AliceClaimsBobDepositInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x960173b5"
       << toHex(jsToBytes(input.depositId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << toHex(toBigEndian(jsToU256(input.aliceCanClaimAfter)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.bobAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress))
       << toHex(jsToBytes(input.bobHash))
       << "000000000000000000000000";
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobMakesEthPayment(BobMakesEthPaymentInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0xcf36fe8e"
       << toHex(jsToBytes(input.paymentId))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << toHex(jsToBytes(input.aliceHash))
       << "000000000000000000000000";
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobMakesErc20Payment(BobMakesErc20PaymentInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x34f64dfd"
       << toHex(jsToBytes(input.paymentId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << toHex(jsToBytes(input.aliceHash))
       << "000000000000000000000000"
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void bobClaimsBobPayment(BobClaimsBobPaymentInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0xb7cc2312"
       << toHex(jsToBytes(input.paymentId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << toHex(toBigEndian(jsToU256(input.bobCanClaimAfter)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.aliceAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress))
       << toHex(jsToBytes(input.aliceHash))
       << "000000000000000000000000";
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}

void aliceClaimsBobPayment(AliceClaimsBobPaymentInput input, BasicTxData txData, char* result)
{
    TransactionSkeleton tx = txDataToSkeleton(txData);
    std::stringstream ss;
    ss << "0x97004255"
       << toHex(jsToBytes(input.paymentId))
       << toHex(toBigEndian(jsToU256(input.amount)))
       << toHex(toBigEndian(jsToU256(input.bobCanClaimAfter)))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.bobAddress))
       << "000000000000000000000000"
       << toHex(jsToAddress(input.tokenAddress))
       << "00000000000000000000000000000000000000000000000000000000000000c0"
       << "0000000000000000000000000000000000000000000000000000000000000020"
       << toHex(jsToBytes(input.aliceSecret));
    tx.data = jsToBytes(ss.str());
    signTx(tx, txData.secretKey, result);
    std::cout << tx.data << std::endl;
}
