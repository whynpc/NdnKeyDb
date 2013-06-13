#include <QtGui>
#include <Qt>
#include <vector>
#include "visualizer.h"
#include "dialog.h"

Visualizer::Visualizer()
{
    kdb = new KeyDB();

    displayKey = new QPlainTextEdit;
    keyInfo = new QLabel;
    keyInfoTitle = new QLabel;
    keyMeta = new QLabel;
    keyMetaTitle = new QLabel;
    keyList = new QComboBox();
    addLocator = new QPushButton(tr("Add Key Locator"));
    addMeta = new QPushButton(tr("Add Metadata"));
    showPriv = new QPushButton(tr("Show Private Key"));

    updateNameList();

    connect(keyList, SIGNAL(activated(QString)), this, SLOT(comboEvent(QString)));
    connect(keyList,SIGNAL(currentIndexChanged(QString)),this, SLOT(comboEvent(QString)));
    connect(addLocator, SIGNAL(clicked()), this, SLOT(addLocatorEvent()));
    connect(addMeta, SIGNAL(clicked()), this, SLOT(addMetaEvent()));
    connect(showPriv, SIGNAL(clicked()), this, SLOT(privateKey()));


    //   horizontalGroupBox = new QGroupBox();
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(keyList, 0, 0, 1, 2);
    layout->addWidget(addLocator, 0, 2, 1, 1);
    layout->addWidget(addMeta, 0, 3, 1, 1);
    layout->addWidget(showPriv, 0, 4, 1, 1);
    layout->addWidget(displayKey, 1, 0, 4, 6);
    layout->addWidget(keyInfoTitle, 5, 1, 1, 1);
    layout->addWidget(keyMetaTitle, 5, 4, 1, 1);
    layout->addWidget(keyInfo, 6, 0, 3, 3);
    layout->addWidget(keyMeta, 6, 4, 1, 3);

    QWidget *widget = new QWidget();
    setCentralWidget(widget);
    centralWidget()->setLayout(layout);

    keyInfoTitle->setText(tr("-== Key Locator Chain ==-"));
    keyMetaTitle->setText(tr("Meta-Information of the Key"));
    keyInfo->setAlignment(Qt::AlignTop);
    keyMeta->setAlignment(Qt::AlignTop);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

string Visualizer::getLocatorChain(const string &keyName, int tab){
    kdb->connectDB();
    string loc = kdb->getLocator(keyName);
    kdb->disconnectDB();
    if (loc == "")
        return loc;
    string spacing = "";
    for (int i = 0; i <= tab; i++)
        spacing += "        ";
    return spacing + loc + "\n" + getLocatorChain(loc, tab+1);
}

void Visualizer::updateNameList(){
    keyList->clear();
    kdb->connectDB();
    vector<string> keyNameList;
    kdb->getAllKeyNames(keyNameList);
    for (int i = 0; i < keyNameList.size(); i++)
        keyList->insertItem(i, tr(keyNameList[i].c_str()));
    kdb->disconnectDB();
}

string Visualizer::comboEvent(QString item){
    displayKey->clear();
    kdb->connectDB();
    string pubKey = kdb->getKey(item.toStdString());
    vector<string> metaData;
    kdb->getMetaData(item.toStdString(), metaData);
    kdb->disconnectDB();
    string locator = getLocatorChain(item.toStdString(), 1);
    displayKey->insertPlainText(pubKey.c_str());
    keyInfo->setText(tr(locator.c_str()));
    string mData;
    for (int i = 0; i < metaData.size(); i++)
        mData += metaData[i] + "\n";
    keyMeta->setText(tr((mData.c_str())));
    return item.toStdString();
}

void Visualizer::addLocatorEvent(){
    QString keyName = keyList->currentText();
    bool ok;
    QString locator = QInputDialog::getText(this, tr("Key Locator"),
                                            tr("Please specify a key locator"), QLineEdit::Normal,
                                            QDir::home().dirName(), &ok);
    kdb->connectDB();
    if (ok && !keyName.isEmpty()){
        kdb->updateLocator(keyName.toStdString(), locator.toStdString());
    }
    kdb->disconnectDB();
    comboEvent(keyName);
}

void Visualizer:: addMetaEvent(){
    string keyName = keyList->currentText().toStdString();

    Dialog d;
    if (d.exec() == Dialog::Accepted){
        string app = d.getAppData().toStdString();
        string uName = d.getUNameData().toStdString();
        int scope = d.getScopeData().toInt();

        kdb->connectDB();
        kdb->addUser(app, (KeyDB::scope)scope, keyName, uName);
        kdb->disconnectDB();
    }
}

void Visualizer::privateKey(){
    kdb->connectDB();
    string name = kdb->getPriKeyName();
    string key = kdb->getPriKey();
    kdb->disconnectDB();

    keyInfo->clear();
    keyMeta->clear();
    displayKey->clear();
    displayKey->insertPlainText(tr(("Key Name: " + name + "\n").c_str()));
    displayKey->insertPlainText(tr(key.c_str()));
}

void Visualizer::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}


void Visualizer::import()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        kdb->connectDB();
        string strKey;
        kdb->importKey(fileName.toStdString(), strKey);
        bool ok;
        QString keyName = QInputDialog::getText(this, tr("Key Name"),
                                             tr("Please enter a name for the key"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !keyName.isEmpty()){
            kdb->insertKey(keyName.toStdString(), strKey);
            string txt = keyName.toStdString() + "\n " + strKey;
            displayKey->insertPlainText(tr(txt.c_str()));
        }
        kdb->disconnectDB();
    }
    updateNameList();
}

void Visualizer::importPri(){
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        kdb->connectDB();
        string strKey;
        kdb->importKey(fileName.toStdString(), strKey);
        bool ok;
        QString keyName = QInputDialog::getText(this, tr("Key Name"),
                                             tr("Please enter a name for the key"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !keyName.isEmpty()){
            kdb->insertPriKey(keyName.toStdString(), strKey);
            string txt = keyName.toStdString() + "\n " + strKey;
            displayKey->insertPlainText(tr(txt.c_str()));
        }
        kdb->disconnectDB();
    }
    updateNameList();
}

void Visualizer::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}


void Visualizer::createActions()
{
    importKey = new QAction(tr("Import Public"), this);
    importKey->setStatusTip(tr("Import a public key from file (.pub)"));
    connect(importKey, SIGNAL(triggered()), this, SLOT(import()));

    importPriKey = new QAction(tr("Import Private"), this);
    importPriKey->setStatusTip(tr("Import a private key from file (.pem)"));
    connect(importPriKey, SIGNAL(triggered()), this, SLOT(importPri()));
}

void Visualizer::createMenus()
{
    keyMenu = menuBar()->addMenu(tr("&Key"));
    keyMenu->addAction(importKey);
    keyMenu->addAction(importPriKey);
    keyMenu->addSeparator();
}

void Visualizer::createToolBars()
{
/*    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);*/
}

void Visualizer::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void Visualizer::readSettings()
{
    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void Visualizer::writeSettings()
{
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void Visualizer::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool Visualizer::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void Visualizer::setCurrentFile(const QString &fileName)
{
    curFile = fileName;

    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString Visualizer::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
