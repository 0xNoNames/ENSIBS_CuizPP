#ifndef TYPEAUDIO_H
#define TYPEAUDIO_H

#include "question.h"

class TypeAudio : public virtual Question
{
public:
    explicit TypeAudio(QString id, QString nomFichier, enumDifficulte difficulte, QString sujet, std::vector<QString> choix);

    explicit TypeAudio();

    QString getNomFichier() const;

private:
    QString nomFichier;
};

#endif // TYPEIMAGE_H
