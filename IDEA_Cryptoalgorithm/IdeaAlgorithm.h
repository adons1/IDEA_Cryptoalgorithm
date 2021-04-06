#define ENCRYPT true
#define DECRYPT false

int _mul(int x, int y);
int _sum(int x, int y);
int _xor(int x, int y);
wchar_t _mulInv(int b, int n = 65537);
wchar_t  _addInv(int16_t b, int16_t n = 65537);


int length(wchar_t* message);
int length(char* message);
void keygen(wchar_t* key);
void key_rounds_gen_enc(wchar_t* encryptionkey, wchar_t** key_rounds);
void key_rounds_gen_dec(wchar_t* key, wchar_t** key_rounds);
void round(wchar_t* buffer, wchar_t** key_rounds, int8_t iterationNmb);
std::wstring encrypt_decrypt(wchar_t* key, wchar_t* plain_text, bool enc_dec);