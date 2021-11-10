#include "mbedtls/aes.h"
int x = 0;
String getCipher(char* input) {
    mbedtls_aes_context aes;

    char* key = "abcdefghijklmnop";
    unsigned char output[16];
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, (const unsigned char*)key, strlen(key) * 8);
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)input, output);
    mbedtls_aes_free(&aes);

    String ret = "";
    for (int i = 0; i < 16; i++) {
        char str[3];

        sprintf(str, "%02x", (int)output[i]);
        ret += str;
    }
    return ret;
}
void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println(getCipher("ertgrtgbrtg"));
    Serial.println(getCipher("erttgerthetrhtrhthhrtwrtwrtg"));
}