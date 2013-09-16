#define DLLIMPORT __declspec(dllimport)

//DLLIMPORT int Multiply(int ParOne,int ParTwo);
DLLIMPORT int MyEncryptFile(const char *src, const char* pwd);
DLLIMPORT int MyDecryptFile(const char *src, const char* pwd);
DLLIMPORT BOOL PingTest(LPCTSTR pszHostName);