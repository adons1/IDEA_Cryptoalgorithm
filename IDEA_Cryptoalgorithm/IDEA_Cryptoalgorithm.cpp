#include <iostream>
#include <fstream>
#include "IdeaAlgorithm.h"
#include "printers.h"


#define MAX_TEXT_SIZE 4096



int main()
{
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
    if (length(message) >4096) {
        O L"Message should be length of "<<MAX_TEXT_SIZE<<" symbols!" E
            goto entermessage;
    }


    std::wstring cipher_text = encrypt_decrypt(key, message, ENCRYPT);
    std::wstring decrypted_text = encrypt_decrypt(key, &cipher_text[0], DECRYPT);

    int8_t toFile = TO_FILE;
    O "Ciphered text\t:" E;
    printWstringAs_UINT16_T(cipher_text, HEX, WITHOUT_SPACES, toFile);

    O "Decrypted text\t:" E;
    int c=0;
    while (decrypted_text[c]!='\0') {
        O decrypted_text[c];
        c++;
    }
    
    if (toFile == TO_FILE) {
        system("cipher_text.txt");
    }
    std::wcout <<L"\n\n"<< std::endl;
    system("pause");
}