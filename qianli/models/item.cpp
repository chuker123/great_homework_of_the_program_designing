#include "item.h"
#include <QList>

Item ItemFactory::createItem(ItemType type) 
{
    switch (type) 
    {
    case ItemType::Bandage:
        return Item(type, "金疮散", 20, -1,
                    "军中常备疗伤药末", "止血愈创，回复25点气血");
    case ItemType::MedicinalWine:
        return Item(type, "虎骨药酒", 50, -1,
                    "古法浸酿的珍贵药酿", "养气活血，回复20点气血并提升15点气血上限");
    case ItemType::Food:
        return Item(type, "行军麦饼", 20, -1,
                    "千里跋涉的便携干粮", "饱腹充能，回复25点体力");
    case ItemType::Ginseng:
        return Item(type, "百年山参", 60, -1,
                    "深山采撷的老山参", "大补元气，永久提升20点体力上限");
    case ItemType::Wine:
        return Item(type, "壮行浊酒", 40, 2,
                    "一碗烈酒壮行色", "下一场战斗攻击力提升20点");
    case ItemType::Forge:
        return Item(type, "偃月淬锋", 50, 2,
                    "为青龙偃月刀精锻开锋", "锋芒再盛，武器等级提升1级");
    default:
        return Item();
    }
}

QList<Item> ItemFactory::getShopItems() 
{
    return {
        createItem(ItemType::Bandage),
        createItem(ItemType::MedicinalWine),
        createItem(ItemType::Food),
        createItem(ItemType::Ginseng),
        createItem(ItemType::Wine),
        createItem(ItemType::Forge)
    };
}
