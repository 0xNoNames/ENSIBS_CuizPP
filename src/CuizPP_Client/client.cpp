#include "client.h"
#include "ui_client.h"

/**
 * @brief Client::Client
 * @param parent
 */
Client::Client(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Client)
{
    this->ui->setupUi(this);

    this->banqueQuestions = new BanqueQuestions();

    // Cache la liste des joueurs d'une partie selectionnée.
    this->ui->listeJoueursAccueil->setVisible(false);

    // Cache le bouton "démarrer la partie" de la page d'attente.
    this->ui->demarrerBouton->setVisible(false);

    // Affiche la page de selection des parties.
    this->ui->stackedWidget->setCurrentIndex(0);

    // Le joueur ne commence pas dans un partie.
    this->enPartie = false;

    // Initialisation du compteur de question.
    this->questionCompteur = 0;

    // Initialisation du player audio et du timer.
    this->sonPlayer = new QMediaPlayer();
    this->questionTimer = new QTimer();

    try
    {
        if (this->banqueQuestions->chargerBanque())
            connecterSocket();
        else
            throw ExceptionClient(ExceptionClient::ExceptionClient::problemeJson, this);
    }
    catch (ExceptionClient ec)
    {
        ec.afficherErreur();
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Client::~Client
 */
Client::~Client()
{
    if (this->socket->isOpen())
        this->socket->close();
    delete this->ui;
}

/**
 * @brief Client::updateListePartie
 */
void Client::updateAffichage()
{
    // Nettoie la liste des parties.
    this->ui->listePartiesAccueil->clear();

    // Permet d'actualiser directement la liste des joueurs à l'écran de selection des parties.
    this->ui->listeJoueursAccueil->clear();

    // Permet d'actualiser directement la liste des joueurs dans la salle d'attente.
    this->ui->listeJoueursAttente->clear();

    // Si le joueur n'a rien selectionné, enlève la liste des joueurs.
    this->ui->placesJoueursAccueil->setText("");
    this->ui->listeJoueursAccueil->setVisible(false);

    foreach (Partie *partie, this->listeParties)
    {
        // Récupère l'ID de la partie, son titre et le nombre de places libres.
        QString idPartie = partie->getID();
        QString titrePartie;
        if (partie->getPlaces() == 0)
            titrePartie = partie->getTitre() + "  -  " + QString::number(partie->getListeJoueurs().size()) + "/inf";
        else
            titrePartie = partie->getTitre() + "  -  " + QString::number(partie->getListeJoueurs().size()) + "/" + QString::number(partie->getPlaces());

        // Rajoute la partie à la liste des parties.
        QListWidgetItem *item = new QListWidgetItem(titrePartie);

        // Ajoute l'ID de la partie à l'item pour différencier deux parties qui auraient le même nom.
        QVariant v;
        v.setValue(idPartie);

        item->setData(Qt::UserRole, v);
        item->setTextAlignment(Qt::AlignCenter);

        // Change la couleur de la partie pour selon son statut (rouge -> pleine, orange -> en cours, verte -> rejoignable).
        if (partie->getStatut() == Partie::enCours)
            item->setForeground(QColor(255, 128, 0));
        else if (partie->getListeJoueurs().size() == partie->getPlaces())
            item->setForeground(Qt::red);
        else
            item->setForeground(Qt::green);

        // Ajoute la partie à la liste des parties à l'écran de selection des parties.
        this->ui->listePartiesAccueil->addItem(item);

        // Affiche les joueurs de la partie selectionnée (si le joueur est dans une partie, "selectionPartie" est forcément modifié).
        if (partie->getID() == this->selectionPartie)
        {
            // Affiche la liste des joueurs.
            this->ui->listeJoueursAccueil->setVisible(true);

            // Si le joueur n'est plus dans une partie.
            if (!partie->getListeJoueurs().values().contains(this->socketOnServeurID))
            {
                // Le redéplace à l'écran de selection des parties s'il n'y est pas.
                if (this->ui->stackedWidget->currentIndex() != 0)
                {
                    this->ui->stackedWidget->setCurrentIndex(0);
                    // Initialise la variable de partie selectionnée.
                    this->selectionPartie = QString();
                    this->enPartie = false;
                }
            }
            else
            {
                // Alors le joueur est une partie et réinitialise ses attributs.
                this->enPartie = true;
                this->clientPartie = partie;
            }

            // Si le joueur est dans une partie.
            if (this->enPartie)
            {
                switch (partie->getStatut())
                {
                case Partie::enAttente:
                    // Reconnecte directement le joueur à la partie.
                    if (this->ui->stackedWidget->currentIndex() != 1 && partie->getListeJoueurs().values().contains(this->socketOnServeurID))
                        this->ui->stackedWidget->setCurrentIndex(1);

                    // Modifie le titre de la fenêtre.
                    this->setWindowTitle("Salle d'attente de la partie : " + partie->getTitre());

                    // Si le joueur est l'hôte, il peut démarrer la partie.
                    if (this->socketOnServeurID == partie->getHote())
                        this->ui->demarrerBouton->setVisible(true);
                    else
                        this->ui->demarrerBouton->setVisible(false);

                    // Affiche le nombre de places dans la salle d'attente.
                    if (partie->getPlaces() == 0)
                        this->ui->infoPartieAttente->setText(partie->getTitre() + '\n' + QString::number(partie->getListeJoueurs().size()) + " / inf" + '\n' + QString::number(partie->getNombreQuestions()) + " questions");
                    else
                        this->ui->infoPartieAttente->setText(partie->getTitre() + '\n' + QString::number(partie->getListeJoueurs().size()) + " / " + QString::number(partie->getPlaces()) + '\n' + QString::number(partie->getNombreQuestions()) + " questions");
                    break;

                case Partie::enCours:
                    // Reconnecte directement le joueur à la partie.
                    if (this->ui->stackedWidget->currentIndex() != 2 && partie->getListeJoueurs().values().contains(this->socketOnServeurID))
                        this->ui->stackedWidget->setCurrentIndex(2);

                    // Modifie le titre de la fenêtre.
                    this->setWindowTitle("Salle de jeu de la partie : " + partie->getTitre());
                }
            }
            else
            {
                // Affiche le nombre de places restantes dans la partie selectionnée à l'écran de selection des parties.
                if (partie->getPlaces() == 0)
                    this->ui->placesJoueursAccueil->setText(partie->getTitre() + '\n' + QString::number(partie->getListeJoueurs().size()) + " / inf");
                else
                    this->ui->placesJoueursAccueil->setText(partie->getTitre() + '\n' + QString::number(partie->getListeJoueurs().size()) + " / " + QString::number(partie->getPlaces()));
            }

            //  Iterator sur la map des parties.
            QMapIterator<Joueur, qintptr> joueursIterator(partie->getListeJoueurs());
            // Tant qu'il y a des éléments, on avance.
            while (joueursIterator.hasNext())
            {
                joueursIterator.next();

                // Crée un item ayant ayant le nom du joueur, le centre dans la QListWidget et supprime les intéractions avec celui-ci.
                QListWidgetItem *item = new QListWidgetItem(joueursIterator.key().getNomUtilisateur() + " : " + QString::number(joueursIterator.key().getScore()));
                item->setTextAlignment(Qt::AlignCenter);
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);

                // Met le maître de la partie en rouge.
                if (joueursIterator.value() == partie->getHote())
                    item->setForeground(Qt::red);

                // Si le joueur est dans une partie.
                if (this->enPartie)
                { //this->ui->stackedWidget->currentIndex() == 1) {
                    // Ajoute le joueur dans la QListWidget de la salle d'attente et l'affiche.
                    this->ui->listeJoueursAttente->addItem(item);
                }
                // Sinon si le joueur est à l'écran de selection des parties.
                else
                    // Ajoute le joueur dans la QListWidget de l'accueil et l'affiche.
                    this->ui->listeJoueursAccueil->addItem(item);
            }
        }
    }
    if (!this->enPartie)
    {
        // Le joueur n'est alors plus dans une partie, il faut le faire revenir au menu principal.
        if (this->ui->stackedWidget->currentIndex() != 0)
        {
            this->ui->stackedWidget->setCurrentIndex(0);
            this->ui->placesJoueursAccueil->setText("");
            this->ui->listeJoueursAccueil->setVisible(false);
            this->questionCompteur = 0;
        }
        this->setWindowTitle("Choisissez ou créez une partie.");
    }

    this->enPartie = false;
}

void Client::envoyerSocket(int messageType, QString arg, QString message, Partie::struct_partie sp)
{
    QDataStream socketStream(this->socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    QByteArray enTete;
    QByteArray data;

    if (messageType == 1)
    {
        // Message contentant les informations permettant de créer une partie.
        enTete = QString("messageType:1,arg:%1;").arg(arg).toUtf8();
        enTete.resize(28);

        socketStream << enTete << sp.id << sp.titre << sp.places << sp.nombreQuestions;
    }
    else
    {
        // messageType == 0 : Envoyer son nom d'utilisateur à la base de données.
        // messageType == 2 : Message spécifiant si le joueur veut quitter ou rejoindre une partie.
        // messageType == 3 : Message spécifiant le démarrage de la partie par l'hôte.
        // messageType == 4 : Message spécifiant la réponse à envoyer.
        enTete = QString("messageType:%1,arg:%2;").arg(messageType).arg(arg).toUtf8();
        enTete.resize(28);

        data = message.toUtf8();
        data.prepend(enTete);

        socketStream << data;
    }
}

/**
 * @brief Client::lireSocket
 */
void Client::lireSocket()
{
    QByteArray data;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> data;

    if (!socketStream.commitTransaction())
    {
        qDebug() << "En attente de plus de données depuis le serveur.";
        return;
    }

    QString enTete = data.mid(0, 28);
    QString typeMessage = enTete.split(",")[0].split(":")[1];

    data = data.mid(28);

    switch (typeMessage.toInt())
    {
    case 0: // Message provenant du serveur afin de stocker l'id de la socket présente sur le serveur.
        this->socketOnServeurID = data.toInt();
        break;

    case 1: // Message provenant du serveur afin de mettre à jour la listes des parties.
    {
        qintptr nbParties = (enTete.split(",")[1].split(":")[1].split(";")[0]).toInt();
        QList<Partie *> tempListe = QList<Partie *>();

        for (int i(0); i < nbParties; ++i)
        {
            Partie::struct_partie sp;
            Joueur::struct_joueur sj;

            // Récupère le flux de données que le serveur lui envoie et place dans tout les "struct_partie".
            socketStream >> sp.id >> sp.titre >> sp.statut >> sp.places >> sp.hote >> sp.listeJoueur >> sp.nombreQuestions;

            // Met la listeJoueurPartie dans une QMap.
            QMap<Joueur, qintptr> tempListeJoueurs;
            foreach (QList<QString> list, sp.listeJoueur)
            {
                sj.nomUtilisateur = list[0];
                sj.score = list[1].toInt();
                sj.socketId = list[2].toInt();

                tempListeJoueurs.insert(Joueur(sj.nomUtilisateur, sj.score), sj.socketId);
            }
            Partie *partie = new Partie(sp.id, sp.titre, sp.statut, sp.places, sp.hote, tempListeJoueurs, sp.nombreQuestions);

            tempListe.append(partie);
        }

        this->listeParties = tempListe;

        std::sort(this->listeParties.begin(), this->listeParties.end(), Partie::PartieComparator);

        updateAffichage();

        break;
    }
    case 4: // Message traitant les questions.
    {
        // Déplace le joueur sur la vue "EnCours".
        this->questionCompteur += 1;
        if (this->ui->stackedWidget->currentIndex() != 2)
            this->ui->stackedWidget->setCurrentIndex(2);

        QString questionID = enTete.split(",")[1].split(":")[1].split(";")[0];
        Question::enumType questionType = (Question::enumType)data.toInt();

        this->questionEnCours = banqueQuestions->getQuestionByID(questionID, questionType);

        // Récupère le temps max de réponse de la question et l'affiche.
        this->questionTempsActuel = this->questionEnCours->getTimer();
        this->ui->timerLabel->setText(QString::number(questionTempsActuel));

        // Créer un nouvel objet QTimer et le connecte au SLOT permettant de mettre à jour l'affichage du timer.
        if (this->questionTimer->isActive())
            delete this->questionTimer;
        this->questionTimer = new QTimer();
        connect(this->questionTimer, SIGNAL(timeout()), this, SLOT(majTimer()));

        // Met à jour chaque seconde le timer.
        this->questionTimer->start(1000);

        // Change la couleur du label contant le numéro de la question pour afficher la diffculté de la question.
        QString points;
        switch (this->questionEnCours->getDifficulte())
        {
        case Question::enumDifficulte::facile:
        {
            this->ui->questionLabel->setStyleSheet("QLabel { color : green; }");
            points = "+5";
            break;
        }
        case Question::enumDifficulte::moyenne:
        {
            this->ui->questionLabel->setStyleSheet("QLabel { color : orange; }");
            points = "+10";
            break;
        }
        case Question::enumDifficulte::difficile:
        {
            this->ui->questionLabel->setStyleSheet("QLabel { color : red; }");
            points = "+20";
            break;
        }
        }

        // Affiche le numéro de la question actuelle avec les points
        QString numQuestion = QString("Question %1 / %2 \n %3").arg(QString::number(questionCompteur), QString::number(this->clientPartie->getNombreQuestions()), points);
        this->ui->questionLabel->setText(numQuestion);

        // Met les choix possibles dans les boutons et les réintialise.
        this->ui->r0Bouton->setText(this->questionEnCours->getChoix()[0]);
        this->ui->r1Bouton->setText(this->questionEnCours->getChoix()[1]);
        this->ui->r2Bouton->setText(this->questionEnCours->getChoix()[2]);
        this->ui->r3Bouton->setText(this->questionEnCours->getChoix()[3]);

        this->ui->r0Bouton->setAutoExclusive(false);
        this->ui->r0Bouton->setChecked(false);
        this->ui->r1Bouton->setAutoExclusive(false);
        this->ui->r1Bouton->setChecked(false);
        this->ui->r2Bouton->setAutoExclusive(false);
        this->ui->r2Bouton->setChecked(false);
        this->ui->r3Bouton->setAutoExclusive(false);
        this->ui->r3Bouton->setChecked(false);

        this->ui->r0Bouton->setAutoExclusive(true);
        this->ui->r1Bouton->setAutoExclusive(true);
        this->ui->r2Bouton->setAutoExclusive(true);
        this->ui->r3Bouton->setAutoExclusive(true);

        this->reponse = 10;

        switch (questionType)
        {
        case Question::enumType::texte:
            this->ui->imageLabel->setText(this->questionEnCours->getSujet());
            this->ui->sujetLabel->setText("");
            break;
        case Question::enumType::image:
        {
            QString nomFichier = this->questionEnCours->getNomFichier();
            QPixmap pix("./data/img/" + nomFichier);

            this->ui->imageLabel->setPixmap(pix);

            this->ui->sujetLabel->setText(this->questionEnCours->getSujet());
            break;
        }
        case Question::enumType::audio:
        {
            this->sonPlayer = new QMediaPlayer;
            QString nomFichier = this->questionEnCours->getNomFichier();
            this->sonPlayer->setMedia(QUrl::fromLocalFile("./data/sound/" + nomFichier));
            this->sonPlayer->setVolume(50);
            this->sonPlayer->play();

            this->ui->imageLabel->setText(this->questionEnCours->getSujet());
            this->ui->sujetLabel->setText("");
            break;
        }
        case Question::enumType::chimere:
        {
            this->sonPlayer = new QMediaPlayer;
            QPair<QString, QString> nomFichiers = dynamic_cast<TypeChimere *>(this->questionEnCours)->getNomFichiers();
            this->sonPlayer->setMedia(QUrl::fromLocalFile("./data/chimere/" + nomFichiers.first));
            this->sonPlayer->setVolume(50);
            this->sonPlayer->play();

            QPixmap pix("./data/chimere/" + nomFichiers.second);

            this->ui->imageLabel->setPixmap(pix);
            this->ui->sujetLabel->setText(this->questionEnCours->getSujet());

            this->ui->sujetLabel->setText(this->questionEnCours->getSujet());
            break;
        }
        }

        break;
    }
    case 5:
        this->ui->stackedWidget->setCurrentIndex(1);
        this->questionCompteur = 0;
        break;
    }
}

/**
 * @brief Client::rejeterSocket
 */
void Client::rejeterSocket()
{
    if (this->socket)
    {
        this->socket->deleteLater();
        this->socket = nullptr;
        this->ui->statusbar->showMessage("Déconnecté.");

        // Permet de se reconnecter au serveur.
        if (this->ui->statusbar->isVisible())
            connecterSocket();
    }
}

/**
 * @brief Client::connecterSocket
 */
void Client::connecterSocket()
{
    this->socket = new QTcpSocket(this);

    connect(this->socket, SIGNAL(readyRead()), this, SLOT(lireSocket()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(rejeterSocket()));
    connect(this->socket, &QAbstractSocket::errorOccurred, this, &Client::afficherErreurSocket);

    connect(this->ui->creerPartieBouton, SIGNAL(clicked()), this, SLOT(onCreerPartieBoutonClicked()));
    connect(this->ui->rejoindrePartieBouton, SIGNAL(clicked()), this, SLOT(onRejoindrePartieBoutonClicked()));
    connect(this->ui->quitterBouton, SIGNAL(clicked()), this, SLOT(onQuitterPartieBoutonClicked()));
    connect(this->ui->demarrerBouton, SIGNAL(clicked()), this, SLOT(onDemarrerBoutonClicked()));

    connect(this->ui->quitterEnCoursBouton, SIGNAL(clicked()), this, SLOT(onQuitterPartieBoutonClicked()));
    connect(this->ui->r0Bouton, SIGNAL(clicked()), this, SLOT(onReponseBoutonClicked()));
    connect(this->ui->r1Bouton, SIGNAL(clicked()), this, SLOT(onReponseBoutonClicked()));
    connect(this->ui->r2Bouton, SIGNAL(clicked()), this, SLOT(onReponseBoutonClicked()));
    connect(this->ui->r3Bouton, SIGNAL(clicked()), this, SLOT(onReponseBoutonClicked()));

    connect(this->ui->listePartiesAccueil, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onPartieClicked(QListWidgetItem *)));
    connect(this->ui->listePartiesAccueil, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onPartieDoubleClicked(QListWidgetItem *)));

    // Tant que la dialog "connexion" ne renvoie pas "accept".
    while (true)
    {
        DialogConnexion dc;
        dc.setModal(true);

        if (dc.exec() == QDialog::Accepted)
            this->socket->connectToHost(dc.getIp(), 25565);
        else
            exit(EXIT_SUCCESS);

        if (this->socket->waitForConnected())
        {
            // Sauvegarde le nom d'utilisateur.
            this->nomUtilisateur = dc.getNomUtilisateur();

            // Envoie le nom d'utilisateur au serveur.
            envoyerSocket(0, "", this->nomUtilisateur);

            // Modifie la barre de statut.
            this->ui->statusbar->showMessage("Connecté au serveur, votre nom d'utilisateur est : " + this->nomUtilisateur);
            break;
        }
    }
}

/**
 * @brief Client::afficherErreur
 * @param erreurSocket
 */
void Client::afficherErreurSocket(QAbstractSocket::SocketError erreurSocket)
{
    switch (erreurSocket)
    {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::critical(this, "Erreur réseau", "L'hote distant a fermé la connexion.");
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::warning(this, "Erreur réseau", "Aucun hôte trouvé, vérifiez le nom d'hôte et le port utilisé.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, "Erreur réseau", "Connexion refusée.");
        break;
    default:
        QMessageBox::critical(this, "Erreur réseau", QString("L'erreur suivante s'est produite : %1.").arg(socket->errorString()));
        break;
    }
}

/**
 * Lorsque le joueur clique sur le bouton "Quitter partie", celui va envoyer un message au serveur de type "2" avec l'argument "0".
 * @brief Client::onQuitterPartieBoutonClicked
 */
void Client::onQuitterPartieBoutonClicked()
{
    // Envoie un message au serveur lui indiquant que l'on veut quitter la partie.
    envoyerSocket(2, "0", this->selectionPartie);

    if (this->sonPlayer->state() == QMediaPlayer::PlayingState)
        this->sonPlayer->stop();

    this->selectionPartie = QString();
    this->enPartie = false;
    this->questionCompteur = 0;

    // Retourne au menu principal.
    this->ui->stackedWidget->setCurrentIndex(0);
}

/**
 * @brief Client::onCreerPartieBoutonClicked
 */
void Client::onCreerPartieBoutonClicked()
{
    while (true)
    {
        // Affche la dialog permettant de créer une partie.
        DialogPartie dp;
        dp.setModal(true);

        if (dp.exec() == QDialog::Accepted)
        {
            QString titre = dp.getTitre();
            qint8 places = dp.getPlaces();
            qint8 nombreQuestions = dp.getNombreQuestions();

            // Créé un ID pour la partie et initialise la variable "selectionPartie" avec l'id.
            QString id = Partie::creerID();

            this->selectionPartie = id;

            Partie::struct_partie sp;
            sp.id = id;
            sp.titre = titre;
            sp.places = places;
            sp.nombreQuestions = nombreQuestions;

            envoyerSocket(1, "1", "", sp);

            break;
        }
        else
            break;
    }
}

/**
 * @brief Client::onPartieDoubleClicked
 * @param partie
 */
void Client::onPartieDoubleClicked(QListWidgetItem *partieItem)
{
    // Récupère l'id de la partie de l'item selectionné.

    // Cherche la partie ayant pour nom la partie selectionnée.
    foreach (Partie *partie, this->listeParties)
    {
        if (partie->getID() == this->selectionPartie)
        {
            // Si la partie n'est pas en cours et qu'il reste de la place dans la partie, alors le joueur peut s'y connecter.
            if (!partie->getStatut() && (partie->getPlaces() == 0 || partie->getNombreJoueurs() < partie->getPlaces()))
            {
                QVariant v = partieItem->data(Qt::UserRole);
                QString partieId = v.value<QString>();

                envoyerSocket(2, "1", partieId);
            }

            // Sort de la boucle une fois la demande traitée.
            return;
        }
    }
}

/**
 * @brief Client::onRejoindrePartieBoutonClicked
 */
void Client::onRejoindrePartieBoutonClicked()
{
    // Cherche la partie ayant pour nom la partie selectionnée.
    foreach (Partie *partie, this->listeParties)
    {
        if (partie->getID() == this->selectionPartie)
        {
            // S'il reste de la place dans la partie et qu'elle n'est pas en cours, alors le joueur peut se connecter.
            if (partie->getNombreJoueurs() < partie->getPlaces() && partie->getStatut() == Partie::enAttente)
                envoyerSocket(2, "1", this->selectionPartie);
            // Sort de la boucle une fois la demande traitée.
            return;
        }
    }
}

/**
 * @brief Client::onPartieClicked
 * @param partie
 */
void Client::onPartieClicked(QListWidgetItem *partie)
{
    // Récupère l'id de la partie de l'item selectionné.
    QVariant v = partie->data(Qt::UserRole);
    QString partieId = v.value<QString>();

    // Enregistre la partie selectionnée pour la rejoindre en appuyant sur le bouton.
    this->selectionPartie = partieId;

    // Nettoie la liste des joueurs.
    this->ui->listeJoueursAccueil->clear();

    // Affiche la liste des joueurs.
    this->ui->listeJoueursAccueil->setVisible(true);

    // Cherche la partie ayant pour nom la partie selectionnée.
    foreach (Partie *partie, this->listeParties)
    {
        // Affiche les joueurs de la partie selectionnée.
        if (partie->getID() == this->selectionPartie)
        {
            // Affiche le nombre de places restantes dans la partie selectionnée.
            if (partie->getPlaces() == 0)
                this->ui->placesJoueursAccueil->setText(partie->getTitre() + '\n' + QString::number(partie->getListeJoueurs().size()) + " / inf");
            else
                this->ui->placesJoueursAccueil->setText(partie->getTitre() + '\n' + QString::number(partie->getListeJoueurs().size()) + " / " + QString::number(partie->getPlaces()));

            //  Iterator sur la map des parties.
            QMapIterator<Joueur, qintptr> joueursIterator(partie->getListeJoueurs());
            // Tant qu'il y a des éléments, on avance.
            while (joueursIterator.hasNext())
            {
                joueursIterator.next();

                // Crée un item ayant ayant le nom du joueur, le centre dans la QListWidget et supprime les intéractions avec celui-ci.
                QListWidgetItem *item = new QListWidgetItem(joueursIterator.key().getNomUtilisateur());
                item->setTextAlignment(Qt::AlignCenter);
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);

                // Met le maître de la partie en rouge.
                if (joueursIterator.value() == partie->getHote())
                    item->setForeground(Qt::red);

                // Ajoute le joueur dans la QListWidget et l'affiche.
                this->ui->listeJoueursAccueil->addItem(item);
            }

            // Sort de la boucle une fois les joueurs affichés.
            return;
        }
    }
}

void Client::onDemarrerBoutonClicked()
{
    envoyerSocket(3, "");
}

void Client::onReponseBoutonClicked()
{
    // Change la couleur du bouton selectionné et envoie la réponse au serveur.
    if (sender() == this->ui->r0Bouton && this->reponse != 0)
    {
        envoyerSocket(4, "", QString::number(0));
        this->reponse = 0;
    }
    else if (sender() == this->ui->r1Bouton && this->reponse != 1)
    {
        envoyerSocket(4, "", QString::number(1));
        this->reponse = 1;
    }
    else if (sender() == this->ui->r2Bouton && this->reponse != 2)
    {
        envoyerSocket(4, "", QString::number(2));
        this->reponse = 2;
    }
    else if (sender() == this->ui->r3Bouton && this->reponse != 3)
    {
        envoyerSocket(4, "", QString::number(3));
        this->reponse = 3;
    }
}

void Client::majTimer()
{
    if (this->questionTempsActuel > 0)
    {
        this->questionTempsActuel -= 1;
        this->ui->timerLabel->setText(QString::number(this->questionTempsActuel));
    }
}
