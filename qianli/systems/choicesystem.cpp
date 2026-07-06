#include "choicesystem.h"

bool ChoiceSystem::hasChoiceEvent(const QString &levelName) 
{
    return (levelName == "汜水关" || levelName == "荥阳");
}

ChoiceEvent ChoiceSystem::getEventForLevel(const QString &levelName) 
{
    ChoiceEvent event;

    if (levelName == "汜水关") 
    {
        event.title = "故人重逢";
        event.description = "你来到汜水关前。守将卞喜久闻你的威名，在镇国寺设宴款待你。\n\n"
                           "你一眼便认出了寺内的普净和尚。他与你有旧怨，想不到在此处相遇。\n\n"
                           "而他则满脸不安，四处张望，鬼鬼祟祟，似有所图……";
        event.optionA = "呵斥驱赶（可能错过信息）";
        event.optionB = "走近攀谈（可能遇刺）";
        event.resultA = "你大喝一声，让普净立即离开。他眼神复杂，默默走开。\n\n酒过三巡，你已是醉眼朦胧。此时，卞喜突然大喝一声，埋伏的刀斧手尽出！\n\n猝不及防间，你身中数刀，待解决掉这些喽啰，你已是伤痕累累。\n\n获得状态：血量-50";
        event.resultB = "原来，普净早已听说你的义举，他不忍义士良将无端殒命于此，于是将门外埋伏着刀斧手的消息告知于你。\n\n你怒喝一声，挥刀直取卞喜!";
        event.effectA = [](Player &p) { p.takeDamage(50); };
        event.effectB = [](Player &p) { };
    } 
    else if (levelName == "荥阳") 
    {
        event.title = "暗藏杀机";
        event.description = "荥阳太守王植见你风尘仆仆，热情邀请你在驿馆歇息一夜。\n\n"
                           "胡班——王植部下从事——深夜来到你的窗边，探头探脑，\n\n"
                           "他神色紧张，似有隐情...";
        event.optionA = "呵斥驱赶（后果未知）";
        event.optionB = "上前询问（后果未知）";
        event.resultA = "你见胡班鬼鬼祟祟，厉声呵斥他离去，\n\n却不知王植已在馆外堆满柴火，只待夜半三更你熟睡之时，准备火攻！\n\n获得状态：进入战斗时血量-80";
        event.resultB = "胡班见你身形威武，一身布衣，手抚长髯，对青灯执古卷而读，俨然一副正气浩然的英雄形象。\n\n他不忍加害于你，于是吐露了王植纵火的阴谋，并暗中放你离去。\n\n孰料，王植一行人得到消息，竟快马加鞭追赶而来！";
        event.effectA = [](Player &p) {  p.takeDamage(80); };
        event.effectB = [](Player &p) { };
    }

    return event;
}
