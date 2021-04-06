#include <iostream>
#include <fstream>
#include "IdeaAlgorithm.h"
#include "printers.h"


#define MAX_TEXT_SIZE 4096


void standart_scenario(int8_t toFile) {
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
std::wstring decrypt_from_file() {
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
   
    int* begin = new int();
    int* end = new int();
    wchar_t* cipher_arr = readHexFromFile(path, begin, end);

    std::wstring decrypted_text = encrypt_decrypt(key, cipher_arr, DECRYPT);
    O std::endl;
    O "Decrypted text\t:";
    int c = 0;
    while ((decrypted_text[c] != '\0')&&((c<(*end-*begin)/4)+2)) {
        if((uint16_t)decrypted_text[c]>255)
            O inHex((uint16_t)decrypted_text[c], 4) <<" ";
        else    
            O decrypted_text[c];

        c++;
    }
    return decrypted_text;
} 

int main()
{
    while (true) {
        decrypt_from_file();
    }
    system("pause");
}