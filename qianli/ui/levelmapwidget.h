#ifndef LEVELMAPWIDGET_H
#define LEVELMAPWIDGET_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QStringList>

class LevelMapWidget : public QWidget 
{
    Q_OBJECT
public:
    explicit LevelMapWidget(QWidget *parent = nullptr);
    void setCurrentLevel(int levelIndex);
    void setLevelsCleared(int count);

signals:
    void levelSelected(int levelIndex);

private:
    QStringList m_levelNames;
    int m_currentLevel = 0;
    int m_clearedLevels = 0;
    QList<QPushButton*> m_levelButtons;
    void setupUI();
    void updateButtonStates();
};

#endif 
