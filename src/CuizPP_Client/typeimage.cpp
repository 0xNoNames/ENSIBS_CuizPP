#include "typeimage.h"

TypeImage::TypeImage(QString id, QString nomFichier, enumDifficulte difficulte, QString sujet, std::vector<QString> choix)
    : Question(id, difficulte, sujet, choix), nomFichier(nomFichier)
{
}

TypeImage::TypeImage()
{
}

QString TypeImage::getNomFichier() const
{
    return this->nomFichier;
}
