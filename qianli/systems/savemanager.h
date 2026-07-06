#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "models/player.h"
#include "inventory.h"
#include <QString>
#include <QList>

class SaveManager 
{
public:
    struct LevelSave 
    {
        QString levelName;
        PlayerStats stats;
        Inventory inventory;
        bool isValid = false;
    };

    static SaveManager &instance();

    void autoSave(const LevelSave &save, int slot);
    LevelSave loadSave(int slot);
    QList<LevelSave> listSaves();
    void clearAllSaves();
    void clearSavesFrom(int startSlot);
    QString getSaveFilePath(int slot);

private:
    SaveManager() = default;
    QString getSaveDir();
};

#endif 