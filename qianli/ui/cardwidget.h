#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include "models/card.h"

class CardWidget : public QWidget {
    Q_OBJECT
public:
    explicit CardWidget(const Card &card, int weaponLevel, QWidget *parent = nullptr);

signals:
    void cardClicked(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    Card m_card;
    int m_weaponLevel;
    int m_actualCost;
    bool m_hovered = false;
    void setupUI();
};

#endif // CARDWIDGET_H
