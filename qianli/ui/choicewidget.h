#ifndef CHOICEWIDGET_H
#define CHOICEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "systems/choicesystem.h"

class ChoiceWidget : public QWidget 
{
    Q_OBJECT
public:
    explicit ChoiceWidget(QWidget *parent = nullptr);
    void setChoiceEvent(const ChoiceEvent &event);

signals:
    void choiceMade(bool choseA);

private slots:
    void onOptionA();
    void onOptionB();

private:
    ChoiceEvent m_currentEvent;
    QLabel *m_titleLabel;
    QLabel *m_descLabel;
    QPushButton *m_btnA;
    QPushButton *m_btnB;
    QLabel *m_resultLabel;
    QPushButton *m_continueBtn;
    void setupUI();
};

#endif 
