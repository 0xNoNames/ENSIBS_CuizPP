#ifndef JOUEUR_H
#define JOUEUR_H

#include <QObject>

class Joueur : public QObject
{
    Q_OBJECT
public:
    explicit Joueur(QString nomUtilisateur, qint8 score, QObject *parent = nullptr);
    explicit Joueur(const Joueur &joueur);

    typedef struct
    {
        QString nomUtilisateur;
        qint8 score;
        qintptr socketId;
    } struct_joueur;

    QString getNomUtilisateur() const;
    qint8 getScore() const;

    void setNomUtilisateur(QString nom);
    void setScore(qint8 score);
    void setPartie(QString partie);

    bool operator<(const Joueur &joueur) const;
    bool operator>(const Joueur &joueur) const;

private:
    QString nomUtilisateur;
    qint8 score;
};

#endif // JOUEUR_H
