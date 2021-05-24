#ifndef BANQUEQUESTIONS_H
#define BANQUEQUESTIONS_H

#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <vector>
#include "question.h"
#include "typetexte.h"
#include "typeimage.h"
#include "typeaudio.h"
#include "typechimere.h"

class BanqueQuestions : public QObject
{
    Q_OBJECT
public:
    explicit BanqueQuestions(QObject *parent = nullptr);

    bool chargerBanque();
    Question *getQuestionByID(QString ID, Question::enumType type);

private:
    QJsonDocument data;
    std::vector<TypeTexte *> banqueTexte;
    std::vector<TypeImage *> banqueImage;
    std::vector<TypeAudio *> banqueAudio;
    std::vector<TypeChimere *> banqueChimere;

    QJsonDocument lireDataJson();
};

#endif // BANQUEQUESTIONS_H
