#ifndef TYPEAUDIO_H
#define TYPEAUDIO_H

#include "question.h"

class TypeAudio : public virtual Question
{
public:
    explicit TypeAudio(QString id, enumDifficulte difficulte, qint8 reponse);

    explicit TypeAudio();

    Question::enumType getType() const;
};

#endif // TYPEIMAGE_H
