#ifndef TYPECHIMERE_H
#define TYPECHIMERE_H

#include "typeaudio.h"
#include "typeimage.h"

class TypeChimere : public TypeAudio, public TypeImage
{
public:
    explicit TypeChimere(QString id, enumDifficulte difficulte, qint8 reponse);

    Question::enumType getType() const;
};

#endif // TYPECHIMERE_H
