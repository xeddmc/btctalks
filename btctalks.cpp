#include "btctalks.h"
#include "DownloadManager.h"

// Single URL constructor.
BTCtalks::BTCtalks(QString url) {
    base_url = url;
}

// Constructor for multiple URLs.
BTCtalks::BTCtalks(QStringList URLs){
    foreach(QString url, URLs)
        urls.push_back(url);
}

// Downloading the data from the URL(s) and parsing them.
QList<BTCtalks_topic> BTCtalks::get_parsed_data() {
    if(urls.isEmpty()) {
        DownloadManager mgr;
        mgr.add_url(base_url);
        mgr.perform();
        return data_parser(mgr.data_all.back());
    } else {
        DownloadManager mgr;
        foreach(QString url, urls)
            mgr.add_url(url);
        mgr.perform();
        QList<BTCtalks_topic> ret_var;
        for(QList<QString>::iterator it = mgr.data_all.begin(); it != mgr.data_all.end(); ++it) {
            QList<BTCtalks_topic> tmp = data_parser(*it);
            for(QList<BTCtalks_topic>::iterator it_2 = tmp.begin(); it_2 != tmp.end(); ++it_2)
                ret_var.push_back(*it_2);
        }
        return ret_var;
    }
}

// Parsing the data received from the URL.
QList<BTCtalks_topic> BTCtalks::data_parser(QString data){
    // Generating the list to return.
    QList<BTCtalks_topic> topics;
    // Replace for parsing quote.
    data = data.replace("\"", "^");

    // A fix for the date parsing.
    QString chars = "QWERTYUIOPASDFGHJKLZXCVBNM<qwertyuiopasdfghjklzxcvbnm";

    // Setting the index pointers for thread name, link and date parsing.
    int index_name = data.indexOf("<span id=^") + QString("<span id=^msg_3133700^><a href=^https://bitcointalk.org/index.php?topic=313370.0^>").length();
    int index_date = data.indexOf("<span class=^smalltext^>") + QString("<span class=^smalltext^>").length();
    int index_link = data.indexOf("<span id=^") + QString("<span id=^msg_3133700^><a href=^").length();

    // Getting the current date.
    QString tmp;
    for(;;){
        if(!current_date.isEmpty())
            break;
        QString check_var, closer("</span>");
        for(int fill(0); fill < closer.length(); ++fill)
            check_var += data.at(index_date + fill);
        if(check_var == closer) {
            // Got current date, synchronizing index with thread name one for future parsing (skipping junk).
            current_date.append(tmp);
         // qDebug() << "Got current date:" << current_date;
            index_date = data.indexOf("<span class=^smalltext^>", index_date) + QString("<span class=^smalltext^>").length();
            index_date = data.indexOf("<span class=^smalltext^>", index_date) + QString("<span class=^smalltext^>").length();
            break;
        } else {
            tmp += data.at(index_date);
            ++index_date;
        }
    }


    // Patching up the date index.
    while(true){
        bool breaking(false);
        for(QString::iterator it = chars.begin(); it != chars.end(); ++it){
            if(*it == data.at(index_date)) {
                breaking = true;
                break;
            }
        }
        if(breaking) break;
        ++index_date;
    }

    while(data.indexOf("<span id=^", index_name) != -1){
        BTCtalks_topic parse;
        for(;;){
            // Parsing a thread name from data.
            QString check_var, closer("</a>");
            for(int fill(0); fill < closer.length(); ++fill)
                check_var += data.at(index_name + fill);
            if(check_var == closer){
                index_name = data.indexOf("<span id=^", index_name) + QString("<span id=^msg_3133700^><a href=^https://bitcointalk.org/index.php?topic=313370.0^>").length();
             // qDebug() << "Got a name: " << parse.thread_name;
                break;
            } else {
                parse.thread_name += data.at(index_name);
                ++index_name;
            }
        }
        for(;;){
            // Parsing its date.
            QString check_var, closer("<br />");
            for(int fill(0); fill < closer.length(); ++fill)
                check_var += data.at(index_date + fill);
            if(check_var == closer){
                index_date = data.indexOf("<span class=^smalltext^>", index_date) + QString("<span class=^smalltext^>").length();
                // Patching up the date index.
                while(true){
                    bool breaking(false);
                    for(QString::iterator it = chars.begin(); it != chars.end(); ++it){
                        if(*it == data.at(index_date)) {
                            breaking = true;
                            break;
                        }
                    }
                    if(breaking) break;
                    ++index_date;
                }
             // qDebug() << "Got its date: " << parse.date;
                break;
            } else {
                parse.date += data.at(index_date);
                ++index_date;
            }
        }
        for(;;){
            // Parsing its link.
            if(data.at(index_link) == '^'){
                index_link = data.indexOf("<span id=^", index_link) + QString("<span id=^msg_3133700^><a href=^").length();
             // qDebug() << "Got its link: " << parse.link;
                break;
            } else {
                parse.link += data.at(index_link);
                ++index_link;
            }
        }
        topics.push_back(parse);
    }

    // Fixing <b>Today</b> with current_date.
    for(QList<BTCtalks_topic>::iterator it = topics.begin(); it != topics.end(); ++it) {
        if( (it->date).contains("<b>Today</b>") ) {
            QString today = current_date.left(current_date.indexOf(" 2014"));
            (it->date).replace("<b>Today</b>", today);
        }
    }

    return topics;
}
