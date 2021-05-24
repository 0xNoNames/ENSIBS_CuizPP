#include "typetexte.h"

TypeTexte::TypeTexte(QString id, enumDifficulte difficulte, qint8 reponse) : Question(id, difficulte, reponse)
{
}

Question::enumType TypeTexte::getType() const
{
    return enumType::texte;
}
