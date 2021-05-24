#ifndef JOUEUR_H
#define JOUEUR_H

#include <QObject>

class Joueur : public QObject
{
    Q_OBJECT
public:
    explicit Joueur(QObject *parent = nullptr);

    typedef struct
    {
        QString nomUtilisateur;
        qint8 score;
        qintptr socketId;
    } struct_joueur;

    QString getNomUtilisateur() const;
    qint8 getScore() const;
    qint8 getReponse() const;

    void setNomUtilisateur(QString nom);
    void setScore(qint8 score);
    void setReponse(qint8 reponse);
signals:

private:
    QString nomUtilisateur;
    qint8 score;
    qint8 reponse;
};

#endif // JOUEUR_H
