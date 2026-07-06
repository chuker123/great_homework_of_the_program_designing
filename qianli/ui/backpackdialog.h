#ifndef BACKPACKDIALOG_H
#define BACKPACKDIALOG_H

#include <QDialog>
#include "systems/inventory.h"
#include "models/player.h"
#include <QLabel>

class BackpackDialog : public QDialog 
{
    Q_OBJECT
public:
    explicit BackpackDialog(Player *player, Inventory *inventory, QWidget *parent = nullptr);
    void refresh();

signals:
    void itemUsed();

private:
    Player *m_player;
    Inventory *m_inventory;
    QWidget *m_itemsContainer;
    QLabel *m_emptyLabel;
    void setupUI();
    void updateItems();
};

#endif 
