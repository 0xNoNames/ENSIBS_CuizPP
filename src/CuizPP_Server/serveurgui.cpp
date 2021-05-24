#include "serveurgui.h"
#include "ui_serveurgui.h"

ServeurGUI::ServeurGUI(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::ServeurGUI)
{
    ui->setupUi(this);

    // Créer un nouvel objet Serveur en parent celle classe.
    this->serveur = new Serveur(this);

    // Permet d'afficher une info émise par le serveur et de l'afficher.
    connect(this->serveur, SIGNAL(infoGUI(QString)), this, SLOT(afficherInfo(QString)));
}

ServeurGUI::~ServeurGUI()
{
    delete this->serveur;
    delete ui;
}

void ServeurGUI::on_dabouton_clicked()
{
    // Si le serveur n'est pas démarré, on le démarre.
    if (this->ui->dabouton->text() == "Démarrer serveur")
    {
        if (this->serveur->demarrerServeur())
            this->ui->dabouton->setText("Arrêter serveur");
    }
    // Si le serveur est démarré, on l'arrête.
    else
    {
        this->serveur->arreterServeur();
        this->ui->dabouton->setText("Démarrer serveur");
    }
};

void ServeurGUI::afficherInfo(const QString &info)
{
    // Ajoute l'info aux log.
    this->ui->logWidget->addItem(info);
    this->ui->logWidget->scrollToBottom();
}
