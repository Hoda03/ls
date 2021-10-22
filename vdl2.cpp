#include "vdl2.h"
#include "ui_vdl2.h"
#include <QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QComboBox>
#include <QList>
#include <QSerialPortInfo>
#include <QStringBuilder>

int ctr =0;
uint8_t counterm = 0;
QSerialPort *serialVDL;
#define SIZE_SYNCHRO_SL     4
const uint8_t synchrovdl2[SIZE_SYNCHRO_SL] = {0xff,0x00,0xf0,0xca};
int counterVDL =0;


vdl2::vdl2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::vdl2)
{
    ui->setupUi(this);
    this->setWindowTitle("VDL Mode2");
    this->setStyleSheet("QMainWindow { background-color : #ffffff}");
    ui->logo->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ui->logo->setStyleSheet("QLabel { background-color : white; color : #0078d7; }");
    ui-> logo->setLineWidth(3);
    ui-> logo->setMidLineWidth(3);

    sockUdp = new QUdpSocket(this);
    sockUdp->bind(QHostAddress::LocalHost,8888);

    serialVDL = new QSerialPort(this);
    serialVDL->setPortName("COM7");
    if (serialVDL->open(QIODevice::ReadWrite))
    {

        qDebug() << "Connected";
    }
    else
    {
        qDebug() << "Error";

    }


    serialVDLfreq = new QSerialPort(this);
    serialVDLfreq->setPortName("COM5");
    if (serialVDLfreq->open(QIODevice::ReadWrite))
    {

        qDebug() << "Connected";
    }
    else
    {
        qDebug() << "Error";

    }



    connect(serialVDL,&QSerialPort::readyRead,[&](){

                qDebug() << "houda";

    while(serialVDL->waitForReadyRead(100)){

        qDebug() << "New data available: " << serialVDL->bytesAvailable();
        QByteArray datas = serialVDL->readAll();

        //Affichage des trames en héxadécimal
        qDebug() << datas;

        if(datas.length() == 17 ){

            ui-> receivehexa->appendPlainText("");

        }

        ui-> receivehexa->moveCursor (QTextCursor::End);
        ui-> receivehexa->insertPlainText (datas.toHex());
        ui-> receivehexa->moveCursor (QTextCursor::End);


        /*qDebug() << "New data available: " << serialVDL->bytesAvailable();
        QByteArray datas = serialVDL->readAll();
        qDebug() << datas;
        //Affichage des trames en héxadécimal
        QByteArray ba_as_hex_string = datas.toHex();
        serialVDL->waitForReadyRead(3000);
        int longueur = ui->donnees_2->value();*/

        if(datas.length() == 17 && (datas[5] == 80) && (datas[8] >= 18)){

            int PARAM_REQ = ui-> frequence_2 -> value();
            ui-> receive-> appendPlainText(QString("PARAM_CONF = %1").arg(PARAM_REQ));
            ui-> receivehexa->appendPlainText("");

          //  ui-> receivehexa->appendPlainText(ba_as_hex_string);

        }
        if(datas.length() == 12 && ((datas[8] == 47) || (datas[8] == 44) || (datas[8] == 40)) && ( datas[9] == 1)){
            ui-> receivehexa->appendPlainText("");

            int PWR_SET_REQ = ui -> puissance -> value();
             ui->receive->appendPlainText(QString("PWR_SET_CONF = %1").arg(PWR_SET_REQ));
             ui-> receivehexa->appendPlainText("");

            // ui-> receivehexa->appendPlainText(ba_as_hex_string);

        }
        if(datas.length() == 12 && datas[9] == 1 && datas[11] == 119){
            ui-> receivehexa->appendPlainText("");

            ui->receive->appendPlainText(QString("MODE_SET_CONF_TX"));
            QByteArray ba_as_hex_string = datas.toHex();
            ui-> receivehexa->appendPlainText("");

           // ui->  receivehexa->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

           }
        if(datas.length() == 14 && datas[5] == 86){
            ui-> receivehexa->appendPlainText("");

            ui->receive->appendPlainText(QString("UNIT DATA CONF"));
            QByteArray ba_as_hex_string = datas.toHex();
            ui-> receivehexa->appendPlainText("");

            //ui->  receivehexa->appendPlainText(ba_as_hex_string);
        }
        if(datas.length() == 12 && datas[5] == 83){
            ui-> receivehexa->appendPlainText("");

            ui->receive->appendPlainText(QString("VDR_ERROR_IND"));
            QByteArray ba_as_hex_string = datas.toHex();
            ui->  receivehexa->appendPlainText(ba_as_hex_string);
            ui-> receivehexa->appendPlainText("");

        }
        else{
           // ui->  receivehexa->appendPlainText(ba_as_hex_string);

        }


    } });
   /* connect(serialVDLfreq,&QSerialPort::readyRead,[&](){

        qDebug() << "New data available: " << serialVDLfreq->bytesAvailable();
        QByteArray data = serialVDLfreq->readAll();
        qDebug() << data;

        QByteArray ba_as_hex_string = data.toHex();

        serialVDLfreq->waitForReadyRead(3000);

        if(data.length() == 17 && (data[5] == 80) && (data[8] >= 18)){

            int PARAM_REQ = ui-> frequence_2 -> value();
            ui-> receive-> appendPlainText(QString("PARAM_CONF_RX = %1").arg(PARAM_REQ));
            ui-> receivehexa->appendPlainText(ba_as_hex_string);
        }

        if(data.length() == 12 && data[9] == 1 && data[11] == 119){
            ui->receive->appendPlainText(QString("MODE_SET_CONF_RX"));
            QByteArray ba_as_hex_string = data.toHex();
            ui->  receivehexa->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

         }
        if(data.length() == 12 && data[5] == 83){
            ui->receive->appendPlainText(QString("VDR_ERROR_IND"));
            QByteArray ba_as_hex_string = data.toHex();
            ui->  receivehexa->appendPlainText(ba_as_hex_string);
        }
        if(data.length() == 15 && data[5] == 84){
            ui->receive->appendPlainText(QString("SQP_IND"));
            QByteArray ba_as_hex_string = data.toHex();
            ui->  receivehexa->appendPlainText(ba_as_hex_string);
            sockUdp->writeDatagram(data,QHostAddress::LocalHost, 2022);

        }
        if( data.length() ==  ui->donnees_2->value() +12 && data[5] == 81 ){
              ui->receive->appendPlainText(QString("UNIT DATA IND"));
              ui->  receivehexa->appendPlainText(ba_as_hex_string);
         }
        if( data.length() ==  10 && data[5] == 81 ){
              ui->receive->appendPlainText(QString("UNIT DATA IND"));
              ui->  receivehexa->appendPlainText(ba_as_hex_string);
         }

        else{
            ui->  receivehexa->appendPlainText(ba_as_hex_string);
        }

    });*/




    QList<QSerialPortInfo> list;

    list = QSerialPortInfo::availablePorts();

    ui->transmitter->addItem(QStringLiteral("COM7"));
    ui ->receiver->addItem(QStringLiteral("COM5"));



    //Set baud rate
    ui->comboBox_baudRate->addItem(QStringLiteral("BaudRate"));
    ui->comboBox_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudRate->addItem(QStringLiteral("57600"),QSerialPort::Baud57600);
    ui->comboBox_baudRate->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);

    ui->comboBox_baudRate_2->addItem(QStringLiteral("BaudRate"));
    ui->comboBox_baudRate_2->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudRate_2->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudRate_2->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudRate_2->addItem(QStringLiteral("57600"),QSerialPort::Baud57600);
    ui->comboBox_baudRate_2->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);

    // set data bits
    ui->comboBox_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_dataBits->setCurrentIndex(3);


    //Set stop bit
    ui->comboBox_stop->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBox_stop->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

}

vdl2::~vdl2()
{
    delete ui;
}

void vdl2::on_actionClose_triggered()
{
    vdl2::close();

}
uint16_t CRCVDL2(uint8_t *x, uint16_t len) {
    uint8_t *data = x;
    uint16_t crc=0;

    while (len--) {
        crc ^= *data++;
        for (uint8_t k = 0; k < 8; k++)
            if (crc & 1) crc =(crc >> 1) ^ 0x8408;
            else crc = crc >> 1;
    }

    return crc;
}

void vdl2::on_actionClear_triggered()
{
    ui->  send->clear();
    ui->  receive->clear();
    ui->  receivehexa->clear();
    ui->  sendhexa->clear();
}


void vdl2::on_actionPARAM_REQ_triggered()
{
    serialVDL->setBaudRate((ui->comboBox_baudRate->currentText().toInt()));
    serialVDL->setDataBits(QSerialPort::Data8);
    serialVDL->setParity(QSerialPort::NoParity);
    serialVDL->setStopBits(QSerialPort::OneStop);
    serialVDL->setFlowControl(QSerialPort::NoFlowControl);



    struct tsPid20vdl {
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t mode;
        uint8_t pid;
        uint8_t dfl1_msb;
        uint8_t dfl2_lsb;
        uint8_t freq_msb;
        uint8_t freq_lsb;
        uint8_t tm1;
        uint8_t tm2;
        uint8_t M1_msb;
        uint8_t M1_lsb;
        uint8_t persistance;
        uint16_t crc;
    };

    /*-----------------------------------------------------------------------------------*/
    // Algo de traitement de la fréquence
    //Il faut créer une liste qui prend la taille de la valeur de fréquence désiré
    //mettre le nombre saisie dans le tableu sous forme de chiffre

    /*-----------------------------------------------------------------------------------------*/


    int T[6];
    int c = 0;
    int msb[2] , lsb[2] ;
    int monnombre = ui->frequence_2->value();
    int mynumber = 0;
    int p =1 , e = 1 , o = 1;
    /*------------------mettre le nmbre saisie dans un tableau en forme de chiffre---------------------------------*/


    while(monnombre!=0) {
        int mod = monnombre%10;
        T[c] = mod;
        monnombre-=mod;
        monnombre/=10;
        c++;
    }

    /*------------------------Reconversion du tableau en chiffre-----------------------------------------------------*/
   for(int i = 1; i<5 ; i++){
        mynumber =  mynumber + T[i]*p;
        p=p*10;
    }

    msb[0] = T[4] ;
    msb[1] = T[3];

    lsb[0] = T[2];
    lsb[1] = T[1];

    uint8_t var = 0 ;
    for(int k=1;k>=0;k--) {
            var = var +msb[k]*e ; //freq_msb
            e = e*10 ;
    }
    uint8_t var2 = 0 ;
    for(int k=1;k>=0;k--) {
            var2 = var2 +lsb[k]*o ; //freq_lsb
            o = o*10 ;
    }

    uint8_t var1 = ui -> timer1 -> value();
    uint8_t var3 =ui -> timer2 -> value();
    uint16_t var4 = ui->counterM1->value();
    uint8_t var5 = ui->persistance->value();

    uint16_t value = var4;
    uint8_t array[2];
    array[0]=value & 0xff;
    array[1]=(value >> 8);


    /*-------------Définition des champs de la trame et son envoie dans un fil------------- */


    if  ( ui -> frequence_2 -> value() >= 118000  &  ui -> frequence_2 -> value() <= 136000 )
            {

                    tsPid20vdl Pid20;
                    Pid20.synchro [0] = synchrovdl2[0];
                    Pid20.synchro [1] = synchrovdl2[1];
                    Pid20.synchro [2] = synchrovdl2[2];
                    Pid20.synchro [3] = synchrovdl2[3];
                    Pid20.mode = 0xF1;
                    Pid20.pid = 0x20;
                    Pid20.dfl1_msb =0x00;
                    Pid20.dfl2_lsb = 0x07;
                    Pid20.freq_msb=var;
                    Pid20.freq_lsb = var2;
                    Pid20.tm1 =var1;
                    Pid20.tm2 =var3;
                    Pid20.M1_lsb= array[0];
                    Pid20.M1_msb= array[1];
                    Pid20.persistance= var5;

                    /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

                    unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf1, 0x20, 0x00, 0x07,var,var2,var1,var3,array[1], array[0],var5 };
                    unsigned int myDataLen;
                    unsigned short myCrc;
                    myDataLen = sizeof(myData);
                    myCrc = CRCVDL2(myData, myDataLen);
                    myCrc = ((myCrc << 8) | (myCrc >> 8));
                    Pid20.crc=myCrc;

                    /*-------------------Intégration des champs du buffer dans un fil d'envoie--------------------------*/


                    QByteArray buffer;

                    //  ecrire les données dans le flux d'envoie "s"
                    QDataStream s(&buffer, QIODevice::WriteOnly);
                    if (false){

                        s.setByteOrder(QDataStream::LittleEndian);
                    }
                    s << (uint8_t)Pid20.synchro [0]<< (uint8_t)Pid20.synchro [1]<<(uint8_t)Pid20.synchro [2]<<(uint8_t)Pid20.synchro [3]
                      << (uint8_t)Pid20.mode << (uint8_t)Pid20.pid << (uint8_t)Pid20.dfl1_msb <<(uint8_t)Pid20.dfl2_lsb
                      <<(uint8_t)Pid20.freq_msb<<(uint8_t)Pid20.freq_lsb<<(uint8_t)Pid20.tm1<<(uint8_t)Pid20.tm2
                      <<(uint8_t)Pid20.M1_msb<<(uint8_t)Pid20.M1_lsb<<(uint8_t)Pid20.persistance<<(uint16_t)Pid20.crc;

                    serialVDL->write(buffer);
                    serialVDL->flush();
                    serialVDL->waitForBytesWritten(1000);

                    int PARAM_REQ = ui -> frequence_2 -> value();
                    ui-> send -> appendPlainText(QString("PARAM_REQ_TX = %1").arg(PARAM_REQ));
                    QByteArray ba_as_hex_string = buffer.toHex();
                    ui->  sendhexa->appendPlainText(ba_as_hex_string);
                    /*/connect(serialVDL,&QSerialPort::readyRead,[&](){

                        qDebug() << "New data available: " << serialVDL->bytesAvailable();
                        QByteArray datas = serialVDL->readAll();
                        qDebug() << datas;
                        //Affichage des trames en héxadécimal


                        if(  datas.length() < 17 && datas.length() != 0 ){
                            while(serialVDL->waitForReadyRead(100)){

                                qDebug() << "concatenate the rest of the array. size: " << serialVDL->bytesAvailable();
                                datas = datas.append(serialVDL->readAll());
                                qDebug() << datas;
                                QByteArray ba_as_hex_string = datas.append(serialVDL->readAll()).toHex();

                                ui-> receivehexa->appendPlainText(ba_as_hex_string);

                            }
                        }





                    });
                    connect(serialVDL,&QSerialPort::readyRead,[&](){
                            while(serialVDL->waitForReadyRead(100)){
                             QByteArray datas = serialVDL->readAll();
                             QByteArray receivedData;

                            if(datas.length() < 17 && serialVDL->bytesAvailable() !=0 ){
                                   qDebug() << "New data available: " << serialVDL->bytesAvailable();

                                    receivedData += serialVDL->readAll();

                            }
                            qDebug() << "onReadyRead: new bytes:" << receivedData.toHex(' ');

                            qDebug() << datas;
                            ui-> receivehexa->appendPlainText(datas.toHex());



                            //Affichage des trames en héxadécimal

                    }});*/

}}
void vdl2::on_actionPWR_SET_REQ_triggered()
{
    serialVDL->setBaudRate((ui->comboBox_baudRate->currentText().toInt()));
    serialVDL->setDataBits(QSerialPort::Data8);
    serialVDL->setParity(QSerialPort::NoParity);
    serialVDL->setStopBits(QSerialPort::OneStop);
    serialVDL->setFlowControl(QSerialPort::NoFlowControl);



    struct tsPidF1vdl {
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t protocoleCode;
        uint8_t pid;
        uint8_t lengthMsb;
        uint8_t lengthLsb;
        uint8_t pwr;
        uint8_t req;
        uint16_t crc;

    };
    /*----------------instruction conditionnelle pour l'envoie de la puissance à la Radio------------------------*/


          int pwr = ui -> puissance -> value();
          uint8_t var3;

         if(  pwr == 50 ) {
               var3 = 47;
          }
         if ( pwr == 25){
               var3 = 44;
          }
         if ( pwr == 10) {

               var3 = 40;
          }

         /*-------------Définition des champs de la trame ------------- */


          tsPidF1vdl PidF1;

          PidF1.synchro [0] = synchrovdl2[0];
          PidF1.synchro [1] = synchrovdl2[1];
          PidF1.synchro [2] = synchrovdl2[2];
          PidF1.synchro [3] = synchrovdl2[3];
          PidF1.protocoleCode = 0xF1;
          PidF1.pid=0xF1;
          PidF1.lengthMsb = 0x00;
          PidF1.lengthLsb = 0x02;
          PidF1.pwr=var3;
          PidF1.req=0;

          /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

          unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf1, 0xf1,0x00,0x02,var3,0x00};
          unsigned int myDataLen;
          unsigned short myCrc;
          myDataLen = sizeof(myData);
          myCrc = CRCVDL2(myData, myDataLen);
          myCrc = ((myCrc << 8) | (myCrc >> 8));
          PidF1.crc=myCrc ;

          /*-------------------Intégration des champs du buffer dans un fil d'envoie "s"--------------------------*/

          QByteArray buffer;
          QDataStream s(&buffer, QIODevice::WriteOnly);

          if (false){

              s.setByteOrder(QDataStream::LittleEndian);
          }
             s << (uint8_t)PidF1.synchro [0]<< (uint8_t)PidF1.synchro [1]<<(uint8_t)PidF1.synchro [2]
               <<(uint8_t)PidF1.synchro [3]  << (uint8_t)PidF1.protocoleCode << (uint8_t)PidF1.pid
               << (uint8_t)PidF1.lengthMsb <<(uint8_t)PidF1.lengthLsb<<(uint8_t)PidF1.pwr
               <<(uint8_t)PidF1.req<<(uint16_t)PidF1.crc;

             serialVDL->write(buffer);
             serialVDL->flush();
             serialVDL->waitForBytesWritten(1000);

             int PWR_SET_REQ = ui-> puissance-> value();
             ui-> send->appendPlainText(QString("PWR_SET_REQ_TX = %1").arg(PWR_SET_REQ));
             QByteArray ba_as_hex_string = buffer.toHex();
             ui->  sendhexa->appendPlainText(ba_as_hex_string);



}


void vdl2::on_actionMODE_SET_REQ_triggered()
{

    serialVDL->setBaudRate((ui->comboBox_baudRate->currentText().toInt()));
    serialVDL->setDataBits(QSerialPort::Data8);
    serialVDL->setParity(QSerialPort::NoParity);
    serialVDL->setStopBits(QSerialPort::OneStop);
    serialVDL->setFlowControl(QSerialPort::NoFlowControl);

    struct tsPidF0vdl{
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t protocoleCode;
        uint8_t pid;
        uint8_t lengthMsb;
        uint8_t lengthLsb;
        uint8_t mode;
        uint8_t req;
        uint16_t crc;

    };
    /*-------------Définition des champs de la trame ------------- */

       tsPidF0vdl PidF0;
       PidF0.synchro [0] = synchrovdl2[0];
       PidF0.synchro [1] = synchrovdl2[1];
       PidF0.synchro [2] = synchrovdl2[2];
       PidF0.synchro [3] = synchrovdl2[3];
       PidF0.protocoleCode = 0xF1;
       PidF0.pid=0xF0;
       PidF0.lengthMsb = 0x00;
       PidF0.lengthLsb = 0x02;
       PidF0.mode=0xF2;
       PidF0.req=0;

       /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/


       unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf1, 0xf0,0x00,0x02,0xf2,0x00};
       unsigned int myDataLen;
       unsigned short myCrc;
       myDataLen = sizeof(myData);
       myCrc = CRCVDL2(myData, myDataLen);
       myCrc = ((myCrc << 8) | (myCrc >> 8));
       PidF0.crc= myCrc;

       /*-------------------Intégration des champs du buffer dans un fil d'envoie--------------------------*/

       QByteArray buffer;

       //  ecrire les données dans le flux d'envoie
       QDataStream s(&buffer, QIODevice::WriteOnly);

       if (false){

           s.setByteOrder(QDataStream::LittleEndian);

       }
       s  << (uint8_t)PidF0.synchro [0]<< (uint8_t)PidF0.synchro [1]<<(uint8_t)PidF0.synchro [2]
          <<(uint8_t)PidF0.synchro [3]  << (uint8_t)PidF0.protocoleCode << (uint8_t)PidF0.pid
          << (uint8_t)PidF0.lengthMsb<< (uint8_t)PidF0.lengthLsb<< (uint8_t)PidF0.mode
          << (uint8_t)PidF0.req   << (uint16_t)PidF0.crc  ;

       /*--------------------------------Envoie des données--------------------------------------*/
       serialVDL->write(buffer);
       serialVDL->flush();
       serialVDL->waitForBytesWritten(1000);

       ui-> send -> appendPlainText(QString("MODE_SET_REQ_TX"));
       QByteArray ba_as_hex_string = buffer.toHex();
       ui->  sendhexa->appendPlainText(ba_as_hex_string);

}


void vdl2::on_actionPURGE_REQ_triggered()
{
    serialVDL->setBaudRate((ui->comboBox_baudRate->currentText().toInt()));
    serialVDL->setDataBits(QSerialPort::Data8);
    serialVDL->setParity(QSerialPort::NoParity);
    serialVDL->setStopBits(QSerialPort::OneStop);
    serialVDL->setFlowControl(QSerialPort::NoFlowControl);


    struct tsPid010bvdl {
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t sal;
        uint8_t donnees;
        uint8_t pid;
        uint8_t protocoelCode;
        uint16_t crc;

    };
    /*-------------Définition des champs de la trame ------------- */


       tsPid010bvdl Pid010b;

       Pid010b.synchro [0] = synchrovdl2[0];
       Pid010b.synchro [1] = synchrovdl2[1];
       Pid010b.synchro [2] = synchrovdl2[2];
       Pid010b.synchro [3] = synchrovdl2[3];
       Pid010b.protocoelCode=0x5f;
       Pid010b.pid=0x14;
       Pid010b.donnees=0x00;
       Pid010b.sal=0x00;

       /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

       unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0x5f, 0x14,0x00,0x00};
       unsigned int myDataLen;
       unsigned short myCrc;
       myDataLen = sizeof(myData);
       myCrc = CRCVDL2(myData, myDataLen);
       myCrc = ((myCrc << 8) | (myCrc >> 8));
       Pid010b.crc=myCrc;

       /*-------------------Intégration des champs du buffer dans un flux d'envoie--------------------------*/


       QByteArray buffer;
       //  ecrire les données dans le flux d'envoie
       QDataStream s(&buffer, QIODevice::WriteOnly);

       if (false){

           s.setByteOrder(QDataStream::LittleEndian);

       }
       s  << (uint8_t)Pid010b.synchro [0]<< (uint8_t)Pid010b.synchro [1]<<(uint8_t)Pid010b.synchro [2]
          <<(uint8_t)Pid010b.synchro [3]<<(uint8_t)Pid010b.protocoelCode<<(uint8_t)Pid010b.pid
          <<(uint8_t)Pid010b.donnees<<(uint8_t)Pid010b.sal<<(uint16_t)Pid010b.crc;

       /*--------------------------------Envoie des données--------------------------------------*/

       serialVDL->write(buffer);
       serialVDL->flush();
       serialVDL->waitForBytesWritten(1000);

       ui-> send -> appendPlainText(QString("PURGE_REQ"));
       QByteArray ba_as_hex_string = buffer.toHex();
       ui->  sendhexa->appendPlainText(ba_as_hex_string);

}


void vdl2::on_actionUNIT_DATA_REQ_triggered()
{
    serialVDL->setBaudRate((ui->comboBox_baudRate->currentText().toInt()));
    serialVDL->setDataBits(QSerialPort::Data8);
    serialVDL->setParity(QSerialPort::NoParity);
    serialVDL->setStopBits(QSerialPort::OneStop);
    serialVDL->setFlowControl(QSerialPort::NoFlowControl);

    struct tsPid21{
           uint8_t synchro[SIZE_SYNCHRO_SL];
           uint8_t protocoleCode;
           uint8_t pid;
           uint8_t lengthMsb;
           uint8_t lengthLsb;
           uint8_t clic;
           uint16_t crc;
           char str1[16384];


       };

       /*la variable counter change à chaque fois on clique sur le button unit data req */
       counterm++;

       /*--------------Définition des 16k valeur à pouvoir envoyer sous le protocole VDL 2------------------------*/

            uint16_t value = ui->donnees_2->value()+1;
            uint8_t array[2];
            array[0]=value & 0xff;
            array[1]=(value >> 8);

            /*----fonction rand()- retourne des entier aléatoire de 0 à 16k----*/

           char table22[ ui->donnees_2->value() ] ;
           unsigned char table222[ui->donnees_2->value()] ;
             for(int i=0; i< (ui->donnees_2->value()); i++){
               srand((unsigned) time(0));
               int strn =  (rand() % 256);
               unsigned char monsef = (unsigned char) strn;
               table22[i] = strn;

               table222[i] = monsef;

               }

            char str[16384];

            str[16383] = '\0';
            strncpy ( str, table22, ui->donnees_2->value() );
            QByteArray databuf = QByteArray(reinterpret_cast<char*>(table22), ui->donnees_2->value());


            /*-------------Définition des champs de la trame et l'intégrer dans un flus d'envoie------------- */

            tsPid21 Pid21;

            Pid21.synchro [0] = synchrovdl2[0];
            Pid21.synchro [1] = synchrovdl2[1];
            Pid21.synchro [2] = synchrovdl2[2];
            Pid21.synchro [3] = synchrovdl2[3];
            Pid21.protocoleCode = 0xF1;
            Pid21.pid = 0x21;
            Pid21.lengthMsb=array[1];
            Pid21.lengthLsb=array[0];
            Pid21.clic=counterm;

            /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/


            unsigned char myData3[] ={0xff ,0x00, 0xf0, 0xca,0xf1, 0x21,array[1],array[0],counterm};
            unsigned char allstrings[sizeof(table222)+sizeof(myData3)];
            memcpy(allstrings,myData3,sizeof(myData3));
            memcpy(allstrings+sizeof(myData3),table222,sizeof(table222));
            unsigned short myCrc;
            unsigned int myDataLen = sizeof(table222)+ sizeof(myData3);

            myCrc = CRCVDL2(allstrings, myDataLen);
            myCrc = ((myCrc << 8) | (myCrc >> 8));

            Pid21.crc=myCrc;



        /*-------------------Intégration des champs du buffer dans un flux d'envoie--------------------------*/


        QByteArray buffer;
        //  ecrire les données dans le flux d'envoie
        QDataStream s(&buffer, QIODevice::WriteOnly);

        if (false){

            s.setByteOrder(QDataStream::LittleEndian);
        }
         s << (uint8_t)Pid21.synchro [0]<< (uint8_t)Pid21.synchro [1]<<(uint8_t)Pid21.synchro [2]
           <<(uint8_t)Pid21.synchro [3]  << (uint8_t)Pid21.protocoleCode << (uint8_t)Pid21.pid
           << (uint8_t)Pid21.lengthMsb<< (uint8_t)Pid21.lengthLsb
           << (uint8_t)Pid21.clic ;



           QByteArray buffer3;
           //  ecrire les données dans le flux d'envoie

           QDataStream g(&buffer3, QIODevice::WriteOnly);

           if (false){

               g.setByteOrder(QDataStream::LittleEndian);
           }
           g <<(uint16_t) Pid21.crc;


           QByteArray buffer5 = buffer +databuf + buffer3;

           /*--------------------------------Envoie des données--------------------------------------*/

           serialVDL->write(buffer5);
           serialVDL->flush();
           serialVDL->waitForBytesWritten(1000);

           ui-> send -> appendPlainText(QString("UNIT_DATA_REQ"));
           QByteArray ba_as_hex_string = buffer5.toHex();
           ui->  sendhexa->appendPlainText(ba_as_hex_string);


}


void vdl2::on_actionPARAM_REQ_2_triggered()
{
    serialVDLfreq->setBaudRate((ui->comboBox_baudRate_2->currentText().toInt()));
    serialVDLfreq->setDataBits(QSerialPort::Data8);
    serialVDLfreq->setParity(QSerialPort::NoParity);
    serialVDLfreq->setStopBits(QSerialPort::OneStop);
    serialVDLfreq->setFlowControl(QSerialPort::NoFlowControl);



    struct tsPid20vdl {
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t mode;
        uint8_t pid;
        uint8_t dfl1_msb;
        uint8_t dfl2_lsb;
        uint8_t freq_msb;
        uint8_t freq_lsb;
        uint8_t tm1;
        uint8_t tm2;
        uint8_t M1_msb;
        uint8_t M1_lsb;
        uint8_t persistance;
        uint16_t crc;
    };

    /*-----------------------------------------------------------------------------------*/
    // Algo de traitement de la fréquence
    //Il faut créer une liste qui prend la taille de la valeur de fréquence désiré
    //mettre le nombre saisie dans le tableu sous forme de chiffre

    /*-----------------------------------------------------------------------------------------*/


    int T[6];
    int c = 0;
    int msb[2] , lsb[2] ;
    int monnombre = ui->frequence_2->value();
    int mynumber = 0;
    int p =1 , e = 1 , o = 1;
    /*------------------mettre le nmbre saisie dans un tableau en forme de chiffre---------------------------------*/


    while(monnombre!=0) {
        int mod = monnombre%10;
        T[c] = mod;
        monnombre-=mod;
        monnombre/=10;
        c++;
    }

    /*------------------------Reconversion du tableau en chiffre-----------------------------------------------------*/
   for(int i = 1; i<5 ; i++){
        mynumber =  mynumber + T[i]*p;
        p=p*10;
    }

    msb[0] = T[4] ;
    msb[1] = T[3];

    lsb[0] = T[2];
    lsb[1] = T[1];

    uint8_t var = 0 ;
    for(int k=1;k>=0;k--) {
            var = var +msb[k]*e ; //freq_msb
            e = e*10 ;
    }
    uint8_t var2 = 0 ;
    for(int k=1;k>=0;k--) {
            var2 = var2 +lsb[k]*o ; //freq_lsb
            o = o*10 ;
    }

    uint8_t var1 = ui -> timer1 -> value();
    uint8_t var3 =ui -> timer2 -> value();
    uint16_t var4 = ui->counterM1->value();
    uint8_t var5 = ui->persistance->value();

    uint16_t value = var4;
    uint8_t array[2];
    array[0]=value & 0xff;
    array[1]=(value >> 8);


    /*-------------Définition des champs de la trame et son envoie dans un fil------------- */


    if  ( ui -> frequence_2 -> value() >= 118000  &  ui -> frequence_2 -> value() <= 136000 )
            {

                    tsPid20vdl Pid20;
                    Pid20.synchro [0] = synchrovdl2[0];
                    Pid20.synchro [1] = synchrovdl2[1];
                    Pid20.synchro [2] = synchrovdl2[2];
                    Pid20.synchro [3] = synchrovdl2[3];
                    Pid20.mode = 0xF1;
                    Pid20.pid = 0x20;
                    Pid20.dfl1_msb =0x00;
                    Pid20.dfl2_lsb = 0x07;
                    Pid20.freq_msb=var;
                    Pid20.freq_lsb = var2;
                    Pid20.tm1 =var1;
                    Pid20.tm2 =var3;
                    Pid20.M1_lsb= array[0];
                    Pid20.M1_msb= array[1];
                    Pid20.persistance= var5;

                    /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

                    unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf1, 0x20, 0x00, 0x07,var,var2,var1,var3,array[1], array[0],var5 };
                    unsigned int myDataLen;
                    unsigned short myCrc;
                    myDataLen = sizeof(myData);
                    myCrc = CRCVDL2(myData, myDataLen);
                    myCrc = ((myCrc << 8) | (myCrc >> 8));
                    Pid20.crc=myCrc;

                    /*-------------------Intégration des champs du buffer dans un fil d'envoie--------------------------*/


                    QByteArray buffer;

                    //  ecrire les données dans le flux d'envoie "s"
                    QDataStream s(&buffer, QIODevice::WriteOnly);
                    if (false){

                        s.setByteOrder(QDataStream::LittleEndian);
                    }
                    s << (uint8_t)Pid20.synchro [0]<< (uint8_t)Pid20.synchro [1]<<(uint8_t)Pid20.synchro [2]<<(uint8_t)Pid20.synchro [3]
                      << (uint8_t)Pid20.mode << (uint8_t)Pid20.pid << (uint8_t)Pid20.dfl1_msb <<(uint8_t)Pid20.dfl2_lsb
                      <<(uint8_t)Pid20.freq_msb<<(uint8_t)Pid20.freq_lsb<<(uint8_t)Pid20.tm1<<(uint8_t)Pid20.tm2
                      <<(uint8_t)Pid20.M1_msb<<(uint8_t)Pid20.M1_lsb<<(uint8_t)Pid20.persistance<<(uint16_t)Pid20.crc;

                    serialVDLfreq->write(buffer);
                    serialVDLfreq->flush();
                    serialVDLfreq->waitForBytesWritten(1000);

                    int PARAM_REQ = ui -> frequence_2 -> value();
                    ui-> send -> appendPlainText(QString("PARAM_REQ_RX = %1").arg(PARAM_REQ));

                    QByteArray ba_as_hex_string = buffer.toHex();
                    ui->  sendhexa->appendPlainText(ba_as_hex_string);

}
}


void vdl2::on_actionMODE_SET_REQ_2_triggered()
{

    serialVDLfreq->setBaudRate((ui->comboBox_baudRate_2->currentText().toInt()));
    serialVDLfreq->setDataBits(QSerialPort::Data8);
    serialVDLfreq->setParity(QSerialPort::NoParity);
    serialVDLfreq->setStopBits(QSerialPort::OneStop);
    serialVDLfreq->setFlowControl(QSerialPort::NoFlowControl);



    struct tsPidF0vdl{
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t protocoleCode;
        uint8_t pid;
        uint8_t lengthMsb;
        uint8_t lengthLsb;
        uint8_t mode;
        uint8_t req;
        uint16_t crc;

    };
    /*-------------Définition des champs de la trame ------------- */

       tsPidF0vdl PidF0;
       PidF0.synchro [0] = synchrovdl2[0];
       PidF0.synchro [1] = synchrovdl2[1];
       PidF0.synchro [2] = synchrovdl2[2];
       PidF0.synchro [3] = synchrovdl2[3];
       PidF0.protocoleCode = 0xF1;
       PidF0.pid=0xF0;
       PidF0.lengthMsb = 0x00;
       PidF0.lengthLsb = 0x02;
       PidF0.mode=0xF2;
       PidF0.req=0;

       /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/


       unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf1, 0xf0,0x00,0x02,0xf2,0x00};
       unsigned int myDataLen;
       unsigned short myCrc;
       myDataLen = sizeof(myData);
       myCrc = CRCVDL2(myData, myDataLen);
       myCrc = ((myCrc << 8) | (myCrc >> 8));
       PidF0.crc= myCrc;

       /*-------------------Intégration des champs du buffer dans un fil d'envoie--------------------------*/

       QByteArray buffer;

       //  ecrire les données dans le flux d'envoie
       QDataStream s(&buffer, QIODevice::WriteOnly);

       if (false){

           s.setByteOrder(QDataStream::LittleEndian);

       }
       s  << (uint8_t)PidF0.synchro [0]<< (uint8_t)PidF0.synchro [1]<<(uint8_t)PidF0.synchro [2]
          <<(uint8_t)PidF0.synchro [3]  << (uint8_t)PidF0.protocoleCode << (uint8_t)PidF0.pid
          << (uint8_t)PidF0.lengthMsb<< (uint8_t)PidF0.lengthLsb<< (uint8_t)PidF0.mode
          << (uint8_t)PidF0.req   << (uint16_t)PidF0.crc  ;

       /*--------------------------------Envoie des données--------------------------------------*/
       serialVDLfreq->write(buffer);
       serialVDLfreq->flush();
       serialVDLfreq->waitForBytesWritten(1000);

       ui-> send -> appendPlainText(QString("MODE_SET_REQ_RX"));

       QByteArray ba_as_hex_string = buffer.toHex();
       ui->  sendhexa->appendPlainText(ba_as_hex_string);


}


void vdl2::on_actionSQP_triggered()
{
    //sqpvdl.show();
}


void vdl2::on_pushButton_clicked()
{
    sqpvdl.show();

}


void vdl2::on_actionVersion_triggered()
{
    vesion.show();
}

