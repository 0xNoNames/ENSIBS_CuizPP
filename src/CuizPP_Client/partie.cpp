#include "partie.h"

/**
 * @brief Partie::Partie
 * @param id
 * @param titre
 * @param enCours
 * @param places
 * @param maitreDePartie
 * @param listeJoueursPartie
 * @param parent
 */
Partie::Partie(QString id, QString titre, statutPartie statut, qint8 places, qintptr hote, QMap<Joueur, qintptr> listeJoueurs, qint8 nombreQuestions, QObject *parent)
    : QObject(parent), id(id), titre(titre), statut(statut), places(places), hote(hote), listeJoueurs(listeJoueurs), nombreQuestions(nombreQuestions)
{
}

/**
 * @brief Partie::creerId
 * @return
 */
QString Partie::creerID()
{
    // Créée un nombre hexadécimal aléatoire à partir de QUuid.
    return QUuid::createUuid().toString().remove(QRegularExpression("{|}|-"));
}

/**
 * @brief Partie::getId
 * @return
 */
QString Partie::getID() const
{
    return this->id;
}

/**
 * @brief Partie::getTitre
 * @return
 */
QString Partie::getTitre() const
{
    return this->titre;
}

/**
 * @brief Partie::getEnCours
 * @return
 */
Partie::statutPartie Partie::getStatut() const
{
    return this->statut;
}

/**
 * @brief Partie::getPlaces
 * @return
 */
qint8 Partie::getPlaces() const
{
    return this->places;
}

/**
 * @brief Partie::getListeJoueursPartie
 * @return
 */
QMap<Joueur, qintptr> Partie::getListeJoueurs() const
{
    return this->listeJoueurs;
}

qint8 Partie::getNombreJoueurs() const
{
    return this->listeJoueurs.size();
}

/**
 * @brief Partie::getMaitreDePartie
 * @return
 */
qintptr Partie::getHote() const
{
    return this->hote;
}

qint8 Partie::getNombreQuestions() const
{
    return this->nombreQuestions;
}

/**
 * @brief PartieComparator
 * @param p1 Partie :
 * @param p2 Partie :
 * @return bool : Vrai si p1 et p2 sont triés par ordre alphabétique.
 */
bool Partie::PartieComparator(const Partie *p1, const Partie *p2)
{
    return p1->getTitre() < p2->getTitre();
}
