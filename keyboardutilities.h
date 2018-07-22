#ifndef KEYBOARDUTILITIES_H
#define KEYBOARDUTILITIES_H

#include <map>
#include <string>
#include <windows.h>

using namespace std;

class KeyboardUtilities
{
public:
    static void writeText(string text);

    static void hitKey(char key);
    static void hitSpecialKey(string vk);
    static void pressSpecialKey(string vk);
    static void releaseSpecialKey(string vk);

private:
    static map<string, BYTE> byteCodes;
};

#endif // KEYBOARDUTILITIES_H
