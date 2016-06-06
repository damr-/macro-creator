#include "keyboardutilities.h"

#include <tchar.h>
#include <string>
#include <Windows.h>

std::map<std::string, BYTE> KeyboardUtilities::byteCodes =
{
    std::pair<std::string, BYTE>("Backspace", 0x08)
};

std::map<std::string, BYTE> KeyboardUtilities::GetByteCodes()
{
    if(byteCodes.size() == 0)
    {
        byteCodes.insert( std::pair<std::string, BYTE>("Backspace", 0x08) );
        byteCodes.insert( std::pair<std::string, BYTE>("Tab", 0x09) );
        byteCodes.insert( std::pair<std::string, BYTE>("Return", 0x0D) );
        byteCodes.insert( std::pair<std::string, BYTE>("Pause", 0x15) );
        byteCodes.insert( std::pair<std::string, BYTE>("Caps Lock", 0x14) );
        byteCodes.insert( std::pair<std::string, BYTE>("Escape", 0x1B) );
        byteCodes.insert( std::pair<std::string, BYTE>("Space", 0x20));
        byteCodes.insert( std::pair<std::string, BYTE>("Page Up", 0x21));
        byteCodes.insert( std::pair<std::string, BYTE>("Page Down", 0x22));
        byteCodes.insert( std::pair<std::string, BYTE>("End", 0x23));
        byteCodes.insert( std::pair<std::string, BYTE>("Home", 0x24));
        byteCodes.insert( std::pair<std::string, BYTE>("Arrow Up", 0x26));
        byteCodes.insert( std::pair<std::string, BYTE>("Arrow Down", 0x28));
        byteCodes.insert( std::pair<std::string, BYTE>("Arrow Left", 0x25));
        byteCodes.insert( std::pair<std::string, BYTE>("Arrow Right", 0x27));
        byteCodes.insert( std::pair<std::string, BYTE>("Delete", 0x2E));
        byteCodes.insert( std::pair<std::string, BYTE>("Insert", 0x2D));
        byteCodes.insert( std::pair<std::string, BYTE>("NUM Lock", 0x90));
        byteCodes.insert( std::pair<std::string, BYTE>("Scroll Lock", 0x91));
        byteCodes.insert( std::pair<std::string, BYTE>("Window", 0x5B));
        byteCodes.insert( std::pair<std::string, BYTE>("F1", 0x70));
        byteCodes.insert( std::pair<std::string, BYTE>("F2", 0x71));
        byteCodes.insert( std::pair<std::string, BYTE>("F3", 0x72));
        byteCodes.insert( std::pair<std::string, BYTE>("F4", 0x73));
        byteCodes.insert( std::pair<std::string, BYTE>("F5", 0x74));
        byteCodes.insert( std::pair<std::string, BYTE>("F6", 0x75));
        byteCodes.insert( std::pair<std::string, BYTE>("F7", 0x76));
        byteCodes.insert( std::pair<std::string, BYTE>("F8", 0x77));
        byteCodes.insert( std::pair<std::string, BYTE>("F9", 0x78));
        byteCodes.insert( std::pair<std::string, BYTE>("F10", 0x79));
        byteCodes.insert( std::pair<std::string, BYTE>("F11", 0x7A));
        byteCodes.insert( std::pair<std::string, BYTE>("F12", 0x7B));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD0", 0x60));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD1", 0x61));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD2", 0x62));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD3", 0x63));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD4", 0x64));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD5", 0x65));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD6", 0x66));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD7", 0x67));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD8", 0x68));
        byteCodes.insert( std::pair<std::string, BYTE>("NUMPAD9", 0x69));
    }
    return byteCodes;
}

void KeyboardUtilities::writeText(std::string text)
{
    for(unsigned int i = 0; i < text.length(); ++i)
    {
        keybd_event(VkKeyScan(text[i]), 0, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VkKeyScan(text[i]), 0, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void KeyboardUtilities::pressVK(std::string vk)
{
    BYTE key = byteCodes.find(vk)->second;
    keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
    Sleep(50);
}
