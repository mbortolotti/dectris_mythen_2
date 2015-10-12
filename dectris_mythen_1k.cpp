#include "dectris_mythen_1k.h"

#include <qthread.h>

#include <iostream>
#include <sstream>
#include <time.h>


// default DCS / detector options
std::string host = "192.168.0.90";
int port = 1031;
std::string radiation = "Mo";

const int MAXMODULES = 1;       // maximum number of supported modules
const int NMAXCHANNELS = 1280;  // maximum channels per module

const int TCP_TIMEOUT_MS = 250;
const int TCP_DELAY_MS = 100;

dectris_mythen_1k::dectris_mythen_1k()
{

}

dectris_mythen_1k::~dectris_mythen_1k()
{

}

bool dectris_mythen_1k::init()
{
    std::cout << "Initializing Dectris Mythen detector at address " << host << "..." << std::endl;

    _pSocket = new QTcpSocket();
    _pSocket->connectToHost(QString::fromStdString(host), port);

    if (!(_pSocket->isValid())) {
        std::cout << "Error: unable to contact Mythen server!" << std::endl;
        return false;
    }

    QByteArray version = sendCommand("-get version", 7);
    std::cout << "Dectris server version " << QString(version).toStdString() << std::endl;

    sendCommand("-reset", sizeof(int));
    sendCommand(("-settings " + radiation).c_str(), sizeof(int));

    std::cout << "Dectris Mythen initialization succesful!" << std::endl;

    return true;
}

void dectris_mythen_1k::setEnergy(double energy)
{
    std::stringstream ss_energy;
    ss_energy << energy;
    sendCommand(("-energy " + ss_energy.str()).c_str(), sizeof(int));
}

void dectris_mythen_1k::setThreshold(double threshold)
{
    std::stringstream ss_threshold;
    ss_threshold << threshold;
    sendCommand(("-kthresh " + ss_threshold.str()).c_str(), sizeof(int));
}

void dectris_mythen_1k::startCounts(long long time_msec)
{
    std::stringstream ss_time_100nsec;
    ss_time_100nsec << (time_msec * 10000);
    sendCommand(("-time " + ss_time_100nsec.str()).c_str(), sizeof(int));
    sendCommand("-start", sizeof(int));
}

void dectris_mythen_1k::stopCounts()
{
    sendCommand("-stop", sizeof(int));
}

std::vector<int> dectris_mythen_1k::readCounts()
{
    std::vector<int> counts;
    QByteArray data = sendCommand("-readout", sizeof(int) * NMAXCHANNELS);
    if (data.length() == sizeof(int)) {
        std::cout << "error code: " << data.toInt() << std::endl;
    }
    else
    {
        int nchannels = data.size() / sizeof(int);
        counts.resize(nchannels, 0);
        QDataStream counts_ds(data);
        counts_ds.setByteOrder(QDataStream::LittleEndian);
        for (int nc = 0; nc < nchannels; nc++)
            counts_ds >> counts[nc];
    }
    return counts;
}

std::vector<int> dectris_mythen_1k::getCounts(long long time_msec)
{
    startCounts(time_msec);
    QThread::msleep(time_msec);
    return readCounts();
}

QByteArray dectris_mythen_1k::sendCommand(const char* command, int rec_length)
{
    QByteArray data;
    _pSocket->write(command);
    while((data.length() < rec_length) && (data.length() != 4))
    {
        _pSocket->waitForReadyRead(TCP_DELAY_MS);
        data.append(_pSocket->readAll());
    }
    return data;
}
