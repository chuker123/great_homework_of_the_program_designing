#include "enemy.h"
#include <QStringList>

Enemy EnemyFactory::createEnemy(const QString &levelName) 
{
    if (levelName == "东岭关") 
        return Enemy("孔秀", 100, 25, 0.8, 200, "东岭关守将，曹操部下");
    else if (levelName == "洛阳") 
        return Enemy("韩福", 150, 40, 0.75, 300, "洛阳太守，设下埋伏");
    else if (levelName == "汜水关") 
        return Enemy("卞喜", 200, 55, 0.7, 400, "汜水关守将，假意设宴");
    else if (levelName == "荥阳") 
        return Enemy("王植", 300, 70, 0.65, 500, "荥阳太守，暗藏杀机");
    else if (levelName == "滑州") 
        return Enemy("秦琪", 400, 85, 0.6, 0, "滑州守将，最后一关");
    return Enemy("刀斧手", 60, 10, 0.9, 100, "普通士兵");
}

QStringList EnemyFactory::getLevelNames() 
{
    return QStringList() << "东岭关" << "洛阳" << "汜水关" << "荥阳" << "滑州";
}
