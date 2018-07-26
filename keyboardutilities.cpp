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
    pair<string, BYTE>("Pause", 0x13),
    pair<string, BYTE>("CapsLock", 0x14),
    pair<string, BYTE>("Escape", 0x1B),
    pair<string, BYTE>("Space", 0x20),
    pair<string, BYTE>("PgUp", 0x21),
    pair<string, BYTE>("PgDown", 0x22),
    pair<string, BYTE>("End", 0x23),
    pair<string, BYTE>("Home", 0x24),
    pair<string, BYTE>("Left", 0x25),
    pair<string, BYTE>("Up", 0x26),
    pair<string, BYTE>("Right", 0x27),
    pair<string, BYTE>("Down", 0x28),
    pair<string, BYTE>("Print", 0x2C),
    pair<string, BYTE>("Ins", 0x2D),
    pair<string, BYTE>("Del", 0x2E),

    pair<string, BYTE>("0", 0x30),
    pair<string, BYTE>("1", 0x31),
    pair<string, BYTE>("2", 0x32),
    pair<string, BYTE>("3", 0x33),
    pair<string, BYTE>("4", 0x34),
    pair<string, BYTE>("5", 0x35),
    pair<string, BYTE>("6", 0x36),
    pair<string, BYTE>("7", 0x37),
    pair<string, BYTE>("8", 0x38),
    pair<string, BYTE>("9", 0x39),

    pair<string, BYTE>("A", 0x41),
    pair<string, BYTE>("B", 0x42),
    pair<string, BYTE>("C", 0x43),
    pair<string, BYTE>("D", 0x44),
    pair<string, BYTE>("E", 0x45),
    pair<string, BYTE>("F", 0x46),
    pair<string, BYTE>("G", 0x47),
    pair<string, BYTE>("H", 0x48),
    pair<string, BYTE>("I", 0x49),
    pair<string, BYTE>("J", 0x4A),
    pair<string, BYTE>("K", 0x4B),
    pair<string, BYTE>("L", 0x4C),
    pair<string, BYTE>("M", 0x4D),
    pair<string, BYTE>("N", 0x4E),
    pair<string, BYTE>("O", 0x4F),
    pair<string, BYTE>("P", 0x50),
    pair<string, BYTE>("Q", 0x51),
    pair<string, BYTE>("R", 0x52),
    pair<string, BYTE>("S", 0x53),
    pair<string, BYTE>("T", 0x54),
    pair<string, BYTE>("U", 0x55),
    pair<string, BYTE>("V", 0x56),
    pair<string, BYTE>("W", 0x57),
    pair<string, BYTE>("X", 0x58),
    pair<string, BYTE>("Y", 0x59),
    pair<string, BYTE>("Z", 0x5A),

    pair<string, BYTE>("Window", 0x5B),

    pair<string, BYTE>("*", 0x6A),
    pair<string, BYTE>("+", 0x6A),
    pair<string, BYTE>("-", 0x6D),
    pair<string, BYTE>(".", 0x6E),
    pair<string, BYTE>("/", 0x6F),

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

    pair<string, BYTE>("NumLock", 0x90),
    pair<string, BYTE>("Scroll Lock", 0x91),
};

void KeyboardUtilities::WriteText(string text)
{
    for(unsigned int i = 0; i < text.length(); ++i)
    {
        if(isupper(text[i]))
            keybd_event(VK_SHIFT, 0xAA, 0, 0);
        PARKey(text[i]);
        if(isupper(text[i]))
            keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
        Sleep(10);
    }
}

void KeyboardUtilities::PARKey(char key)
{
    BYTE kscan = BYTE(VkKeyScan(WCHAR(key)));
    keybd_event(kscan, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(kscan, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void KeyboardUtilities::PARSpecialKey(string vk)
{
    PressSpecialKey(vk);
    ReleaseSpecialKey(vk);
}

void KeyboardUtilities::PressSpecialKey(string vk)
{
    BYTE key = byteCodes.find(vk)->second;
    keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
}

void KeyboardUtilities::ReleaseSpecialKey(string vk)
{
    BYTE key = byteCodes.find(vk)->second;
    keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}
