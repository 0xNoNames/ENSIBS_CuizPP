#include "question.h"

Question::Question(QString id, enumDifficulte difficulte, qint8 reponse, QObject *parent) : QObject(parent), id(id), difficulte(difficulte), reponse(reponse)
{
    switch (this->difficulte)
    {
    case enumDifficulte::facile:
        this->points = 5;
        this->timer = 11000;
        break;
    case enumDifficulte::moyenne:
        this->points = 10;
        this->timer = 16000;
        break;
    case enumDifficulte::difficile:
        this->points = 20;
        this->timer = 26000;
        break;
    }
}

Question::Question(const Question &q) : QObject(q.parent()), id(q.id), difficulte(q.difficulte), reponse(q.reponse)
{
}

Question::Question()
{
}

qint8 Question::getPoints() const
{
    return this->points;
}

qint16 Question::getTimer() const
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

qint8 Question::getReponse() const
{
    return this->reponse;
}

Question::enumDifficulte Question::stringToEnum(QString s)
{
    if (s == "facile")
        return enumDifficulte::facile;
    else if (s == "moyenne")
        return enumDifficulte::moyenne;
    else
        return enumDifficulte::difficile;
}
