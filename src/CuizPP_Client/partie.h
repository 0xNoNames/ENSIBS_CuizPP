#ifndef PARTIE_H
#define PARTIE_H

#include <QObject>
#include <QtNetwork>
#include <QUuid>
#include "joueur.h"

class Partie : public QObject
{
    Q_OBJECT
public:
    typedef enum
    {
        enAttente,
        enCours
    } statutPartie;

    explicit Partie(QString id, QString titre, statutPartie statut, qint8 places, qintptr hote, QMap<Joueur, qintptr> listeJoueurs, qint8 nombreQuestions, QObject *parent = nullptr);

    typedef struct
    {
        QString id;
        QString titre;
        qint8 places;
        statutPartie statut;
        qintptr hote;
        QList<QList<QString>> listeJoueur;
        qint8 nombreQuestions;
    } struct_partie;

    static QString creerID();
    static bool PartieComparator(const Partie *p1, const Partie *p2);

    QString getID() const;
    QString getTitre() const;
    statutPartie getStatut() const;
    qint8 getPlaces() const;
    QMap<Joueur, qintptr> getListeJoueurs() const;
    qint8 getNombreJoueurs() const;
    qintptr getHote() const;
    qint8 getNombreQuestions() const;

private:
    QString id;
    QString titre;
    statutPartie statut;
    qint8 places;
    qintptr hote;
    QMap<Joueur, qintptr> listeJoueurs;
    qint8 nombreQuestions;
};

#endif // PARTIE_H
