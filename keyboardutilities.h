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

        static void PressAndReleaseScannedKey(char key);
        static void PressAndReleaseKey(string vk);

        static void PressKey(string vk);
        static void ReleaseKey(string vk);

    private:
        static map<string, BYTE> byteCodes;
};

#endif // KEYBOARDUTILITIES_H
