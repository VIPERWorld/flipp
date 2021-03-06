#ifndef CONNECTIONUNIT_H
#define CONNECTIONUNIT_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QByteArray>
#include <QDebug>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>

#ifdef SERIAL_WINDOWS
enum baudRate{B4800,B9600,B19200,B57600,B115200};
#endif
enum connectionType{TCP,UDP,SERIAL};
enum connectionState{INACTIVE,CONNECTING,ACTIVE};

struct connectionInfo
{
    QString connName;
    connectionType connType;
    QString addr_port;
    quint32 port_baud;
    connectionState connState;
    bool valid;
};

class ConnectionUnit : public QObject
{
    Q_OBJECT
public:
    ConnectionUnit();
    QString getName();
    connectionType getType();
    QString getAddress_Port();
    quint32 getPort_Baud();
    connectionState getState();
    bool isValid();


    void setType(connectionType);
    void setAddress_Port(QString);
    void setPort_Baud(quint32);
    void setID(quint16);
    void setValid();
    void setInvalid();

signals:
    void dataIn(QByteArray);
    void serverDisconnect();
    void clientConnected();
    void connectionStatus(connectionState);
    void overrideBaud(QString);
    void connectionError(QString);
    void validChange(bool);

public slots:
    void setName(QString);
    void dataConnect();
    void dataDisconnect();
    void dataOut(QByteArray);
    void tcpConnected();

private:
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QSerialPort *serialPort;
    connectionInfo connConfig;
    QString serialPrefix;

private slots:
    void dataAvailable();
    void disconnected();
    void connectionChanged();
    void errorHandler(QAbstractSocket::SocketError);

};

#endif // ConnectionUnit_H
