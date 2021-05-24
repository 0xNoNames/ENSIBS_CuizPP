#include "banquequestions.h"

BanqueQuestions::BanqueQuestions(QObject *parent) : QObject(parent)
{
}

bool BanqueQuestions::chargerBanque()
{
    this->data = this->lireDataJson();

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
        QString sujet = value["sujet"].toString();
        //
        std::vector<QString> choix = std::vector<QString>();
        QJsonArray a = value["choix"].toArray();
        for (int i(0); i < a.size(); i++)
        {
            choix.push_back(a[i].toString());
        }
        //
        TypeTexte *question = new TypeTexte(id, difficulte, sujet, choix);
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
        QString idimg = value["idImage"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        QString sujet = value["sujet"].toString();
        //
        std::vector<QString> choix = std::vector<QString>();
        QJsonArray a = value["choix"].toArray();
        for (int i(0); i < a.size(); i++)
        {
            choix.push_back(a[i].toString());
        }
        //
        TypeImage *question = new TypeImage(id, idimg, difficulte, sujet, choix);
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
        QString idaud = value["idAudio"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        QString sujet = value["sujet"].toString();
        //
        std::vector<QString> choix = std::vector<QString>();
        QJsonArray a = value["choix"].toArray();
        for (int i(0); i < a.size(); i++)
        {
            choix.push_back(a[i].toString());
        }
        //
        TypeAudio *question = new TypeAudio(id, idaud, difficulte, sujet, choix);
        this->banqueAudio.push_back(question);
    }

    /* Parsing des questions chimeres */
    QJsonArray chimere = data["chimere"].toArray();
    for (int i(0); i < chimere.size(); i++)
    {
        //
        QJsonObject value = chimere[i].toObject();
        //
        QString id = value["id"].toString();
        //
        QString idaud = value["idAudio"].toString();
        //
        QString idimg = value["idImage"].toString();
        //
        Question::enumDifficulte difficulte = Question::stringToEnum(value["difficulte"].toString());
        //
        QString sujet = value["sujet"].toString();
        //
        std::vector<QString> choix = std::vector<QString>();
        QJsonArray a = value["choix"].toArray();
        for (int i(0); i < a.size(); i++)
        {
            choix.push_back(a[i].toString());
        }
        //
        TypeChimere *question = new TypeChimere(id, idaud, idimg, difficulte, sujet, choix);
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

    // Convert to QJsonDocument
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &errorPtr);
    // Renvoie le document
    return doc;
};

Question *BanqueQuestions::getQuestionByID(QString ID, Question::enumType type)
{
    switch (type)
    {
    case Question::enumType::texte:
    {
        foreach (TypeTexte *question, this->banqueTexte)
            if (question->getID() == ID)
                return question;
        break;
    }

    case Question::enumType::audio:
    {
        foreach (TypeAudio *question, this->banqueAudio)
            if (question->getID() == ID)
                return question;
        break;
    }

    case Question::enumType::image:
    {
        foreach (TypeImage *question, this->banqueImage)
            if (question->getID() == ID)
                return question;
        break;
    }

    case Question::enumType::chimere:
    {
        foreach (TypeChimere *question, this->banqueChimere)
            if (question->getID() == ID)
                return question;
        break;
    }
    }
}
