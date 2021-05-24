#include "typeimage.h"

TypeImage::TypeImage(QString id, enumDifficulte difficulte, qint8 reponse) : Question(id, difficulte, reponse)
{
}

TypeImage::TypeImage()
{
}

Question::enumType TypeImage::getType() const
{
    return enumType::image;
}
