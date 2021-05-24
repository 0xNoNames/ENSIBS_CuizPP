#include "typeaudio.h"

TypeAudio::TypeAudio(QString id, enumDifficulte difficulte, qint8 reponse) : Question(id, difficulte, reponse)
{
}

TypeAudio::TypeAudio()
{
}

Question::enumType TypeAudio::getType() const
{
    return enumType::audio;
}
