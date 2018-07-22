#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>

#include <QString>
#include <QStringList>

enum CmdType
{
    DELAY = 0,
    CLICK = 1,
    CURPOS = 2,
    DRAG = 3,
    SCROLL = 4,
    HITKEY = 5,
    WRITETEXT = 6,
    RUNEXE = 7
};

static std::map<CmdType, int> CmdStrLen
{
    std::pair<CmdType, int>(CmdType::DELAY, 3),
    std::pair<CmdType, int>(CmdType::CLICK, 3),
    std::pair<CmdType, int>(CmdType::CURPOS, 3),
    std::pair<CmdType, int>(CmdType::DRAG, 3),
    std::pair<CmdType, int>(CmdType::SCROLL, 3),
    std::pair<CmdType, int>(CmdType::HITKEY, 4),
    std::pair<CmdType, int>(CmdType::WRITETEXT, 2),
    std::pair<CmdType, int>(CmdType::RUNEXE, 2)
};

class Commands
{
public:
    static void ExecuteCommand(QString command);
    static int GetCmdStrLen(CmdType cmdType) { return CmdStrLen.find(cmdType)->second; }

private:
    static void Click(QStringList cmd);
    static void Delay(QStringList cmd);
    static void CursorPos(QStringList cmd);
    static void Drag(QStringList cmd);
    static void Scroll(QStringList cmd);
    static void HitKey(QStringList cmd);
    static void WriteText(QStringList cmd);
    static void RunExe(QStringList cmd);
};

#endif // COMMANDS_H
