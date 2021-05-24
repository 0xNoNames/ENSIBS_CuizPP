#include "dialogpartie.h"
#include "ui_dialogpartie.h"

/**
 * @brief DialogPartie::DialogPartie
 * @param parent
 */
DialogPartie::DialogPartie(QWidget *parent) : QDialog(parent),
                                              ui(new Ui::DialogPartie)
{
    ui->setupUi(this);
}

/**
 * @brief DialogPartie::~DialogPartie
 */
DialogPartie::~DialogPartie()
{
    delete ui;
}

void DialogPartie::on_okBouton_clicked()
{
    try
    {
        // Si les entrées sont trop grandes, on déclenche un erreur.
        if (this->ui->titrePartie->text().size() > 12)
            throw ExceptionClient(ExceptionClient::erreurEnum::chaineTropLongueErreur, this);

        if (this->ui->placesPartie->text().size() > 2)
            throw ExceptionClient(ExceptionClient::erreurEnum::nbPartiePlacesErreur, this);

        // Si les entrées sont trop petites, on déclenche un erreur.
        if (this->ui->titrePartie->text().trimmed().isEmpty() || this->ui->placesPartie->text().trimmed().isEmpty() || this->ui->titrePartie->text().size() < 3)
        {
            throw ExceptionClient(ExceptionClient::erreurEnum::chaineTropCourteErreur, this);
        }
        // Si les entrées contiennent des caractères non ascii, on déclenche un erreur.
        foreach (const QChar c, this->ui->titrePartie->text())
            if ((c).isNonCharacter())
                throw ExceptionClient(ExceptionClient::erreurEnum::caractereNonAsciiErreur, this);

        // Si l'entrée du nombre de places n'est pas un nombre, on déclenche un erreur.
        foreach (const QChar c, this->ui->placesPartie->text())
            if (!(c).isNumber())
                throw ExceptionClient(ExceptionClient::erreurEnum::caractereNonNumeriqueErreur, this);

        this->close();
        this->setResult(QDialog::Accepted);
    }
    catch (ExceptionClient erreur)
    {
        erreur.afficherErreur();
    }
}

/**
 * @brief DialogPartie::getTitre
 * @return
 */
QString DialogPartie::getTitre() const
{
    return this->ui->titrePartie->text();
}

/**
 * @brief DialogPartie::getPlaces
 * @return
 */
qint8 DialogPartie::getPlaces() const
{
    return this->ui->placesPartie->text().toInt();
}

qint8 DialogPartie::getNombreQuestions() const
{
    return this->ui->nbQuestions->text().toInt();
}
