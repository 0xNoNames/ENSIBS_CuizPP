#include "dialogconnexion.h"
#include "ui_dialogconnexion.h"

/**
 * @brief DialogConnexion::DialogConnexion
 * @param parent
 */
DialogConnexion::DialogConnexion(QWidget *parent) : QDialog(parent),
                                                    ui(new Ui::DialogConnexion)
{
    this->ui->setupUi(this);

    connect(this->ui->ipInput, SIGNAL(returnPressed()), this->ui->connecterBouton, SIGNAL(clicked()));
    connect(this->ui->nomUtilisateurInput, SIGNAL(returnPressed()), this->ui->connecterBouton, SIGNAL(clicked()));
}

/**
 * @brief DialogConnexion::~DialogConnexion
 */
DialogConnexion::~DialogConnexion()
{
    delete this->ui;
}

/**
 * @brief DialogConnexion::on_connectButton_clicked
 */
void DialogConnexion::on_connecterBouton_clicked()
{
    try
    {
        // Si les entrées sont trop grandes, on déclenche un erreur.
        if (this->ui->nomUtilisateurInput->text().size() > 12)
            throw ExceptionClient(ExceptionClient::erreurEnum::chaineTropLongueErreur, this);

        // Si les entrées sont trop petites, on déclenche un erreur.
        if (this->ui->ipInput->text().trimmed().isEmpty() || this->ui->nomUtilisateurInput->text().isEmpty() || this->ui->nomUtilisateurInput->text().size() < 3)
        {
            throw ExceptionClient(ExceptionClient::erreurEnum::chaineTropCourteErreur, this);
        }

        // Si les entrées contiennent des caractères non ascii, on déclenche un erreur.
        foreach (const QChar c, this->ui->nomUtilisateurInput->text())
            if ((c).isNonCharacter())
                throw ExceptionClient(ExceptionClient::erreurEnum::caractereNonAsciiErreur, this);

        this->close();
        this->setResult(QDialog::Accepted);
    }
    catch (ExceptionClient erreur)
    {
        erreur.afficherErreur();
    }
}

/**
 * @brief DialogConnexion::getIp
 * @return
 */
QString DialogConnexion::getIp() const
{
    return this->ui->ipInput->text();
}

/**
 * @brief DialogConnexion::getUsername
 * @return
 */
QString DialogConnexion::getNomUtilisateur() const
{
    return this->ui->nomUtilisateurInput->text();
}
