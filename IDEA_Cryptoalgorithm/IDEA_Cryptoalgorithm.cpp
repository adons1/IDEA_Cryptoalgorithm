#include <iostream>
#include <fstream>
#include "IdeaAlgorithm.h"
#include "printers.h"


#define MAX_TEXT_SIZE 4096


void standart_scenario() {
    wchar_t key[9];
    wchar_t message[MAX_TEXT_SIZE];

enterkey:
    std::cout << "secret key:\t";
    std::wcin.getline(key, 9);
    if (length(key) != 8) {
        O L"Wrong key length!" E
            goto enterkey;
    }

entermessage:
    std::cout << "message:\t";
    std::wcin.getline(message, MAX_TEXT_SIZE);
    if (length(message) > 4096) {
        O L"Message should be length of " << MAX_TEXT_SIZE << " symbols!" E
            goto entermessage;
    }


    std::wstring cipher_text = encrypt_decrypt(key, message, ENCRYPT);

    std::wstring decrypted_text = encrypt_decrypt(key, &cipher_text[0], DECRYPT);

    int8_t toFile = TO_CONSOLE;
    O "Ciphered text\t:" E;
    printWstringAs_UINT16_T(cipher_text, HEX, WITHOUT_SPACES, toFile);

    O "Decrypted text\t:" E;
    int c = 0;
    while (decrypted_text[c] != '\0') {
        O decrypted_text[c];
        c++;
    }

    if (toFile == TO_FILE) {
        system("cipher_text.txt");
    }
    std::wcout << L"\n\n" << std::endl;
}
void decrypt_from_file() {
    wchar_t key[9];
    char* path=new char[128];

enterpath:
    std::cout << "\nenter path:\t";
    std::cin.getline(path, 128);

enterkey:
    std::cout << "secret key:\t";
    std::wcin.getline(key, 9);
    if (length(key) != 8) {
        O L"Wrong key length!" E
            goto enterkey;
    }
    std::wifstream fin(path);
    std::wstring cipher_text = L"";
    std::wstring cipher_text_pure = L"";
    fin >> cipher_text;

    int i = 0;
    while (cipher_text[i] != L'0' && cipher_text[i] != L'x') {
        i++;
    }
    int begin = i;
    i = begin+2;
    while ((cipher_text[i] != L' ' || cipher_text[i] != L'\n' || cipher_text[i] != L'\t')&&i!=cipher_text.length()) {
        cipher_text_pure = cipher_text_pure + cipher_text[i];
        i++;
    }
    int end = i;
    O L"Cryptogramm:\t";
    O cipher_text_pure E;

    i = 0;
    int cipher_text_pure_len = cipher_text_pure.length();

#define ctp cipher_text_pure

    wchar_t* cipher_arr = new wchar_t[cipher_text_pure_len/4];
    while (i < cipher_text_pure_len) {
        if (i % 4 == 0) {
            std::wstring val = L"";
            val = val   + cipher_text_pure[i] 
                        + cipher_text_pure[i + 1] 
                        + cipher_text_pure[i + 2] 
                        + cipher_text_pure[i + 3];
            cipher_arr[i / 4] = inDec(val);
        }
        i++;
    }

    std::wstring decrypted_text = encrypt_decrypt(key, cipher_arr, DECRYPT);
    O std::endl;
    O "Decrypted text\t:";
    int c = 0;
    while (decrypted_text[c] != '\0') {
        O decrypted_text[c];
        c++;
    }
    
} 

int main()
{
    while (true) {
        decrypt_from_file();
    }
    system("pause");
}