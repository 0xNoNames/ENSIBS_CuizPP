#ifndef TYPECHIMERE_H
#define TYPECHIMERE_H

#include "typeaudio.h"
#include "typeimage.h"

class TypeChimere : public TypeAudio, public TypeImage
{
public:
    explicit TypeChimere(QString id, QString nomFichierAudio, QString nomFichierImage, enumDifficulte difficulte, QString sujet, std::vector<QString> choix);

    QString getNomFichier() const;

    QPair<QString, QString> getNomFichiers() const;

private:
    QPair<QString, QString> nomFichiers;
};

#endif // TYPECHIMERE_H
