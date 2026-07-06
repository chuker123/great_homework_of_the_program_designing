#ifndef ITEM_H
#define ITEM_H

#include <QString>

enum class ItemType 
{
    None,
    Bandage,        
    MedicinalWine,  
    Food,           
    Ginseng,        
    Wine,          
    Forge        
};

struct Item 
{
    ItemType type;
    QString name;
    int price;
    int buyLimit;       
    int remainingStock; 
    QString description;
    QString effectDesc;

    Item() = default;
    Item(ItemType t, QString n, int p, int limit, QString desc, QString eff)
        : type(t), name(n), price(p), buyLimit(limit), remainingStock(limit), 
          description(desc), effectDesc(eff) {}
};

class ItemFactory 
{
public:
    static Item createItem(ItemType type);
    static QList<Item> getShopItems();
};

#endif 
