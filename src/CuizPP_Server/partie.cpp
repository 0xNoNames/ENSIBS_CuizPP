#include "partie.h"

/**
 * @brief Partie::Partie
 * @param id
 * @param titre
 * @param places
 * @param maitreDePartie
 * @param listeJoueursPartie
 * @param parent
 */
Partie::Partie(QString id, QString titre, qint8 places, QTcpSocket *hote, QHash<QTcpSocket *, Joueur *> listeJoueurs, qint8 nombreQuestions, QObject *parent)
    : QObject(parent), id(id), titre(titre), places(places), hote(hote), listeJoueurs(listeJoueurs), nombreQuestions(nombreQuestions)
{
    this->statut = enAttente;
}

Partie::~Partie()
{
    foreach (Joueur *joueur, this->listeJoueurs)
        joueur->setScore(0);
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
QHash<QTcpSocket *, Joueur *> Partie::getListeJoueurs() const
{
    return this->listeJoueurs;
}

/**
 * @brief addListeJoueursPartie
 * @param socket
 * @param nomUtilisateur
 */
void Partie::addListeJoueurs(QTcpSocket *socket, Joueur *joueur)
{
    this->listeJoueurs.insert(socket, joueur);
}

/**
 * @brief Partie::removeListeJoueursPartie
 * @param socket
 */
void Partie::removeListeJoueurs(QTcpSocket *socket)
{
    this->listeJoueurs.remove(socket);
}

/**
 * @brief Partie::getMaitreDePartie
 * @return
 */
QTcpSocket *Partie::getHote() const
{
    return this->hote;
}

qint8 Partie::getNombreQuestions() const
{
    return this->nombreQuestions;
}
/**
 * @brief Partie::setEnCours
 * @param statut
 */
void Partie::setStatut(statutPartie statut)
{
    this->statut = statut;
}

void Partie::setListeQuestions(std::vector<Question *> listeQuestions)
{
    this->listeQuestions = listeQuestions;
}

void Partie::demarrer()
{

    this->statut = statutPartie::enCours;
    this->questionEnCours = this->listeQuestions.back();
    this->listeQuestions.pop_back();

    envoyerQuestion("4", this->questionEnCours->getID());

    this->timerQuestion = new QTimer(this);
    this->timerQuestion->setSingleShot(true);
    connect(this->timerQuestion, SIGNAL(timeout()), this, SLOT(nextQuestion()));

    this->timerQuestion->start(this->questionEnCours->getTimer());
}

void Partie::nextQuestion()
{
    // Vérifie les réponses des utilisateurs et incrément ou non leurs scores.
    verifierQuestion();

    // S'il reste des questions, on continue.
    if (this->listeQuestions.size() > 0)
    {
        this->questionEnCours = this->listeQuestions.back();
        listeQuestions.pop_back();

        envoyerQuestion("4", this->questionEnCours->getID());

        this->timerQuestion->start(this->questionEnCours->getTimer());
    }
    else
    {
        this->statut = statutPartie::enAttente;

        // Signale au serveur que la partie est finie.
        emit partieFinie();
    }
}

void Partie::verifierQuestion()
{
    foreach (Joueur *joueur, this->listeJoueurs.values())
    {
        if (joueur->getReponse() == this->questionEnCours->getReponse())
        {
            qint8 points = joueur->getScore() + this->questionEnCours->getPoints();
            joueur->setScore(points);
        }
    }
}

void Partie::envoyerQuestion(QString type, QString arg)
{
    foreach (QTcpSocket *socket, this->listeJoueurs.keys())
    {
        if (socket && socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            QByteArray enTete = QString("messageType:%1,arg:%2;").arg(type, arg).toUtf8();
            enTete.resize(28);

            QByteArray data = QString::number(this->questionEnCours->getType()).toUtf8();
            data.prepend(enTete);

            socketStream << data;
        }
    }
}
