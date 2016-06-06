#ifndef COMMANDLISTITEM_H
#define COMMANDLISTITEM_H

#include <QListWidgetItem>

class CommandListItem : public QListWidgetItem
{
public:
    int commandIndex = -1;
};

#endif // COMMANDLISTITEM_H
