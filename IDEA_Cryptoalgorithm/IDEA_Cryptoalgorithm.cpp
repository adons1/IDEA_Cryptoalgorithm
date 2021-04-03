#include <iostream>
#include<fstream>
#include <io.h>
#include <fcntl.h>

#define O std::wcout<<
#define I std::wcin>>
#define E <<std::endl;

wchar_t** allocate2Darray(int8_t, int8_t);
void deallocate2Darray(wchar_t**, int8_t);
void randomlyfill2Darray(wchar_t**, int8_t, int8_t);
void print2Darray(wchar_t**, int8_t, int8_t);

int mod_val = pow(2, 16) + 1;

wchar_t _mul(wchar_t x, wchar_t y){
    wchar_t rt = (wchar_t)((int)x * (int)y) % mod_val;
    return rt;
}

wchar_t _sum(wchar_t x, wchar_t y) {
    return (wchar_t)((int)x + (int)y) % mod_val;
}

wchar_t _xor(wchar_t x, wchar_t y) {
    return (wchar_t)(x xor y);
}

int length(wchar_t* message) {
    int16_t i = 0;
    while (message[i]) {
        i++;
    }
    return i;
}
void padding(wchar_t* plain_text){
    int _txtLen = length(plain_text);
    int8_t requiredforPad = 4 - _txtLen % 4;

    int newsize = _txtLen + requiredforPad;
    if (requiredforPad > 0) {
        for (int8_t i = 0; i < requiredforPad; i++) {
            *(plain_text + _txtLen + 2*i) = L'0';
        }
    }
}


void keygen(wchar_t* key) {
    *(key + 0) = L'a';
    *(key + 1) = L'b';
    *(key + 2) = L'c';
    *(key + 3) = L'd';
    *(key + 4) = L'q';
    *(key + 5) = L'r';
    *(key + 6) = L'6';
    *(key + 7) = L'7';
    *(key + 8) = L'1';
}
void key_rounds_gen(wchar_t* key, wchar_t** key_rounds) {
    int8_t counter = 0;
    int8_t key_ptr = 0;
    for (int8_t i = 0; i < 9; i++) {
        for (int8_t j = 0; j < 6; j++) {
            counter++;
            *(*(key_rounds + i) + j) = *(key + key_ptr);
            key_ptr++;
            if (key_ptr == 8) {

                #pragma region shift 15 positions
                wchar_t key0 = *(key);
                for (int8_t i = 0; i < 7; i++) {
                    *(key + i) = *(key + i + 1);
                }
                *(key + 7) = key0;
                #pragma endregion

                #pragma region shift 10 positions
                key0 = *(key + 0);
                for (int8_t i = 0; i < 7; i++) {
                    *(key + i) = *(key + i) << 10 xor *(key + i +1) >> 6;
                }
                *(key + 7) = *(key + 7) << 10 xor key0 >>6;
                #pragma endregion

                key_ptr = 0;
            }
        }
    }
    //because of ending that  has only 4 blocks
    /**(*(key_rounds + 9) + 4) = 0;
    *(*(key_rounds + 9) + 5) = 0;*/
}

void round(wchar_t* buffer, wchar_t** key_rounds, int8_t iterationNmb) {
    wchar_t mulK1 = _mul(buffer[0], key_rounds[iterationNmb][0]);
    wchar_t sumK2 = _sum(buffer[1], key_rounds[iterationNmb][1]);
    wchar_t sumK3 = _sum(buffer[2], key_rounds[iterationNmb][2]);
    wchar_t mulK4 = _mul(buffer[0], key_rounds[iterationNmb][3]);

    wchar_t xor_mulK1_sumK3 = _xor(mulK1, sumK3);
    wchar_t xor_mulK2_sumK4 = _xor(sumK2, mulK4);

    wchar_t mulK5 = _mul(xor_mulK1_sumK3, key_rounds[iterationNmb][5]);
    wchar_t sum_mulK5__xor_mulK2_sumK4 = _sum(xor_mulK2_sumK4, mulK5);
    
    wchar_t mulK6 = _mul(sum_mulK5__xor_mulK2_sumK4, key_rounds[iterationNmb][6]);
    wchar_t sum_mulK5_mulK6 = _sum(mulK6, mulK5);

    
    buffer[0] = _xor(mulK6, buffer[0]);
    buffer[1] = _xor(mulK6, buffer[2]);
    buffer[2] = _xor(sum_mulK5_mulK6, buffer[1]);
    buffer[3] = _xor(sum_mulK5_mulK6, buffer[3]);
}
std::wstring encrypt(wchar_t* key, wchar_t* plain_text) {
    wchar_t** key_rounds = allocate2Darray(9, 6);
    #define KEY_PARAM key_rounds, 9, 6

    keygen(key);
    key_rounds_gen(key, key_rounds);
    //print2Darray(KEY_PARAM);

    int _txtLen = length(plain_text);
    int requiredforPad = 4 - _txtLen % 4;

    int inxbuffer = 0;
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

            buffer[0] = _mul(buffer[0], key_rounds[8][0]);
            buffer[1] = _sum(buffer[1], key_rounds[8][2]);
            buffer[2] = _sum(buffer[2], key_rounds[8][3]);
            buffer[3] = _mul(buffer[3], key_rounds[8][4]);
            
            cipher_text = cipher_text + buffer[0] + buffer[1] + buffer[2] + buffer[3];

        }
    }
    return cipher_text;
    deallocate2Darray(key_rounds, 9);
}
#pragma region array functions
wchar_t** allocate2Darray(int8_t n, int8_t m) {
    wchar_t** key = new wchar_t* [n];
    for (int8_t i = 0; i < n; i++) {
        *(key + i) = new wchar_t[m];
    }
    return key;
}
void deallocate2Darray(wchar_t** key, int8_t n) {
    for (int8_t i = 0; i < n; i++) {
        delete[] *(key + i);
    }
    delete[] key;
}
void randomlyfill2Darray(wchar_t** key, int8_t n, int8_t m) {
    for (int8_t i = 0; i < n; i++) {
        for (int8_t j = 0; j < m; j++) {
            *(*(key+i) +j) = (wchar_t)(rand() % mod_val);
        }
    }
}
void print2Darray(wchar_t** key, int8_t n, int8_t m) {
    for (int8_t i = 0; i < n; i++) {
        for (int8_t j = 0; j < m; j++) {
            std::cout << *(*(key + i) + j) << "\t";
        }
        std::cout << "\n";
    }
}
void print1Darray(wchar_t* plain_text) {
    std::wcout << plain_text << L"\n";
}
void printWstringAs_UINT16_T(std::wstring str) {
    O "\n";
    O "\n";
    for (int i = 0; i < str.length(); i++) {
        O(uint16_t)str[i] << "\t";
        if (i % 8 == 0 && i != 0) {
            O "\n";
        }
    }
    O "\n";
    O "\n";
}
#pragma endregion

int main()
{
    //_setmode(_fileno(stdout), _O_U16TEXT);

    wchar_t* key = new wchar_t[8];
    wchar_t* message = new wchar_t[1024];


    std::wcin.getline(message, 1024);
    
    O "Plain text\t:" E;
    printWstringAs_UINT16_T(std::wstring(message));
    
    std::wstring cipher_textttt = encrypt(key, message);

    O "Ciphered text\t:" E;
    printWstringAs_UINT16_T(cipher_textttt);

    

    /*std::wofstream fout("cipher.txt");
    fout << cipher_text;
    fout.close();*//*
    system("cipher.txt");*/
    system("pause");
}
