#include "joueur.h"

Joueur::Joueur(QObject *parent) : QObject(parent)
{
    this->score = 0;
    this->reponse = NULL;
}

QString Joueur::getNomUtilisateur() const
{
    return this->nomUtilisateur;
}

qint8 Joueur::getScore() const
{
    return this->score;
}

qint8 Joueur::getReponse() const
{
    return this->reponse;
}

void Joueur::setNomUtilisateur(QString nom)
{
    this->nomUtilisateur = nom;
}

void Joueur::setScore(qint8 score)
{
    this->score = score;
}

void Joueur::setReponse(qint8 reponse)
{
    this->reponse = reponse;
}
