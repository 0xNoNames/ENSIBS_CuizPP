#include "exceptionclient.h"

ExceptionClient::ExceptionClient(erreurEnum erreur, QWidget *parent) throw() : QDialog(parent), erreur(erreur)
{
}

ExceptionClient::ExceptionClient(const ExceptionClient &ec) throw() : QDialog(nullptr), erreur(ec.erreur)
{
}

ExceptionClient::~ExceptionClient() throw()
{
}

const char *ExceptionClient::what() const throw()
{
    return (char *)this->erreur;
}

void ExceptionClient::afficherErreur()
{
    switch (this->erreur)
    {
    case ExceptionClient::caractereNonAsciiErreur:
        QMessageBox::warning(this, "Erreur entrée", "La chaîne de caractères contient des caractères non ascii.");
        break;
    case ExceptionClient::chaineTropCourteErreur:
        QMessageBox::warning(this, "Erreur entrée", "La chaîne de caractères est trop courte. (min 3)");
        break;
    case ExceptionClient::chaineTropLongueErreur:
        QMessageBox::warning(this, "Erreur entrée", "La chaîne de caractères est trop longue. (max 12)");
        break;
    case ExceptionClient::caractereNonNumeriqueErreur:
        QMessageBox::warning(this, "Erreur entrée", "La chaîne de caractères contient des caractères non numériques.");
        break;
    case ExceptionClient::nbPartiePlacesErreur:
        QMessageBox::warning(this, "Erreur nombre de places", "Le nombre de partie ne doit pas dépasser 99.");
        break;
    case ExceptionClient::problemeJson:
        QMessageBox::critical(this, "Erreur fichier json", "Le fichier JSON est corrompu ou introuvable, veuillez relancer l'application après vérification.");
        break;
    default:
        QMessageBox::critical(this, "Erreur entrée", "Une erreur s'est produite.");
        break;
    }
}
