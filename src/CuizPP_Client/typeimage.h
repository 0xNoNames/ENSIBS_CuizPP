#ifndef TYPEIMAGE_H
#define TYPEIMAGE_H

#include "question.h"

class TypeImage : public virtual Question
{
public:
    explicit TypeImage(QString id, QString nomFichier, enumDifficulte difficulte, QString sujet, std::vector<QString> choix);

    explicit TypeImage();

    QString getNomFichier() const;

private:
    QString nomFichier;
};

#endif // TYPEIMAGE_H
