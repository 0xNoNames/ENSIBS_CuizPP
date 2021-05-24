#ifndef TYPETEXTE_H
#define TYPETEXTE_H

#include "question.h"

class TypeTexte : public Question
{
public:
    explicit TypeTexte(QString id, enumDifficulte difficulte, qint8 reponse);

    Question::enumType getType() const;
};

#endif // TYPETEXTE_H
