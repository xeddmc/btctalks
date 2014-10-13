#ifndef BTCTALKS_H
#define BTCTALKS_H
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <random>

struct BTCtalks_topic {
    QString thread_name;
    QString date;
    QString link;
};

class BTCtalks {

public:
    BTCtalks(QString url);
    QString current_date;
    QString base_url;
    QString get_data();
    QList<BTCtalks_topic> data_parser(QString data);
};

#endif // BTCTALKS_H
