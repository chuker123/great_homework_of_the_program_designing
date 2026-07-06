#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QStackedWidget>
#include "models/player.h"
#include "models/enemy.h"
#include "systems/inventory.h"
#include "ui/battlescene.h"
#include "ui/choicewidget.h"
#include "ui/shopwidget.h"
#include "ui/levelmapwidget.h"

class GameScene : public QWidget 
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr);
    void startNewGame();
    void loadGame(int saveSlot);
    void forceVictory();

signals:
    void gameOver(bool victory);
    void returnToMenu();

private slots:
    void onLevelSelected(int levelIndex);
    void onChoiceMade(bool choseA);
    void onBattleFinished(bool won, int rewardMoney);
    void onShopClosed();
    void onOpenBackpack();
    void onBackpackItemUsed();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    enum GameState { Map, Choice, Battle, Shop, Victory, GameOver };

    QList<QLabel*> m_bgLabels; 

    QStackedWidget *m_stack;
    Player m_player;
    Inventory m_inventory;

    LevelMapWidget *m_mapWidget;
    ChoiceWidget *m_choiceWidget;
    BattleScene *m_battleScene;
    ShopWidget *m_shopWidget;

    QStringList m_levelNames;
    int m_currentLevelIndex = 0;
    int m_clearedLevels = 0;
    bool m_hasChoiceEvent = false;

    void setupUI();
    void goToMap();
    void goToChoice(const QString &levelName);
    void goToBattle(const QString &levelName);
    void goToShop();
    void showVictory();
    void showGameOver();
    void autoSave();
};

#endif 
