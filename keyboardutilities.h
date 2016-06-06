#ifndef KEYBOARDUTILITIES_H
#define KEYBOARDUTILITIES_H

#include <map>
#include <string>
#include <Windows.h>

class KeyboardUtilities
{
public:
    static std::map<std::string, BYTE> GetByteCodes();
    static void writeText(std::string text);
    static void pressVK(std::string vk);

private:
    static std::map<std::string, BYTE> byteCodes;

};

#endif // KEYBOARDUTILITIES_H
