#include <QtGui>

 #include "dialog.h"

Dialog::Dialog()
{
    create();
    connect(button, SIGNAL(clicked()), this, SLOT(accept()));
}

/*
void Dialog::accept()
{
    data1 =app->text();
}
*/

QString Dialog::getAppData()
{
    return app->text();
}

QString Dialog::getUNameData()
{
    return uName->text();
}

QString Dialog::getScopeData()
{
    return scope->text();
}


void Dialog::create()
{
//    gridGroupBox = new QGroupBox(tr("xx"));
    QGridLayout *layout = new QGridLayout;
    appLbl = new QLabel(tr("App Name"));
    uNameLbl= new QLabel(tr("User Name"));
    scLbl = new QLabel(tr("Scope (1 or 0)"));
    app = new QLineEdit;
    uName = new QLineEdit;
    scope = new QLineEdit;
    button = new QPushButton(tr("OK"));
    layout->addWidget(appLbl,0,0);
    layout->addWidget(uNameLbl,1,0);
    layout->addWidget(scLbl,2,0);
    layout->addWidget(app,0,1);
    layout->addWidget(uName,1,1);
    layout->addWidget(scope,2,1);

    layout->addWidget(button,4,0);
    setLayout(layout);
}

