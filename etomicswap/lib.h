//
// Created by artem on 24.01.18.
//
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    char* from;
    char* to;
    char* amount;
    int nonce;
    char* secretKey;
} BasicTxData;

typedef struct {
    char* dealId;
    char* bobAddress;
    char* aliceHash;
    char* bobHash;
} AliceInitEthInput;

typedef struct {
    char* dealId;
    char* amount;
    char* tokenAddress;
    char* bobAddress;
    char* aliceHash;
    char* bobHash;
} AliceInitErc20Input;

typedef struct {
    char* dealId;
    char* amount;
    char* tokenAddress;
    char* bobAddress;
    char* aliceHash;
    char* bobSecret;
} AliceClaimsAlicePaymentInput;

typedef struct {
    char* dealId;
    char* amount;
    char* tokenAddress;
    char* aliceAddress;
    char* aliceSecret;
    char* bobHash;
} BobClaimsAlicePaymentInput;

typedef struct {
    char* depositId;
    char* aliceAddress;
    char* bobHash;
} BobMakesEthDepositInput;

typedef struct {
    char* depositId;
    char* amount;
    char* tokenAddress;
    char* aliceAddress;
    char* bobHash;
} BobMakesErc20DepositInput;

typedef struct {
    char* depositId;
    char* amount;
    char* tokenAddress;
    char* aliceAddress;
    char* bobSecret;
    char* aliceCanClaimAfter;
} BobClaimsDepositInput;

typedef struct {
    char* depositId;
    char* amount;
    char* tokenAddress;
    char* bobAddress;
    char* bobHash;
    char* aliceCanClaimAfter;
} AliceClaimsBobDepositInput;

typedef struct {
    char* paymentId;
    char* aliceAddress;
    char* aliceHash;
} BobMakesEthPaymentInput;

typedef struct {
    char* paymentId;
    char* amount;
    char* tokenAddress;
    char* aliceAddress;
    char* aliceHash;
} BobMakesErc20PaymentInput;

typedef struct {
    char* paymentId;
    char* amount;
    char* tokenAddress;
    char* aliceAddress;
    char* aliceHash;
    char* bobCanClaimAfter;
} BobClaimsBobPaymentInput;

typedef struct {
    char* paymentId;
    char* amount;
    char* tokenAddress;
    char* aliceSecret;
    char* bobAddress;
    char* bobCanClaimAfter;
} AliceClaimsBobPaymentInput;

void approveErc20(char* amount, char* from, char* secret, char* buffer, int nonce);
void aliceInitsEthDeal(AliceInitEthInput input, BasicTxData txData, char* result);
void aliceInitsErc20Deal(AliceInitErc20Input input, BasicTxData txData, char* result);
void aliceClaimsAlicePayment(AliceClaimsAlicePaymentInput input, BasicTxData txData, char* result);
void bobClaimsAlicePayment(BobClaimsAlicePaymentInput input, BasicTxData txData, char* result);
void bobMakesEthDeposit(BobMakesEthDepositInput input, BasicTxData txData, char* result);
void bobMakesErc20Deposit(BobMakesErc20DepositInput input, BasicTxData txData, char* result);
void bobClaimsDeposit(BobClaimsDepositInput input, BasicTxData txData, char* result);
void aliceClaimsBobDeposit(AliceClaimsBobDepositInput input, BasicTxData txData, char* result);
void bobMakesEthPayment(BobMakesEthPaymentInput input, BasicTxData txData, char* result);
void bobMakesErc20Payment(BobMakesErc20PaymentInput input, BasicTxData txData, char* result);
void bobClaimsBobPayment(BobClaimsBobPaymentInput input, BasicTxData txData, char* result);
void aliceClaimsBobPayment(AliceClaimsBobPaymentInput input, BasicTxData txData, char* result);
// Your prototype or Definition
#ifdef __cplusplus
}
#endif
