#include "joueur.h"

Joueur::Joueur(QString nomUtilisateur, qint8 score, QObject *parent) : QObject(parent), nomUtilisateur(nomUtilisateur), score(score)
{
}

Joueur::Joueur(const Joueur &joueur) : nomUtilisateur(joueur.nomUtilisateur), score(joueur.score)
{
}

QString Joueur::getNomUtilisateur() const
{
    return this->nomUtilisateur;
}

qint8 Joueur::getScore() const
{
    return this->score;
}

void Joueur::setNomUtilisateur(QString nom)
{
    this->nomUtilisateur = nom;
}

void Joueur::setScore(qint8 score)
{
    this->score = score;
}

bool Joueur::operator<(const Joueur &j1) const
{
    if (score == j1.score)
        return nomUtilisateur < j1.nomUtilisateur;
    else
        return score < j1.score;
}

bool Joueur::operator>(const Joueur &j1) const
{
    if (score == j1.score)
        return nomUtilisateur > j1.nomUtilisateur;
    else
        return score > j1.score;
}
