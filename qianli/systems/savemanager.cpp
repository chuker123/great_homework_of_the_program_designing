#include "savemanager.h"
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>

SaveManager &SaveManager::instance() {
    static SaveManager mgr;
    return mgr;
}

QString SaveManager::getSaveDir() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return path;
}

QString SaveManager::getSaveFilePath(int slot) {
    return getSaveDir() + QString("/save_%1.dat").arg(slot);
}

void SaveManager::autoSave(const LevelSave &save, int slot) {
    QString path = getSaveFilePath(slot);
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << save.levelName;
        out << save.stats;
        out << save.inventory;
        file.close();
    }
}

SaveManager::LevelSave SaveManager::loadSave(int slot) {
    LevelSave save;
    QString path = getSaveFilePath(slot);
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> save.levelName;
        in >> save.stats;
        in >> save.inventory;
        save.isValid = true;
        file.close();
    }
    return save;
}

QList<SaveManager::LevelSave> SaveManager::listSaves() {
    QList<LevelSave> saves;
    for (int i = 0; i < 5; ++i) {
        auto save = loadSave(i);
        if (save.isValid) {
            saves.append(save);
        }
    }
    return saves;
}

void SaveManager::clearAllSaves() {
    for (int i = 0; i < 5; ++i) {
        QFile::remove(getSaveFilePath(i));
    }
}

// 新增：清除从 startSlot 开始的所有存档
void SaveManager::clearSavesFrom(int startSlot) {
    for (int i = startSlot; i < 5; ++i) {
        QFile::remove(getSaveFilePath(i));
    }
}