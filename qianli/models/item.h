#ifndef ITEM_H
#define ITEM_H

#include <QString>

enum class ItemType {
    None,
    Bandage,        // 绷带：回血
    MedicinalWine,  // 药酒：回血+加血上限
    Food,           // 粮食：回体力
    Ginseng,        // 人参：加体力上限
    Wine,           // 酒：下关临时加攻击
    Forge           // 打磨锻造：升武器等级（立即生效，不入背包）
};

struct Item {
    ItemType type;
    QString name;
    int price;
    int buyLimit;       // 限购数量，-1为无限
    int remainingStock; // 剩余库存
    QString description;
    QString effectDesc;

    Item() = default;
    Item(ItemType t, QString n, int p, int limit, QString desc, QString eff)
        : type(t), name(n), price(p), buyLimit(limit), remainingStock(limit), 
          description(desc), effectDesc(eff) {}
};

class ItemFactory {
public:
    static Item createItem(ItemType type);
    static QList<Item> getShopItems();
};

#endif // ITEM_H
