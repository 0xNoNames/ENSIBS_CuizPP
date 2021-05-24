#ifndef DIALOGCONNEXION_H
#define DIALOGCONNEXION_H

#include <QDialog>
#include <QMessageBox>
#include "exceptionclient.h"

namespace Ui
{
    class DialogConnexion;
}

class DialogConnexion : public QDialog
{
    Q_OBJECT
public:
    explicit DialogConnexion(QWidget *parent = nullptr);
    ~DialogConnexion();

    QString getIp() const;
    QString getNomUtilisateur() const;

private slots:
    void on_connecterBouton_clicked();

private:
    Ui::DialogConnexion *ui;
};

#endif // DIALOGCONNEXION_H
