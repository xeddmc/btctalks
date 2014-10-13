#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QTimer>
#include <QUrl>
#include <QByteArray>

class QSslError;

QT_USE_NAMESPACE

class DownloadManager: public QObject {
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;
    QList<QString> urls;

public:
    DownloadManager();
    QList<QString> data_all;
    void add_url(QString url);
    void doDownload(const QUrl &url);
    void perform();
    bool saveToDisk(const QString &filename, QIODevice *data);
    bool save_file = false;
    //bool is_done = false;
    QString saveFileName(const QUrl &url);
    QByteArray rand_user_agent();
    QEventLoop qel;

public slots:
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
};

#endif // DOWNLOADMANAGER_H
