/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/serverbrowsing/sb_crypt.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011,
   adapted to the SDK revision used in this game with the GameSpy OpenSDK (github.com/nitrocaster/GameSpy,
   commit d1deb2d1a951cf77933dda040b8d311cc09815a7).
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/serverbrowsing/LICENSE.txt. */

#ifndef _SB_CRYPT_H_
#define _SB_CRYPT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GOACryptState {
  unsigned char cards[256];  // A permutation of 0-255.
  unsigned char rotor;       // Index that rotates smoothly
  unsigned char ratchet;     // Index that moves erratically
  unsigned char avalanche;   // Index heavily data dependent
  unsigned char last_plain;  // Last plain text byte
  unsigned char last_cipher; // Last cipher text byte
} GOACryptState;

void GOACryptInit(GOACryptState* state, unsigned char* key,
                  unsigned char keysize);
void GOAHashInit(GOACryptState* state);
unsigned char GOAEncryptByte(GOACryptState* state,
                             unsigned char b); // Encrypt byte
void GOAEncrypt(GOACryptState* state, unsigned char* bp,
                int len); // Encrypt byte array
unsigned char GOADecryptByte(GOACryptState* state,
                             unsigned char b); // Decrypt byte.
void GOADecrypt(GOACryptState* state, unsigned char* bp,
                int len); // decrypt byte array
void GOAHashFinal(GOACryptState* state, unsigned char* hash,
                  unsigned char hashlength); // Hash length (16-32)

#ifdef __cplusplus
}
#endif

#endif
