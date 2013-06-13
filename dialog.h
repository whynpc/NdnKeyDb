#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QGroupBox;

class Dialog : public QDialog
{
    Q_OBJECT
public:
    Dialog();
    QString getAppData();
    QString getUNameData();
    QString getScopeData();
//private slots:
//    void accept();
private:
    void create();
    QGroupBox *gridGroupBox;
    QLabel *appLbl;
    QLabel *uNameLbl;
    QLabel *scLbl;
    QLineEdit *app;
    QLineEdit *uName;
    QLineEdit *scope;
    QPushButton *button;

};

#endif // DIALOG_H
