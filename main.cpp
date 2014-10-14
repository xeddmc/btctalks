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
    QStringList url_list;
    url_list.append("https://bitcointalk.org/index.php?board=67.0;all");
    url_list.append("http://bitcointalk.org/index.php?board=159.0;all");
    BTCtalks sections(url_list);
    QList<BTCtalks_topic> data = sections.get_parsed_data();

    // Eventually parsing for the loot.
    int count(0);
    for(QList<BTCtalks_topic>::iterator it = data.begin(); it != data.end(); ++it) {
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

    qDebug() << "Got a total of #threads: " << count;
    qDebug() << sections.current_date << "\n";
    qDebug() << "Execution time (ms): " << timer.elapsed();
    return a.exec();
}
