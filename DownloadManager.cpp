#include "DownloadManager.h"

class QSslError;
QT_USE_NAMESPACE


DownloadManager::DownloadManager() {
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void DownloadManager::add_url(QString url){
    urls.push_back(url);
}

void DownloadManager::doDownload(const QUrl &url) {
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", rand_user_agent());
    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));

    currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}



bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void DownloadManager::perform() {
    foreach (QString url, urls)
        doDownload(url);
    qel.exec();
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_OPENSSL
    foreach (const QSslError &error, sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#endif
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    }
    // Handling redirection.
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 301) {
        doDownload(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
    } else {
        if(save_file){
            QString filename = saveFileName(url);
            if (saveToDisk(filename, reply))
                printf("Download of %s succeeded (saved to %s).\n",
                       url.toEncoded().constData(), qPrintable(filename));
        } else {
            printf("Download of %s succeeded.\n", url.toEncoded().constData());
        }
        data_all.push_back(reply->readAll());
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if (currentDownloads.isEmpty()){
        // all downloads finished
        qel.exit();
    }
}

QByteArray DownloadManager::rand_user_agent(){
    std::list<QByteArray> user_agent_list;
    user_agent_list.push_back("Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2049.0 Safari/537.36");
    user_agent_list.push_back("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.47 Safari/537.36");
    user_agent_list.push_back("Mozilla/5.0 (iPad; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B334b Safari/531.21.10");
    user_agent_list.push_back("Mozilla/5.0 (Windows NT 5.1; rv:31.0) Gecko/20100101 Firefox/31.0");
    user_agent_list.push_back("Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; de-at) AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1");

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,331);
    for(int i(0); i<distribution(generator); i++){
        user_agent_list.sort();
        user_agent_list.reverse();
    }

    return user_agent_list.front();
}
