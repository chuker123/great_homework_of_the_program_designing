#include "cardwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFont>
#include <QEvent>

CardWidget::CardWidget(const Card& card, int weaponLevel, QWidget* parent)
    : QWidget(parent), m_card(card), m_weaponLevel(weaponLevel) {
    m_actualCost = CardDeck::getPhyCostWithWeaponLevel(card.costPhy, weaponLevel);
    setFixedSize(140, 200);
    setCursor(Qt::PointingHandCursor);
}

void CardWidget::paintEvent(QPaintEvent* event) 
{
    Q_UNUSED(event)
        QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QString bgImage;
    if (m_card.isSpecial) 
        bgImage = ":/resources/cards/card_special.png";
    else if (m_card.type == CardType::Attack) 
        bgImage = ":/resources/cards/card_attack.png";
    else 
        bgImage = ":/resources/cards/card_defense.png";

    QPixmap bg(bgImage);
    qDebug() << "Card bg:" << bgImage << "null:" << bg.isNull() << "size:" << bg.size();

    if (!bg.isNull()) 
    {
        QPixmap scaled = bg.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int x = (scaled.width() - width()) / 2;
        int y = (scaled.height() - height()) / 2;
        painter.drawPixmap(0, 0, width(), height(), scaled, x, y, width(), height());
    }
    else 
    {
        QColor bgColor = (m_card.type == CardType::Attack)
            ? QColor(180, 60, 60) : QColor(60, 80, 150);
        if (m_hovered) bgColor = bgColor.lighter(120);
        painter.setPen(Qt::NoPen);
        painter.setBrush(bgColor);
        painter.drawRoundedRect(2, 2, width() - 4, height() - 4, 10, 10);
    }

    if (m_hovered) 
    {
        painter.setPen(QPen(QColor(255, 215, 0), 3));
        painter.drawRoundedRect(2, 2, width() - 4, height() - 4, 10, 10);
    }
    else 
    {
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRoundedRect(2, 2, width() - 4, height() - 4, 10, 10);
    }

    painter.setPen(QColor(0, 0, 0, 200));
    QFont nameFont("楷体", 20, QFont::Bold);
    painter.setFont(nameFont);
    painter.setPen(QColor(255, 255, 255, 200));
    painter.drawText(rect().adjusted(8, 17, -4, 0), Qt::AlignHCenter | Qt::AlignTop, m_card.name);

    painter.setPen(Qt::black);
    painter.drawText(rect().adjusted(5, 14, -5, 0), Qt::AlignHCenter | Qt::AlignTop, m_card.name);

    QFont infoFont("楷体", 14);
    painter.setFont(infoFont);

    QString info;
    if (m_card.type == CardType::Attack) 
        info = QString("倍率: %1x\n消耗体力: %2").arg(m_card.multiplier).arg(m_actualCost);
    else 
    {
        if (m_card.isSpecial) 
            info = QString("弹反: %1%\n消耗体力: %2\n[特殊]").arg(int(m_card.multiplier * 100)).arg(m_actualCost);
        else 
            info = QString("防御率: 70%\n消耗体力: %1").arg(m_actualCost);
    }

    painter.setPen(QColor(0, 0, 0, 180));
    painter.drawText(rect().adjusted(11, 60, -9, -9), Qt::AlignLeft | Qt::AlignTop, info);

    painter.setPen(Qt::white);
    painter.drawText(rect().adjusted(10, 59, -10, -10), Qt::AlignLeft | Qt::AlignTop, info);

    QString typeText = (m_card.type == CardType::Attack) ? "攻击" : "防御";
    QFont typeFont("楷体", 10);
    painter.setFont(typeFont);
    painter.setPen((m_card.type == CardType::Attack) ? QColor(255, 200, 200) : QColor(200, 200, 255));
    painter.drawText(rect().adjusted(5, -10, -5, -14), Qt::AlignRight | Qt::AlignBottom, typeText);
}

void CardWidget::mousePressEvent(QMouseEvent* event) 
{
    if (event->button() == Qt::LeftButton) 
        emit cardClicked(0);
}

void CardWidget::enterEvent(QEnterEvent* event) 
{
    m_hovered = true;
    update();
}

void CardWidget::leaveEvent(QEvent* event) 
{
    m_hovered = false;
    update();
}