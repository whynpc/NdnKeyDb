#include <QApplication>
#include "visualizer.h"

#include "keydb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <ccn/keystore.h>


using namespace std;


int main(int argc, char *argv[])
{
/*    KeyDB *kdb = new KeyDB();
    kdb->connectDB();
    string k;
    kdb->importPriKey("mykey.pem", k);
    cout << k << endl;
    kdb->insertPriKey("myKey", k);
    string retKey = kdb->getPriKey("myKey");
    cout << "retrieved key: " << retKey << endl;
    EVP_PKEY* evpK = kdb->str2priKey(retKey);
    cout << RSA_check_key(EVP_PKEY_get1_RSA(evpK));

    kdb->disconnectDB();*/

//     Q_INIT_RESOURCE(application);



     QApplication app(argc, argv);
     app.setOrganizationName("UCLA NDN");
     app.setApplicationName("Key Visualizer");
     Visualizer v;
     v.show();
     return app.exec();
}
