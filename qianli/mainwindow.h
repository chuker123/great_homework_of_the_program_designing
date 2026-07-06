#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui/gamescene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGame();
    void onLoadGame();
    void onShowHelp();
    void onShowCredits();
    void onGameOver(bool victory);
    void onReturnToMenu();

private:
    void setupMenu();
    void showMenu();

    QWidget *m_menuWidget;
    GameScene *m_gameScene;
    QLabel *m_menuBgLabel;
};

#endif // MAINWINDOW_H
