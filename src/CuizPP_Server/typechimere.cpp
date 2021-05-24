#include "typechimere.h"

TypeChimere::TypeChimere(QString id, enumDifficulte difficulte, qint8 reponse)
    : TypeAudio(), TypeImage()
{
    this->id = id;
    this->difficulte = difficulte;
    this->reponse = reponse;

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

Question::enumType TypeChimere::getType() const
{
    return enumType::chimere;
}
