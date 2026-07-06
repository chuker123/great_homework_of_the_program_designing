#ifndef CHOICESYSTEM_H
#define CHOICESYSTEM_H

#include <QString>
#include <functional>
#include "models/player.h"

struct ChoiceEvent {
    QString title;
    QString description;
    QString optionA;
    QString optionB;
    QString resultA;
    QString resultB;
    std::function<void(Player &)> effectA;
    std::function<void(Player &)> effectB;
};

class ChoiceSystem {
public:
    static ChoiceEvent getEventForLevel(const QString &levelName);
    static bool hasChoiceEvent(const QString &levelName);
};

#endif // CHOICESYSTEM_H
