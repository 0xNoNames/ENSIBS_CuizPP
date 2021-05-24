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
    Q_ENUM(enumType)

    explicit Question(QString id, enumDifficulte difficulte, qint8 reponse, QObject *parent = nullptr);
    explicit Question(const Question &q);
    explicit Question();

    static enumDifficulte stringToEnum(QString s);

    qint8 getPoints() const;
    qint16 getTimer() const;
    QString getID() const;
    enumDifficulte getDifficulte() const;
    qint8 getReponse() const;
    virtual Question::enumType getType() const = 0;

protected:
    QString id;
    enumDifficulte difficulte;
    qint8 reponse;
    qint16 timer;
    qint8 points;
};

#endif // QUESTION_H
