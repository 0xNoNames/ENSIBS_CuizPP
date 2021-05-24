#ifndef SERVEURGUI_H
#define SERVEURGUI_H

#include <QMainWindow>
#include "serveur.h"

namespace Ui
{
    class ServeurGUI;
}

class ServeurGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServeurGUI(QWidget *parent = nullptr);
    ~ServeurGUI();

private slots:
    void on_dabouton_clicked();
    void afficherInfo(const QString &info);

private:
    Ui::ServeurGUI *ui;
    Serveur *serveur;
};

#endif // SERVEURGUI_H
