#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>

class Question : public QObject
{
    Q_OBJECT
public:
    typedef enum
    {
        facile,
        moyenne,
        difficile
    } enumDifficulte;

    typedef enum
    {
        texte,
        audio,
        image,
        chimere
    } enumType;

    explicit Question(QString id, enumDifficulte difficulte, QString Sujet, std::vector<QString> choix, QObject *parent = nullptr);
    explicit Question(const Question &q);
    explicit Question();

    static enumDifficulte stringToEnum(QString s);

    qint8 getTimer() const;
    QString getID() const;
    enumDifficulte getDifficulte() const;
    QString getSujet() const;
    std::vector<QString> getChoix() const;

    virtual QString getNomFichier() const = 0;

protected:
    QString id;
    enumDifficulte difficulte;
    QString sujet;
    std::vector<QString> choix;
    qint32 timer;
};

#endif // QUESTION_H
