#ifndef DECTRIS_MYTHEN_1K_H
#define DECTRIS_MYTHEN_1K_H

#include <QTcpSocket>

#include <vector>
#include <string>

class dectris_mythen_1k
{

public:

    dectris_mythen_1k();
    ~dectris_mythen_1k();

    bool init();
    void setEnergy(double energy);
    void setThreshold(double threshold);

    void startCounts(long long time_msec);
    void stopCounts();
    std::vector<int> readCounts();
    std::vector<int> getCounts(long long time_msec);

private:

    QByteArray sendCommand(const char *command, int rec_length);

    QTcpSocket * _pSocket;

};

#endif // DECTRIS_MYTHEN_1K_H
