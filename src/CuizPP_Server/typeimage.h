#ifndef TYPEIMAGE_H
#define TYPEIMAGE_H

#include "question.h"

class TypeImage : public virtual Question
{
public:
    explicit TypeImage(QString id, enumDifficulte difficulte, qint8 reponse);

    explicit TypeImage();

    Question::enumType getType() const;
};

#endif // TYPEIMAGE_H
