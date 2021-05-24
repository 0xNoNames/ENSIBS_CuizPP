#ifndef TYPETEXTE_H
#define TYPETEXTE_H

#include "question.h"

class TypeTexte : public Question
{
public:
    explicit TypeTexte(QString id, enumDifficulte difficulte, QString sujet, std::vector<QString> choix);

    QString getNomFichier() const;
};

#endif // TYPETEXTE_H
