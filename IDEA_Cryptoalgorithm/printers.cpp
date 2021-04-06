#include <iostream>
#include <fstream>
#include "printers.h"

#define HEX 16
#define DEC 10

#define WITH_SPACES  1
#define WITHOUT_SPACES  0

#define TO_FILE 1
#define TO_CONSOLE 0
//
//#define O std::wcout<<
//#define I std::wcin>>
//#define E <<std::endl;



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
        delete[] * (key + i);
    }
    delete[] key;
}
void print2Darray(wchar_t** key, int8_t n, int8_t m, int8_t CS) {
    for (int8_t i = 0; i < n; i++) {
        for (int8_t j = 0; j < m; j++) {
            if (CS == HEX) {
                std::wcout << inHex(*(*(key + i) + j)) << "\t";
            }
            else {
                std::cout << *(*(key + i) + j) << "\t";
            }

        }
        std::cout << "\n";
    }
}
void printWstringAs_UINT16_T(std::wstring str, int8_t CS, int8_t spaces, int8_t toFile) {
    if (toFile == TO_FILE) {
        std::wofstream fout("cipher_text.txt");

        #define O fout<< 
        #define E fout<< 

        O L"\n";
        O L"\n";
        int devider = 15;
        if (spaces == WITHOUT_SPACES)
            O "0x";
        for (int i = 0; i < str.length(); i++) {
            if (CS == HEX) {
                if (spaces == WITHOUT_SPACES)
                    O inHex((uint16_t)str[i]);
                else
                    O inHex((uint16_t)str[i]) << "\t";
            }
            else {
                if (spaces == WITHOUT_SPACES)
                    O(uint16_t)str[i];
                else
                    O(uint16_t)str[i] << "\t";
            }
            if ((i % devider == 0) && (i != 0)) {
                O "\n";
            }
        }
        O "\n";
        O "\n";
    }
    else {
        #define O std::wcout<< 
        #define E <<std::endl 

        O L"\n";
        O L"\n";
        int devider = 15;
        if (spaces == WITHOUT_SPACES)
            O "0x";
        for (int i = 0; i < str.length(); i++) {
            if (CS == HEX) {
                if (spaces == WITHOUT_SPACES)
                    O inHex((uint16_t)str[i]);
                else
                    O inHex((uint16_t)str[i]) << "\t";
            }
            else {
                if (spaces == WITHOUT_SPACES)
                    O(uint16_t)str[i];
                else
                    O(uint16_t)str[i] << "\t";
            }
            if ((i % devider == 0) && (i != 0)) {
                O "\n";
            }
        }
        O "\n";
        O "\n";
    }
    
}
#pragma endregion


void reverseStr(std::wstring& str)
{
    int n = str.length();

    for (int i = 0; i < n / 2; i++)
        std::swap(str[i], str[n - i - 1]);
}
std::wstring inHex(int number) {
    int x = number;
    int y = 0;

    std::wstring result = L"";

    while (x > 0) {
        y = x % 16;
        switch (y) {
        case 0:
            result = result + L"0";
            break;
        case 1:
            result = result + L"1";
            break;
        case 2:
            result = result + L"2";
            break;
        case 3:
            result = result + L"3";
            break;
        case 4:
            result = result + L"4";
            break;
        case 5:
            result = result + L"5";
            break;
        case 6:
            result = result + L"6";
            break;
        case 7:
            result = result + L"7";
            break;
        case 8:
            result = result + L"8";
            break;
        case 9:
            result = result + L"9";
            break;
        case 10:
            result = result + L"a";
            break;
        case 11:
            result = result + L"b";
            break;
        case 12:
            result = result + L"c";
            break;
        case 13:
            result = result + L"d";
            break;
        case 14:
            result = result + L"e";
            break;
        case 15:
            result = result + L"f";
            break;
        }
        x = x / 16;
    }

    while (result.length() < 4){
        result = result + L"0";
    }

    reverseStr(result);
    return result;
}