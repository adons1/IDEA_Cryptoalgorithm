#include <iostream>
#include "printers.h"

#define ENCRYPT true
#define DECRYPT false


#define HEX 16
#define DEC 10

#define WITH_SPACES  1
#define WITHOUT_SPACES  0


#pragma region mathematical operations

uint64_t mod_val = 65537;
int _mul(int x, int y) {
    if (x == 0)
        x = mod_val - 1;
    if (y == 0)
        y = mod_val - 1;

    int xx = x;
    int yy = y;
    int64_t rt = (xx * yy) % mod_val;
    return rt;
}
int _sum(int x, int y) {
    int xx = x;
    int yy = y;
    int64_t rt = (xx + yy) % (mod_val - 1);
    return rt;
}
int _xor(int x, int y) {
    int rt = x ^ y;
    return rt % (mod_val);
}
wchar_t _mulInv(int b, int n = mod_val) {
    int r1 = n;
    int r2 = b;
    int r;

    int t1 = 0;
    int t2 = 1;
    int t;

    int q;

    while (r2 > 0) {
        q = r1 / r2;

        r = r1 - q * r2;
        r1 = r2; r2 = r;

        t = t1 - q * t2;
        t1 = t2; t2 = t;
    }
    if (r1 == 1) {

        if (t1 * b % n != 1) {
            return t1 + 1;
        }
        return t1;
    }
}
wchar_t  _addInv(int16_t b, int16_t n = mod_val) {
    return n - b;
}
#pragma endregion


int length(wchar_t* message) {
    int16_t i = 0;
    while (message[i]) {
        i++;
    }
    return i;
}

void keygen(wchar_t* key) {
    *(key + 0) = 1;
    *(key + 1) = 2;
    *(key + 2) = 3;
    *(key + 3) = 4;
    *(key + 4) = 5;
    *(key + 5) = 6;
    *(key + 6) = 7;
    *(key + 7) = 8;
}
void key_rounds_gen_enc(wchar_t* key, wchar_t** key_rounds) {
    
    int8_t counter = 0;
    int8_t key_ptr = 0;
    for (int8_t i = 0; i < 9; i++) {
        for (int8_t j = 0; j < 6; j++) {
            *(*(key_rounds + i) + j) = 0;
        }
    }
    for (int8_t i = 0; i < 9; i++) {
        for (int8_t j = 0; j < 6; j++) {
            counter++;
            *(*(key_rounds + i) + j) = *(key + key_ptr);
            key_ptr++;
            if (key_ptr == 8) {

#pragma region shift 16 positions
                wchar_t key0 = *(key);
                for (int8_t k = 0; k < 7; k++) {
                    *(key + k) = *(key + k + 1);
                }
                *(key + 7) = key0;
#pragma endregion

#pragma region shift 9 positions
                key0 = *(key + 0);
                for (int8_t k = 0; k < 7; k++) {
                    *(key + k) = *(key + k) << 9 xor *(key + k + 1) >> 7;
                }
                *(key + 7) = *(key + 7) << 9 xor key0 >> 7;
#pragma endregion

                key_ptr = 0;
            }
        }
    }
}
void key_rounds_gen_dec(wchar_t* key, wchar_t** key_rounds) {
    key_rounds_gen_enc(key, key_rounds);
    #define KEY_PARAM key_rounds, 9, 6
    wchar_t first;
    wchar_t last;

    for (int8_t i = 0; i < 5; i++) {
        for (int8_t j = 0; j < 6; j++) {
            if (j == 0 || j == 3) {
                if (i < 4) {
                    first = key_rounds[i][j];
                    last = key_rounds[8 - i][j];
                    key_rounds[i][j] = _mulInv(last);
                    key_rounds[8 - i][j] = _mulInv(first);
                }
                else {
                    key_rounds[i][j] = _mulInv(key_rounds[i][j]);
                }

            }
            if (j == 1 || j == 2) {
                first = key_rounds[i][j];
                last = key_rounds[8 - i][j];
                key_rounds[i][j] = _addInv(last) - 1;
                key_rounds[8 - i][j] = _addInv(first) - 1;
            }
            if ((j == 4 || j == 5) && (i < 4)) {
                first = key_rounds[i][j];
                last = key_rounds[7 - i][j];
                key_rounds[i][j] = last;
                key_rounds[7 - i][j] = first;
            }
        }
    }
    for (int8_t i = 1; i < 8; i++) {
        std::swap(key_rounds[i][1], key_rounds[i][2]);
    }
}

void round(wchar_t* buffer, wchar_t** key_rounds, int8_t iterationNmb) {
#define RNDS_TYPE uint16_t
#define K key_rounds[iterationNmb]
    RNDS_TYPE A = _mul(buffer[0], K[0]);
    RNDS_TYPE B = _sum(buffer[1], K[1]);
    RNDS_TYPE C = _sum(buffer[2], K[2]);
    RNDS_TYPE D = _mul(buffer[3], K[3]);
    RNDS_TYPE _E = _xor(A, C);
    RNDS_TYPE F = _xor(B, D);


    RNDS_TYPE q1 = _mul(_E, K[4]);
    RNDS_TYPE q2 = _sum(F, q1);
    RNDS_TYPE q3 = _mul(q2, K[5]);
    RNDS_TYPE q4 = _xor(A, q3);

    buffer[0] = _xor(A, _mul(_sum(F, _mul(_E, K[4])), K[5]));
    buffer[1] = _xor(C, _mul(_sum(F, _mul(_E, K[4])), K[5]));
    buffer[2] = _xor(B, _sum(_mul(_E, K[4]), _mul(_sum(F, _mul(_E, K[4])), K[5])));
    buffer[3] = _xor(D, _sum(_mul(_E, K[4]), _mul(_sum(F, _mul(_E, K[4])), K[5]))); 
}
std::wstring encrypt_decrypt(wchar_t* key, wchar_t* plain_text, bool enc_dec) {
#define KEY_PARAM key_rounds, 9, 6

    wchar_t** key_rounds = allocate2Darray(9, 6);

#pragma region keycopy
    wchar_t key_copy[9];
    for (int8_t i = 0; i < 8; i++) {
        key_copy[i] = key[i];
    }
#pragma endregion 

    if (enc_dec == ENCRYPT) {
        
        key_rounds_gen_enc(key_copy, key_rounds);
    }
    else {
        key_rounds_gen_dec(key_copy, key_rounds);
    }
    int _txtLen = length(plain_text);
    int requiredforPad = 4 - _txtLen % 4;


    wchar_t buffer[4];
    std::wstring cipher_text = L"";
    for (int cnt = 0; cnt < _txtLen + requiredforPad; cnt++) {
        if (cnt % 4 == 0) {
            buffer[0] = *(plain_text + cnt);
            buffer[1] = *(plain_text + cnt + 1);
            buffer[2] = *(plain_text + cnt + 2);
            buffer[3] = *(plain_text + cnt + 3);
            for (int8_t i = 0; i < 8; i++) {
                round(buffer, key_rounds, i);
            }

            wchar_t buffer1save = buffer[1];

            buffer[0] = _mul(buffer[0], key_rounds[8][0]);
            buffer[1] = _sum(buffer[2], key_rounds[8][1]);
            buffer[2] = _sum(buffer1save, key_rounds[8][2]);
            buffer[3] = _mul(buffer[3], key_rounds[8][3]);

            cipher_text = cipher_text + buffer[0] + buffer[1] + buffer[2] + buffer[3];
        }
    }
    return cipher_text;
    deallocate2Darray(key_rounds, 9);
}