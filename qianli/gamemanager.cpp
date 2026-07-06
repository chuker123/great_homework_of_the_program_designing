#include "gamemanager.h"

GameManager &GameManager::instance() {
    static GameManager mgr;
    return mgr;
}
