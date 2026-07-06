#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "models/player.h"
#include "inventory.h"
#include <QString>
#include <QList>

class SaveManager {
public:
    struct LevelSave {
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
    // 新增：清除从 startSlot 开始（包括 startSlot）的所有存档
    void clearSavesFrom(int startSlot);
    QString getSaveFilePath(int slot);

private:
    SaveManager() = default;
    QString getSaveDir();
};

#endif // SAVEMANAGER_H