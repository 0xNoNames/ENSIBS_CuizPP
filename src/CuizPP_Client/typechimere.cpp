#include "typechimere.h"

TypeChimere::TypeChimere(QString id, QString nomFichierAudio, QString nomFichierImage, enumDifficulte difficulte, QString sujet, std::vector<QString> choix)
    : TypeAudio(), TypeImage(), nomFichiers({nomFichierAudio, nomFichierImage})
{
    this->id = id;
    this->difficulte = difficulte;
    this->sujet = sujet;
    this->choix = choix;

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

QString TypeChimere::getNomFichier() const
{
    return QString();
}

QPair<QString, QString> TypeChimere::getNomFichiers() const
{
    return this->nomFichiers;
}
