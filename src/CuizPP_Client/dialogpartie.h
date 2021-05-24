#ifndef DIALOGPARTIE_H
#define DIALOGPARTIE_H

#include <QDialog>
#include "exceptionclient.h"

namespace Ui
{
    class DialogPartie;
}

class DialogPartie : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPartie(QWidget *parent = nullptr);
    ~DialogPartie();

    QString getTitre() const;
    qint8 getPlaces() const;
    qint8 getNombreQuestions() const;

private slots:
    void on_okBouton_clicked();

private:
    Ui::DialogPartie *ui;
};

#endif // DIALOGPARTIE_H
