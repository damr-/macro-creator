#ifndef KEYBOARDUTILITIES_H
#define KEYBOARDUTILITIES_H

#include <map>
#include <string>
#include <windows.h>

using namespace std;

class KeyboardUtilities
{
    public:
        static void WriteText(string text);

        //PAR ... Press And Release
        static void PARKey(char key);
        static void PARSpecialKey(string vk);

        static void PressSpecialKey(string vk);
        static void ReleaseSpecialKey(string vk);

    private:
        static map<string, BYTE> byteCodes;
};

#endif // KEYBOARDUTILITIES_H
