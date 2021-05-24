#ifndef PARTIE_H
#define PARTIE_H

#include <QObject>
#include <QtNetwork>
#include <QUuid>
#include "joueur.h"
#include "question.h"

class Partie : public QObject
{
    Q_OBJECT
public:
    typedef enum
    {
        enAttente,
        enCours
    } statutPartie;

    explicit Partie(QString id, QString titre, qint8 places, QTcpSocket *hote, QHash<QTcpSocket *, Joueur *> listeJoueurs, qint8 nombreQuestions, QObject *parent = nullptr);

    ~Partie();

    typedef struct
    {
        QString id;
        QString titre;
        qint8 places;
        statutPartie statut;
        qintptr hote;
        QList<QList<QString>> listeJoueurPartie;
        qint8 nombreQuestions;
    } struct_partie;

    QString getID() const;
    QString getTitre() const;
    statutPartie getStatut() const;
    qint8 getPlaces() const;
    QHash<QTcpSocket *, Joueur *> getListeJoueurs() const;
    QTcpSocket *getHote() const;
    qint8 getNombreQuestions() const;

    void addListeJoueurs(QTcpSocket *socket, Joueur *joueur);
    void removeListeJoueurs(QTcpSocket *socket);

    void demarrer();

    // POUR TESTER, A SUPPRIMER;
    void setStatut(statutPartie statut);
    void setListeQuestions(std::vector<Question *> question);

signals:
    void partieFinie();

private:
    QString id;
    QString titre;
    statutPartie statut;
    qint8 places;
    QTcpSocket *hote;
    QHash<QTcpSocket *, Joueur *> listeJoueurs;
    qint8 nombreQuestions;
    Question *questionEnCours;
    std::vector<Question *> listeQuestions;
    QTimer *timerQuestion;

    void envoyerQuestion(QString type, QString arg);
    void verifierQuestion();

private slots:
    void nextQuestion();
};

#endif // PARTIE_H
