#include "keyboardutilities.h"

#include <tchar.h>
#include <string>
#include <windows.h>

#include <QDebug>

map<string, BYTE> KeyboardUtilities::byteCodes =
{
    pair<string, BYTE>("Backspace", 0x08),
    pair<string, BYTE>("Tab", 0x09),
    pair<string, BYTE>("Return", 0x0D),
    pair<string, BYTE>("Shift", 0x10),
    pair<string, BYTE>("Ctrl", 0x11),
    pair<string, BYTE>("Alt", 0x12),
    pair<string, BYTE>("Pause", 0x15),
    pair<string, BYTE>("CapsLock", 0x14),
    pair<string, BYTE>("Escape", 0x1B),
    pair<string, BYTE>("Space", 0x20),
    pair<string, BYTE>("PgUp", 0x21),
    pair<string, BYTE>("PgDown", 0x22),
    pair<string, BYTE>("End", 0x23),
    pair<string, BYTE>("Home", 0x24),
    pair<string, BYTE>("Up", 0x26),
    pair<string, BYTE>("Down", 0x28),
    pair<string, BYTE>("Left", 0x25),
    pair<string, BYTE>("Right", 0x27),
    pair<string, BYTE>("Del", 0x2E),
    pair<string, BYTE>("Ins", 0x2D),
    pair<string, BYTE>("NumLock", 0x90),
    pair<string, BYTE>("Scroll Lock", 0x91),
    pair<string, BYTE>("Window", 0x5B),
    pair<string, BYTE>("F1", 0x70),
    pair<string, BYTE>("F2", 0x71),
    pair<string, BYTE>("F3", 0x72),
    pair<string, BYTE>("F4", 0x73),
    pair<string, BYTE>("F5", 0x74),
    pair<string, BYTE>("F6", 0x75),
    pair<string, BYTE>("F7", 0x76),
    pair<string, BYTE>("F8", 0x77),
    pair<string, BYTE>("F9", 0x78),
    pair<string, BYTE>("F10", 0x79),
    pair<string, BYTE>("F11", 0x7A),
    pair<string, BYTE>("F12", 0x7B),
//    pair<string, BYTE>("NUMPAD0", 0x60),
//    pair<string, BYTE>("NUMPAD1", 0x61),
//    pair<string, BYTE>("NUMPAD2", 0x62),
//    pair<string, BYTE>("NUMPAD3", 0x63),
//    pair<string, BYTE>("NUMPAD4", 0x64),
//    pair<string, BYTE>("NUMPAD5", 0x65),
//    pair<string, BYTE>("NUMPAD6", 0x66),
//    pair<string, BYTE>("NUMPAD7", 0x67),
//    pair<string, BYTE>("NUMPAD8", 0x68),
//    pair<string, BYTE>("NUMPAD9", 0x69)
};

void KeyboardUtilities::writeText(string text)
{
    for(unsigned int i = 0; i < text.length(); ++i)
    {
        if(isupper(text[i]))
            keybd_event(VK_SHIFT, 0xAA, 0, 0);
        hitKey(text[i]);
        if(isupper(text[i]))
            keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
        Sleep(10);
    }
}

void KeyboardUtilities::hitKey(char key)
{
    BYTE kscan = BYTE(VkKeyScan(WCHAR(key)));
    keybd_event(kscan, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(kscan, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void KeyboardUtilities::hitSpecialKey(string vk)
{
    pressSpecialKey(vk);
    releaseSpecialKey(vk);
}

void KeyboardUtilities::pressSpecialKey(string vk)
{
    BYTE key = byteCodes.find(vk)->second;
    keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
}

void KeyboardUtilities::releaseSpecialKey(string vk)
{
    BYTE key = byteCodes.find(vk)->second;
    keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}
