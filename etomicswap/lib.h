//
// Created by artem on 24.01.18.
//
#ifdef __cplusplus
extern "C" {
#endif
void aliceApprovesErc20(char* amount, char* secret, char* buffer, int nonce);
void aliceInitsEthDeal(char* dealId, char* amount, char* secret, char* buffer, char* bob, char* aliceHash, char* bobHash, int nonce);
void aliceInitsErc20Deal(char* dealId, char* amount, char* tokenAddress, char* secret, char* buffer, char* bob, char* aliceHash, char* bobHash, int nonce);
void aliceClaimsPayment(char* dealId, char* amount, char* tokenAddress, char* secret, char* buffer, char* bob, char* aliceHash, char* bobSecret, int nonce);
void bobClaimsPayment(char* dealId, char* amount, char* tokenAddress, char* secret, char* buffer, char* alice, char* aliceSecret, char* bobHash, int nonce);
// Your prototype or Definition
#ifdef __cplusplus
}
#endif
