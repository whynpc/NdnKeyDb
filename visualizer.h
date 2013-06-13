#ifndef VISUALIZER_H
#define VISUALIZER_H


#include <QMainWindow>
#include "keydb.h"

class QAction;
class QMenu;
class QLabel;
class QComboBox;
class QGroupBox;
class QPushButton;
class QPlainTextEdit;

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer();


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void import();
    void importPri();
    void about();

    string comboEvent(QString item);
    void addLocatorEvent();
    void addMetaEvent();
    void privateKey();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    string getLocatorChain(const string &keyName, int tab);

    void updateNameList();

    QPlainTextEdit *displayKey;
    QLabel* keyInfo;
    QLabel* keyInfoTitle;
    QLabel* keyMeta;
    QLabel* keyMetaTitle;
    QString curFile;

    QMenu *keyMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *importKey;
    QAction *importPriKey;
    QAction *showKey;
    QComboBox* keyList;
    QGroupBox *horizontalGroupBox;
    QPushButton* addLocator;
    QPushButton* addMeta;
    QPushButton* showPriv;


    KeyDB* kdb;
};


#endif // VISUALIZER_H
