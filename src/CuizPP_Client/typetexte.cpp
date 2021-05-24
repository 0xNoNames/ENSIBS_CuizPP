#include "typetexte.h"

TypeTexte::TypeTexte(QString id, enumDifficulte difficulte, QString sujet, std::vector<QString> choix) : Question(id, difficulte, sujet, choix)
{
}

QString TypeTexte::getNomFichier() const
{
    return QString();
}
