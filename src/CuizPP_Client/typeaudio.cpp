#include "typeaudio.h"

TypeAudio::TypeAudio(QString id, QString nomFichier, enumDifficulte difficulte, QString sujet, std::vector<QString> choix)
    : Question(id, difficulte, sujet, choix), nomFichier(nomFichier)
{
}

TypeAudio::TypeAudio()
{
}

QString TypeAudio::getNomFichier() const
{
    return this->nomFichier;
}
