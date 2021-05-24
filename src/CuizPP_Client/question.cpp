#include "question.h"

Question::Question(QString id, enumDifficulte difficulte, QString sujet, std::vector<QString> choix, QObject *parent) : QObject(parent), id(id), difficulte(difficulte), sujet(sujet),
                                                                                                                        choix(choix)
{
    switch (this->difficulte)
    {
    case enumDifficulte::facile:
        this->timer = 10;
        break;
    case enumDifficulte::moyenne:
        this->timer = 15;
        break;
    case enumDifficulte::difficile:
        this->timer = 25;
        break;
    }
}

Question::Question(const Question &q) : QObject(q.parent()), id(q.id), difficulte(q.difficulte), sujet(q.sujet), choix(q.choix)
{
    switch (this->difficulte)
    {
    case enumDifficulte::facile:
        this->timer = 10;
        break;
    case enumDifficulte::moyenne:
        this->timer = 15;
        break;
    case enumDifficulte::difficile:
        this->timer = 25;
        break;
    }
}

Question::Question()
{
}

qint8 Question::getTimer() const
{
    return this->timer;
}

QString Question::getID() const
{
    return this->id;
}

Question::enumDifficulte Question::getDifficulte() const
{
    return this->difficulte;
}

QString Question::getSujet() const
{
    return this->sujet;
}

std::vector<QString> Question::getChoix() const
{
    return this->choix;
}

Question::enumDifficulte Question::stringToEnum(QString s)
{
    if (s == "facile")
        return Question::enumDifficulte::facile;
    else if (s == "moyenne")
        return Question::enumDifficulte::moyenne;
    else
        return Question::enumDifficulte::difficile;
}
