#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QtNetwork>
#include "partie.h"
#include "joueur.h"
#include "banquequestions.h"

class Serveur : public QObject
{
    Q_OBJECT
public:
    explicit Serveur(QObject *parent = nullptr);
    ~Serveur();

    bool demarrerServeur();
    void arreterServeur();
    void envoyerSocket(int messageType, QString message, QTcpSocket *socket);
    //void sendAttachment(QTcpSocket *socket, QString filePath);

signals:
    void infoGUI(QString);

private slots:
    void nouvelleConnexion();
    void ajouterListeSocket(QTcpSocket *socket);
    void envoyerListeParties();

    void lireSocket();
    void rejeterSocket();
    void afficherErreur(QAbstractSocket::SocketError erreurSocket);

private:
    QSet<Partie *> listeParties;
    QTcpServer *serveurTCP;
    QHash<QTcpSocket *, Joueur *> listeSocketsJoueurs;

    BanqueQuestions *banqueQuestions;
};

#endif // SERVEUR_H
