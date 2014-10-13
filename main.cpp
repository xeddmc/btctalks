#include <QCoreApplication>
#include <btctalks.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTime timer;
    timer.start();

    // Vessels for the loot.
    QList<BTCtalks_topic> ICOs, IPOs;
    // Initializing.
    BTCtalks alt_crypt("https://bitcointalk.org/index.php?board=67.0;all");
    BTCtalks announcements("http://bitcointalk.org/index.php?board=159.0;all");
    // Fetching the data from the URL(s).
    QString data = alt_crypt.get_data();
    QString data_ann = announcements.get_data();
    // Parsing the HTML data.
    QList<BTCtalks_topic> topics_alt_crypt = alt_crypt.data_parser(data);
    QList<BTCtalks_topic> topics_annc =  announcements.data_parser(data_ann);

    // Eventually parsing for the loot.
    int count(0);
    for(QList<BTCtalks_topic>::iterator it = topics_alt_crypt.begin(); it != topics_alt_crypt.end(); ++it) {
        if( it->thread_name.contains("IPO", Qt::CaseInsensitive) ){
            IPOs.push_back(*it);
            qDebug() << "-------------\n [*] IPO found.";
            qDebug() << it->thread_name << it->date;
            qDebug() << it->link << "\n------------------\n";
            ++count;
        } else if( it->thread_name.contains("ICO",  Qt::CaseInsensitive) ) {
            ICOs.push_back(*it);
            qDebug() << "-------------\n [*] ICO found.";
            qDebug() << it->thread_name << it->date;
            qDebug() << it->link << "\n------------------\n";
            ++count;
        }
    }

    for(QList<BTCtalks_topic>::iterator it = topics_annc.begin(); it != topics_annc.end(); ++it) {
        if( it->thread_name.contains("IPO", Qt::CaseInsensitive) ){
            IPOs.push_back(*it);
            qDebug() << "-------------\n [*] IPO found.";
            qDebug() << it->thread_name << it->date;
            qDebug() << it->link << "\n------------------\n";
            ++count;
        } else if( it->thread_name.contains("ICO", Qt::CaseInsensitive) ) {
            ICOs.push_back(*it);
            qDebug() << "-------------\n [*] ICO found.";
            qDebug() << it->thread_name << it->date;
            qDebug() << it->link << "\n------------------\n";
            ++count;
        }
    }

    qDebug() << "Got a total of #threads: " << count;
    qDebug() << alt_crypt.current_date << "\n";
    qDebug() << "Execution time (ms): " << timer.elapsed();
    return a.exec();
}
