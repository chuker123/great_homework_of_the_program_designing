#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>

// 游戏状态管理器（简化版，主要逻辑在GameScene中）
class GameManager : public QObject {
    Q_OBJECT
public:
    static GameManager &instance();

private:
    GameManager() = default;
};

#endif // GAMEMANAGER_H
