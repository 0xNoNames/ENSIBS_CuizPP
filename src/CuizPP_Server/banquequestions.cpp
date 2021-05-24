#include "banquequestions.h"

BanqueQuestions::BanqueQuestions(QObject *parent) : QObject(parent)
{
}

bool BanqueQuestions::chargerBanque()
{
    this->data = lireDataJson();

    if (this->data.isNull())
        return false;

    this->banqueTexte = std::vector<TypeTexte *>();
    this->banqueImage = std::vector<TypeImage *>();
    this->banqueAudio = std::vector<TypeAudio *>();
    this->banqueChimere = std::vector<TypeChimere *>();

    /* Parsing des questions textes */
    QJsonArray txt = data["texte"].toArray();
    for (int i(0); i < txt.size(); i++)
    {
        //
        QJsonObject value = txt[i].toObject();
        //
        QString id = value["id"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        qint8 reponse = value["reponse"].toInt();
        //
        TypeTexte *question = new TypeTexte(id, difficulte, reponse);
        this->banqueTexte.push_back(question);
    }

    /* Parsing des questions images */
    QJsonArray img = data["image"].toArray();
    for (int i(0); i < img.size(); i++)
    {
        //
        QJsonObject value = img[i].toObject();
        //
        QString id = value["id"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        qint8 reponse = value["reponse"].toInt();
        //
        TypeImage *question = new TypeImage(id, difficulte, reponse);
        this->banqueImage.push_back(question);
    }

    /* Parsing des questions audios */
    QJsonArray audio = data["audio"].toArray();
    for (int i(0); i < audio.size(); i++)
    {
        //
        QJsonObject value = audio[i].toObject();
        //
        QString id = value["id"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        qint8 reponse = value["reponse"].toInt();
        //
        TypeAudio *question = new TypeAudio(id, difficulte, reponse);
        this->banqueAudio.push_back(question);
    }

    /* Parsing des questions chimères */
    QJsonArray chimere = data["chimere"].toArray();
    for (int i(0); i < chimere.size(); i++)
    {
        //
        QJsonObject value = chimere[i].toObject();
        //
        QString id = value["id"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        qint8 reponse = value["reponse"].toInt();
        //
        TypeChimere *question = new TypeChimere(id, difficulte, reponse);
        this->banqueChimere.push_back(question);
    }

    return true;
}

QJsonDocument BanqueQuestions::lireDataJson()
{
    // Lecture du fichier
    QFile file("./data.json");
    bool readSuccess = file.open(QIODevice::ReadOnly);
    QByteArray rawData;

    //Vérification de la lecture
    if (readSuccess)
    {
        rawData = file.readAll();
        file.close();
    }

    // Convertie le fichier en un object QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(rawData);

    // Retourne le document
    return doc;
};

std::vector<Question *> BanqueQuestions::genererQuiz(qint8 nombreQuestions)
{
    std::vector<Question *> quiz;
    qint8 indexQuestion;
    qint8 i = 0;

    // Ajoute de manière aléatoire des questions de chaque type.

    while (i < nombreQuestions)
    {
        switch (rand() % 4)
        {
        case 0:
        {
            indexQuestion = rand() % this->banqueTexte.size();
            // Permet de ne pas avoir de questions en doublon.
            if (std::find(quiz.begin(), quiz.end(), this->banqueTexte[indexQuestion]) != quiz.end())
                continue;
            quiz.push_back(this->banqueTexte[indexQuestion]);
            ++i;
            break;
        }
        case 1:
        {
            indexQuestion = rand() % this->banqueImage.size();
            // Permet de ne pas avoir de questions en doublon.
            if (std::find(quiz.begin(), quiz.end(), this->banqueImage[indexQuestion]) != quiz.end())
                continue;
            quiz.push_back(this->banqueImage[indexQuestion]);
            ++i;
            break;
        }
        case 2:
        {
            indexQuestion = rand() % this->banqueAudio.size();
            // Permet de ne pas avoir de questions en doublon.
            if (std::find(quiz.begin(), quiz.end(), this->banqueAudio[indexQuestion]) != quiz.end())
                continue;
            quiz.push_back(this->banqueAudio[indexQuestion]);
            ++i;
            break;
        }
        case 3:
        {
            indexQuestion = rand() % this->banqueChimere.size();
            // Permet de ne pas avoir de questions en doublon.
            if (std::find(quiz.begin(), quiz.end(), this->banqueChimere[indexQuestion]) != quiz.end())
                continue;
            quiz.push_back(this->banqueChimere[indexQuestion]);
            ++i;
            break;
        }
        }
    }
    return quiz;
}
