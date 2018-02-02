//
// Created by artem on 24.01.18.
//
#include "etomiclib.h"
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

char* signTx(TransactionSkeleton& tx, char* secret)
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
    char* result = (char*)malloc(strlen(tmp.c_str()) + 1);
    strncpy(result, tmp.c_str(), strlen(tmp.c_str()));
    result[strlen(tmp.c_str())] = '\0';
    return result;
}

char* approveErc20(char* amount, char* from, char* secret, int nonce)
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
    return signTx(tx, secret);
}

char* aliceSendsEthPayment(AliceSendsEthPaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* aliceSendsErc20Payment(AliceSendsErc20PaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* aliceReclaimsAlicePayment(AliceReclaimsAlicePaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobSpendsAlicePayment(BobSpendsAlicePaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobSendsEthDeposit(BobSendsEthDepositInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobSendsErc20Deposit(BobSendsErc20DepositInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobRefundsDeposit(BobRefundsDepositInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* aliceClaimsBobDeposit(AliceClaimsBobDepositInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobSendsEthPayment(BobSendsEthPaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobSendsErc20Payment(BobSendsErc20PaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* bobReclaimsBobPayment(BobReclaimsBobPaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}

char* aliceSpendsBobPayment(AliceSpendsBobPaymentInput input, BasicTxData txData)
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
    return signTx(tx, txData.secretKey);
}
