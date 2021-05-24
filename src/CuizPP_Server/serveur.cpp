#include "serveur.h"

using namespace std;
/**
 * @brief Serveur::Serveur
 * @param parent
 */
Serveur::Serveur(QObject *parent) : QObject(parent)
{
    this->banqueQuestions = new BanqueQuestions();
    this->serveurTCP = NULL;

    //-- -- -- -- -- -- -- -- -- -- -- -- PARTIES TEST -- -- -- -- -- -- -- -- -- -- -- --
    //    QHash<QTcpSocket *, Joueur *> su1;
    //    QHash<QTcpSocket *, Joueur *> su2;
    //    QHash<QTcpSocket *, Joueur *> su3;

    //    QTcpSocket *JeanST = new QTcpSocket();
    //    QTcpSocket *PierreST = new QTcpSocket();
    //    QTcpSocket *MarcST = new QTcpSocket();

    //    Joueur *Jean = new Joueur();
    //    Jean->setNomUtilisateur("Jean");
    //    Joueur *Pierre = new Joueur();
    //    Pierre->setNomUtilisateur("Pierre");
    //    Joueur *Marc = new Joueur();
    //    Marc->setNomUtilisateur("Marc");

    //    su1.insert(JeanST, Jean);
    //    su2.insert(PierreST, Pierre);
    //    su3.insert(MarcST, Marc);

    //    Partie *partie1 = new Partie("1", "Partie 1", 1, JeanST, su1, 5);
    //    Partie *partie2 = new Partie("2", "Partie 2", 3, PierreST, su2, 10);
    //    partie2->setStatut(Partie::enCours);
    //    Partie *partie3 = new Partie("3", "Partie 3", 0, MarcST, su3, 15);

    //    this->listeParties.insert(partie1);
    //    this->listeParties.insert(partie2);
    //    this->listeParties.insert(partie3);
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
}

/**
 * @brief Serveur::~Serveur
 */
Serveur::~Serveur()
{
    foreach (QTcpSocket *socket, listeSocketsJoueurs.keys())
    {
        socket->close();
        socket->deleteLater();
    }

    if (this->serveurTCP != NULL)
    {
        this->serveurTCP->close();
        this->serveurTCP->deleteLater();
    }
}

/**
 * @brief Serveur::demarrerServeur
 */
bool Serveur::demarrerServeur()
{
    // Ne démarre pas le serveur si la banque de données n'arrive pas à parse le fichier json.
    if (!this->banqueQuestions->chargerBanque())
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + " - Il y a eu un problème avec la lecture du fichier de question \"data.json\", veuillez vérifier le fichier");
        return false;
    }

    this->serveurTCP = new QTcpServer();

    if (this->serveurTCP->listen(QHostAddress::Any, 25565))
    {
        connect(this->serveurTCP, &QTcpServer::newConnection, this, &Serveur::nouvelleConnexion);
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + " - Serveur en écoute");
        return true;
    }
    else
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + " - Impossible de démarrer le serveur");
        return false;
    }
}

void Serveur::arreterServeur()
{
    foreach (QTcpSocket *socket, listeSocketsJoueurs.keys())
    {
        socket->close();
        socket->deleteLater();
    }

    this->serveurTCP->close();

    QTime heure = QTime::currentTime();
    emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + " - Arrêt du serveur");
}

/**
 * @brief Serveur::nouvelleConnexion
 */
void Serveur::nouvelleConnexion()
{
    while (this->serveurTCP->hasPendingConnections())
        ajouterListeSocket(this->serveurTCP->nextPendingConnection());
}

/**
 * @brief Serveur::ajouterListeSocket
 * @param socket
 */
void Serveur::ajouterListeSocket(QTcpSocket *socket)
{
    listeSocketsJoueurs.insert(socket, new Joueur());
    connect(socket, &QTcpSocket::readyRead, this, &Serveur::lireSocket);
    connect(socket, &QTcpSocket::disconnected, this, &Serveur::rejeterSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &Serveur::afficherErreur);
}

/**
 * @brief Serveur::envoyerListeParties
 */
void Serveur::envoyerListeParties()
{
    // Envoie une message de type "1" (donc la liste des parties) en broadcast.
    foreach (QTcpSocket *socket, listeSocketsJoueurs.keys())
        envoyerSocket(1, QString(), socket);
}

/**
 * @brief Serveur::lireSocket
 */
void Serveur::lireSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket *>(sender());

    // ID de la socket de l'envoyeur.
    //qintptr socketID = socket->socketDescriptor();

    QByteArray data;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> data;

    if (!socketStream.commitTransaction())
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - attente de plus de données de la part de %1").arg(socket->socketDescriptor()));
        return;
    }

    QString enTete = data.mid(0, 28);
    QString messageType = enTete.split(",")[0].split(":")[1];

    data = data.mid(28);

    switch (messageType.toInt())
    {
    case 0: // Message création d'un nom d'utilisateur et l'ajoute à la liste des joueurs.
    {
        QString nomUtilisateur = QString::fromStdString(data.toStdString());

        this->listeSocketsJoueurs[socket]->setNomUtilisateur(nomUtilisateur);

        envoyerSocket(0, QString::number(socket->socketDescriptor()), socket);

        // Ajout d'un timer pour bloquer la socket (il y a un "bug" que je ne sais pas réssoudre qui fait qu'on continue d'écrire dans la socket précédement ouverte)
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        connect(timer, SIGNAL(timeout()), SLOT(envoyerListeParties()));

        timer->start(100);

        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Nouvel utilisateur : \"%1\" - \"%2\"").arg(nomUtilisateur, QString::number(socket->socketDescriptor())));

        break;
    }

    case 1: // Message création d'une partie par un utilisateur.
    {
        Partie::struct_partie sp;
        QHash<QTcpSocket *, Joueur *> listeSUTemp;

        socketStream >> sp.id >> sp.titre >> sp.places >> sp.nombreQuestions;

        // Ajout du joueur ayant créé la partie dans la liste des joueurs de la même partie.
        listeSUTemp.insert(socket, this->listeSocketsJoueurs[socket]);

        Partie *temp = new Partie(sp.id, sp.titre, sp.places, socket, listeSUTemp, sp.nombreQuestions);
        connect(temp, SIGNAL(partieFinie()), this, SLOT(envoyerListeParties()));

        // Ajout de la partie à la liste des parties.
        this->listeParties.insert(temp);

        envoyerListeParties();

        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Nouvelle partie : \"%1\" de %2 places avec %3 questions, créée par \"%4\"").arg(sp.titre, QString::number(sp.places), QString::number(sp.nombreQuestions), this->listeSocketsJoueurs[socket]->getNomUtilisateur()));
        break;
    }

    case 2: // Message utilisateur voulant rejoindre ou quitter une partie.
    {
        QString idPartie = QString::fromStdString(data.toStdString());
        bool rejoindre = enTete.split(",")[1].split(":")[1].split(";")[0].toInt();

        foreach (Partie *partie, this->listeParties)
        {
            // Affiche les joueurs de la partie selectionnée.
            if (partie->getID() == idPartie)
            {
                // Si le joueur demande à rejoindre une partie ou la quitter, il faut qu'il reste de la place et que la partie ne soit pas en cours.
                if (rejoindre)
                {
                    // S'il reste de la place et que la partie n'est pas en cours alors le joueur l'a rejoint.
                    if ((partie->getPlaces() == 0 || partie->getListeJoueurs().size() < partie->getPlaces()) && partie->getStatut() == Partie::enAttente)
                    {
                        partie->addListeJoueurs(socket, this->listeSocketsJoueurs[socket]);
                        QTime heure = QTime::currentTime();
                        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - \"%1\" a rejoint la partie \"%2\"").arg(this->listeSocketsJoueurs[socket]->getNomUtilisateur(), partie->getTitre()));
                    }
                }
                else
                {
                    QTime heure = QTime::currentTime();
                    emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - \"%1\" a quitté la partie \"%2\"").arg(this->listeSocketsJoueurs[socket]->getNomUtilisateur(), partie->getTitre()));

                    if (partie->getHote() == socket)
                    {
                        QSet<Partie *>::iterator partieIterator = this->listeParties.find(partie);
                        if (partieIterator != listeParties.end())
                        {
                            QTime heure = QTime::currentTime();
                            emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - \"%1\" a supprimé la partie \"%2\"").arg(this->listeSocketsJoueurs[socket]->getNomUtilisateur(), partie->getTitre()));

                            this->listeParties.remove(*partieIterator);
                        }
                        delete partie;
                    }
                    else
                        partie->removeListeJoueurs(socket);
                }
            }
        }

        envoyerListeParties();
        break;
    }

    case 3: // L'hôte démarre sa partie.
    {
        foreach (Partie *partie, this->listeParties)
        {
            if (partie->getHote() == socket)
            {
                partie->setListeQuestions(this->banqueQuestions->genererQuiz(partie->getNombreQuestions()));
                QTime heure = QTime::currentTime();
                emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - L'hôte \"%1\" a démarré la partie \"%2\"").arg(this->listeSocketsJoueurs[socket]->getNomUtilisateur(), partie->getTitre()));
                partie->demarrer();
                return;
            }
        }
        envoyerListeParties();
        break;
    }
    case 4: // Message lié aux réponses des utilisateurs.
    {
        qint8 reponse = data.toInt();
        this->listeSocketsJoueurs[socket]->setReponse(reponse);
        break;
    }
    default:
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Message non traité par le serveur par \"%1\"").arg(socket->socketDescriptor()));
        break;
    }
}

/**
 * @brief Serveur::envoyerMessage
 * @param messageType
 * @param message
 * @param socket
 */
void Serveur::envoyerSocket(int messageType, QString message, QTcpSocket *socket)
{
    if (socket && socket->isOpen())
    {
        QDataStream socketStream(socket);
        socketStream.setVersion(QDataStream::Qt_5_15);

        QByteArray enTete;
        QByteArray data;

        switch (messageType)
        {
        case 0:
            enTete = QString("messageType:0,arg:null;").toUtf8();
            enTete.resize(28);

            data = message.toUtf8();
            data.prepend(enTete);

            socketStream << data;
            break;

        case 1: // Envoie un message contenant la liste des parties et des joueurs dans ces parties à la socket.
            enTete = QString("messageType:1,arg:%1;").arg(this->listeParties.size()).toUtf8();
            enTete.resize(28);

            socketStream << enTete;

            foreach (Partie *partie, this->listeParties)
            {
                Partie::struct_partie sp;
                sp.id = partie->getID();
                sp.titre = partie->getTitre();
                sp.statut = partie->getStatut();
                sp.places = partie->getPlaces();
                sp.nombreQuestions = partie->getNombreQuestions();

                Joueur::struct_joueur sj;
                // Transforme la QHash de QTcpSocket/QString en QString/qintptr pour que la QMap trie directement les joueurs alphabétiquement.
                QHashIterator<QTcpSocket *, Joueur *> listeJoueursPartieIterator(partie->getListeJoueurs());
                while (listeJoueursPartieIterator.hasNext())
                {
                    listeJoueursPartieIterator.next();
                    sj.nomUtilisateur = listeJoueursPartieIterator.value()->getNomUtilisateur();
                    sj.score = listeJoueursPartieIterator.value()->getScore();
                    sj.socketId = listeJoueursPartieIterator.key()->socketDescriptor();

                    QList<QString> tmp = {sj.nomUtilisateur, QString::number(sj.score), QString::number(sj.socketId)};
                    sp.listeJoueurPartie.append(tmp);
                }

                sp.hote = partie->getHote()->socketDescriptor();

                socketStream << sp.id << sp.titre << sp.statut << sp.places << sp.hote << sp.listeJoueurPartie << sp.nombreQuestions;
            }
            break;
        }
    }
    else
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Impossible d'envoyer le message, le joueur \"%1\" est déconnecté").arg(socket->socketDescriptor()));
    }
}

/**
 * @brief Serveur::rejeterSocket
 */
void Serveur::rejeterSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket *>(sender());

    QHash<QTcpSocket *, Joueur *>::iterator listeSocketIterator = this->listeSocketsJoueurs.find(socket);
    if (listeSocketIterator != this->listeSocketsJoueurs.end())
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Joueur déconnecté : \"%1\" - \"%2\"").arg(this->listeSocketsJoueurs[socket]->getNomUtilisateur(), QString::number(socket->socketDescriptor())));

        // Si le joueur était maître d'une partie, le serveur supprime cette partie. Sinon enlève le joueur de la partie.
        foreach (Partie *partie, this->listeParties)
            if (partie->getHote() == socket)
            {
                QSet<Partie *>::iterator partieIterator = this->listeParties.find(partie);
                if (partieIterator != listeParties.end())
                {
                    QTime heure = QTime::currentTime();
                    emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - \"%1\" s'est déconnecté et était hôte de la partie \"%2\"").arg(this->listeSocketsJoueurs[socket]->getNomUtilisateur(), partie->getTitre()));

                    this->listeParties.remove(*partieIterator);
                }
                partie->deleteLater();
            }
            else
                partie->removeListeJoueurs(socket);

        this->listeSocketsJoueurs.remove(socket);
    }

    socket->deleteLater();

    envoyerListeParties();
}

/**
 * @brief Serveur::afficherErreur
 * @param erreurSocket
 */
void Serveur::afficherErreur(QAbstractSocket::SocketError erreurSocket)
{
    switch (erreurSocket)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Aucun hôte trouvé, vérifiez le nom de l'hôte et le port"));
        break;
    }
    case QAbstractSocket::ConnectionRefusedError:
    {
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Connexion refusée"));
        break;
    }
    default:
        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
        QTime heure = QTime::currentTime();
        emit infoGUI("[" + heure.toString("hh:mm:ss") + "]" + QString(" - Une erreur s'est produite : \"%1\"").arg(socket->errorString().toUtf8().constData()));
        break;
    }
}
