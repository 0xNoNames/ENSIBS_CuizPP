#ifndef EXCEPTIONCLIENT_H
#define EXCEPTIONCLIENT_H

#include <QDialog>
#include <QMessageBox>
#include <exception>

using namespace std;
class ExceptionClient : public exception, QDialog
{

public:
    typedef enum
    {
        chaineTropLongueErreur,
        chaineTropCourteErreur,
        caractereNonAsciiErreur,
        caractereNonNumeriqueErreur,
        nbPartiePlacesErreur,
        problemeJson
    } erreurEnum;

    ExceptionClient(erreurEnum erreur, QWidget *parent = nullptr) throw();
    ExceptionClient(const ExceptionClient &ec) throw();
    virtual ~ExceptionClient() throw();

    virtual const char *what() const throw();
    void afficherErreur();

private:
    erreurEnum erreur;
};

#endif // EXCEPTIONCLIENT_H
