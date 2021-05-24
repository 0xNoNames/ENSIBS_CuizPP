#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QListWidget>
#include <QPixmap>
#include <QtMultimedia/QMediaPlayer>
#include "partie.h"
#include "joueur.h"
#include "banquequestions.h"
#include "dialogconnexion.h"
#include "dialogpartie.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Client;
}
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void lireSocket();
    void rejeterSocket();
    void afficherErreurSocket(QAbstractSocket::SocketError erreurSocket);

    void onQuitterPartieBoutonClicked();
    void onCreerPartieBoutonClicked();
    void onRejoindrePartieBoutonClicked();
    void onPartieDoubleClicked(QListWidgetItem *partie);
    void onPartieClicked(QListWidgetItem *partie);
    void onDemarrerBoutonClicked();
    void onReponseBoutonClicked();
    void majTimer();

private:
    Ui::Client *ui;
    QTcpSocket *socket;
    QString nomUtilisateur;
    qintptr socketOnServeurID;
    QList<Partie *> listeParties;
    QString selectionPartie;
    Partie *clientPartie;
    bool enPartie;
    qint8 reponse;

    BanqueQuestions *banqueQuestions;
    Question *questionEnCours;
    QTimer *questionTimer;
    qint8 questionTempsActuel;
    qint8 questionCompteur;
    QMediaPlayer *sonPlayer;

    void connecterSocket();
    void envoyerSocket(int messageType, QString arg = QString(), QString message = QString(), Partie::struct_partie sp = Partie::struct_partie());
    void updateAffichage();
};
#endif // CLIENT_H
