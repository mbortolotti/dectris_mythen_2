#include <QCoreApplication>

#include "dectris_mythen_1k.h"

#include <qthread.h>

#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int count_time_msec = 10000;
    dectris_mythen_1k mythen;
    mythen.init();
    std::vector<int> counts = mythen.getCounts(count_time_msec);
    for (int nc = 0; nc < counts.size(); nc++)
        std::cout << "[" << nc << "]=" << counts[nc] << "\t";

    return a.exec();
}
