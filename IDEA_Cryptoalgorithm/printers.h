#define HEX 16
#define DEC 10

#define WITH_SPACES  1
#define WITHOUT_SPACES  0

#define TO_FILE 1
#define TO_CONSOLE 0

#define O std::wcout<<
#define I std::wcin>>
#define E <<std::endl;

wchar_t** allocate2Darray(int8_t, int8_t);
void deallocate2Darray(wchar_t**, int8_t);
void printWstringAs_UINT16_T(std::wstring, int8_t, int8_t, int8_t);
void print2Darray(wchar_t**, int8_t, int8_t, int8_t);
std::wstring inHex(int);