/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "client.h"
#include "enum.h"
#include "vlc.h"

#include "include/plug/bbcv_plugapi.h"
#include "include/plug/ipanel_key.h"
#include "include/eterm_porting.h"
#include "include/os/blccore_os.h"

#include <stdio.h>
#define qtu( i ) ((i).toUtf8().constData())

Client::Client(QWidget *parent) : QWidget(parent) {
    createLeftLayout();
    createRightLayout();

    connect(&plugEventThread, SIGNAL(log(QString)), this,
            SLOT(showLogs(QString)));
    connect(&plugEventThread, SIGNAL(sendStatus(bool)), this,
            SLOT(proccessLoginResult(bool)));
    connect(&plugEventThread, SIGNAL(sendOSD(QString)), this,
            SLOT(proccessOSD(QString)));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("仿真终端v0.1.2"));
    setWindowFlags(Qt::WindowShadeButtonHint);
    this->setMouseTracking(true);

    loadDefautConf();
    StbInit();
}

void Client::createUpperLeftLayout() {
    hostLabel = new QLabel(tr("&接入网关:"));
    ipLineEdit = new QLineEdit;
    portLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    hostLabel->setBuddy(ipLineEdit);
    hostLabel->setBuddy(portLineEdit);

    termidLabel = new QLabel(tr("&机顶盒号:"));
    termidLineEdit = new QLineEdit;
    termidLabel->setBuddy(termidLineEdit);

    regionIdLabel = new QLabel(tr("&RegionID:"));
    regionIdLineEdit = new QLineEdit;
    regionIdLabel->setBuddy(regionIdLineEdit);

    upperLeftLayout = new QGridLayout;

    upperLeftLayout->addWidget(hostLabel, 0, 0);
    upperLeftLayout->addWidget(ipLineEdit, 0, 1);
    upperLeftLayout->addWidget(portLineEdit, 0, 2);
    upperLeftLayout->addWidget(termidLabel, 1, 0);
    upperLeftLayout->addWidget(termidLineEdit, 1, 1);
    upperLeftLayout->addWidget(regionIdLabel, 2, 0);
    upperLeftLayout->addWidget(regionIdLineEdit, 2, 1);
}

void Client::createButtonLeftLayout() {
    logText = new QTextEdit;
    QColor *logBackColor = new QColor;
    logBackColor->setBlue(255);
    logText->setTextColor(*logBackColor);

    bottomLeftLayout = new QGridLayout;
    bottomLeftLayout->addWidget(logText, 10, Qt::AlignLeft);
}

void Client::createLeftLayout() {
    createUpperLeftLayout();
    createButtonLeftLayout();

    leftLayout = new QVBoxLayout;
    leftLayout->addLayout(upperLeftLayout);
    leftLayout->addLayout(bottomLeftLayout);
}

void Client::createUpperRightLayout() {
    poweronButton = new QPushButton(tr("开机"));
    poweronButton->setDefault(true);

    loginButton = new QPushButton(tr("登录"));
    loginButton->setDefault(false);
    loginButton->setEnabled(false);

    irrButton1 = new QPushButton(tr("1"));
    irrButton2 = new QPushButton(tr("2"));
    irrButton3 = new QPushButton(tr("3"));
    irrButton4 = new QPushButton(tr("4"));
    irrButton5 = new QPushButton(tr("5"));
    irrButton6 = new QPushButton(tr("6"));
    irrButton7 = new QPushButton(tr("7"));
    irrButton8 = new QPushButton(tr("8"));
    irrButton9 = new QPushButton(tr("9"));
    irrButton0 = new QPushButton(tr("0"));

    irrButtonFF = new QPushButton(tr("快进"));
    irrButtonUpArrow = new QToolButton;
    irrButtonUpArrow->setArrowType(Qt::UpArrow);
    irrButtonFR = new QPushButton(tr("快退"));
    irrButtonLeftArrow = new QToolButton;
    irrButtonLeftArrow->setArrowType(Qt::LeftArrow);
    irrButtonOK = new QPushButton(tr("OK"));
    irrButtonRightArrow = new QToolButton;
    irrButtonRightArrow->setArrowType(Qt::RightArrow);
    irrButtonDownArrow = new QToolButton;
    irrButtonDownArrow->setArrowType(Qt::DownArrow);
    irrButtonReturn = new QPushButton(tr("返回"));
    irrButtonQuit = new QPushButton(tr("退出"));

    //    irrButton1->setFixedSize(50, 20);
    //    irrButton2->setFixedSize(50, 20);
    //    irrButton3->setFixedSize(50, 20);
    //    irrButton4->setFixedSize(50, 20);
    //    irrButton5->setFixedSize(50, 20);
    //    irrButton6->setFixedSize(50, 20);
    //    irrButton7->setFixedSize(50, 20);
    //    irrButton8->setFixedSize(50, 20);
    //    irrButton9->setFixedSize(50, 20);
    //    irrButton0->setFixedSize(50, 20);
    //    irrButtonFF->setFixedSize(50, 20);
    irrButtonUpArrow->setFixedSize(80, 23);
    //    irrButtonFR->setFixedSize(50, 20);
    irrButtonLeftArrow->setFixedSize(80, 23);
    //    irrButtonOK->setFixedSize(50, 20);
    irrButtonRightArrow->setFixedSize(80, 23);
    irrButtonDownArrow->setFixedSize(80, 23);
    //    irrButtonReturn->setFixedSize(50, 20);
    //    irrButtonQuit->setFixedSize(50, 20);

    connect(poweronButton, SIGNAL(clicked()), this, SLOT(plugInit()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(handleStbEvent()));

    connect(irrButton0, SIGNAL(clicked()), this, SLOT(irrKeySend0()));
    connect(irrButton1, SIGNAL(clicked()), this, SLOT(irrKeySend1()));
    connect(irrButton2, SIGNAL(clicked()), this, SLOT(irrKeySend2()));
    connect(irrButton3, SIGNAL(clicked()), this, SLOT(irrKeySend3()));
    connect(irrButton4, SIGNAL(clicked()), this, SLOT(irrKeySend4()));
    connect(irrButton5, SIGNAL(clicked()), this, SLOT(irrKeySend5()));
    connect(irrButton6, SIGNAL(clicked()), this, SLOT(irrKeySend6()));
    connect(irrButton7, SIGNAL(clicked()), this, SLOT(irrKeySend7()));
    connect(irrButton8, SIGNAL(clicked()), this, SLOT(irrKeySend8()));
    connect(irrButton9, SIGNAL(clicked()), this, SLOT(irrKeySend9()));
    connect(irrButtonFF, SIGNAL(clicked()), this, SLOT(irrKeySendFF()));
    connect(irrButtonFR, SIGNAL(clicked()), this, SLOT(irrKeySendFW()));
    connect(irrButtonUpArrow, SIGNAL(clicked()), this, SLOT(irrKeySendUp()));
    connect(irrButtonDownArrow, SIGNAL(clicked()), this, SLOT(irrKeySendDown()));
    connect(irrButtonLeftArrow, SIGNAL(clicked()), this, SLOT(irrKeySendLeft()));
    connect(irrButtonRightArrow, SIGNAL(clicked()), this,
            SLOT(irrKeySendRight()));
    connect(irrButtonOK, SIGNAL(clicked()), this, SLOT(irrKeySendOK()));
    connect(irrButtonReturn, SIGNAL(clicked()), this, SLOT(irrKeySendReturn()));
    connect(irrButtonQuit, SIGNAL(clicked()), this, SLOT(irrKeySendQuit()));

    upperRightLayout = new QGridLayout;
    upperRightLayout->addWidget(poweronButton, 0, 0);
    upperRightLayout->addWidget(loginButton, 0, 1);
    upperRightLayout->addWidget(irrButton1, 2, 0);
    upperRightLayout->addWidget(irrButton2, 2, 1);
    upperRightLayout->addWidget(irrButton3, 2, 2);
    upperRightLayout->addWidget(irrButton4, 3, 0);
    upperRightLayout->addWidget(irrButton5, 3, 1);
    upperRightLayout->addWidget(irrButton6, 3, 2);
    upperRightLayout->addWidget(irrButton7, 4, 0);
    upperRightLayout->addWidget(irrButton8, 4, 1);
    upperRightLayout->addWidget(irrButton9, 4, 2);
    upperRightLayout->addWidget(irrButton0, 5, 1);

    upperRightLayout->addWidget(irrButtonFF, 6, 0);
    upperRightLayout->addWidget(irrButtonUpArrow, 6, 1);
    upperRightLayout->addWidget(irrButtonFR, 6, 2);
    upperRightLayout->addWidget(irrButtonLeftArrow, 7, 0);
    upperRightLayout->addWidget(irrButtonOK, 7, 1);
    upperRightLayout->addWidget(irrButtonRightArrow, 7, 2);
    upperRightLayout->addWidget(irrButtonDownArrow, 8, 1);
    upperRightLayout->addWidget(irrButtonReturn, 9, 0);
    upperRightLayout->addWidget(irrButtonQuit, 9, 2);
}

void Client::createButtonRightLayout() {
    simulateKbButton = new QPushButton(tr("开始模拟键盘"));
    simulateMouseButton = new QPushButton(tr("开始模拟鼠标"));
    simulateConsoleButton = new QPushButton(tr("开始模拟手柄"));
    clearLogButton = new QPushButton(tr("清理终端日志"));

    connect(simulateKbButton, SIGNAL(clicked()), this, SLOT(simulateKeyboard()));
    connect(simulateMouseButton, SIGNAL(clicked()), this, SLOT(simulateMouse()));
    connect(simulateConsoleButton, SIGNAL(clicked()), this,
            SLOT(simulateConsole()));
    connect(clearLogButton, SIGNAL(clicked()), this, SLOT(clearLogs()));

    bottomRightLayout = new QGridLayout;
    bottomRightLayout->addWidget(simulateKbButton, 0, 0);
    bottomRightLayout->addWidget(simulateMouseButton, 0, 1);
    bottomRightLayout->addWidget(simulateConsoleButton, 1, 0);
    bottomRightLayout->addWidget(clearLogButton, 1, 1);

    disablekeysButton();
}

void Client::createRightLayout() {
    createUpperRightLayout();
    createButtonRightLayout();

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(upperRightLayout);
    rightLayout->addLayout(bottomRightLayout);
}

void Client::StbInit() {
    poweronFlag = 0;
    loginFlag = 0;
    simulateKbFlag = 0;
    simulateMouseFlag = 0;
    simulateConsoleFlag = 0;

    stb_status = 0;

    BlcCore_SemCreate((int *)&all_sem, 0);
    BlcCore_SemCreate((int *)&ts_sem, 0);
    BlcCore_SemCreate((int *)&plug_sem, 0);

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR) {
        printf("Error initialising WSA.\n");
    }

    plugEventThread.start();
}

void Client::loadDefautConf() {
    termidLineEdit->setText("1104001051800024C1145E59");
    ipLineEdit->setText("192.168.60.238");
    portLineEdit->setText("10000");
    regionIdLineEdit->setText("2089");
}

void Client::updateStbConf() {
    sprintf(gw_ip, "%s", (char *)ipLineEdit->text().toStdString().c_str());
    strcat(gw_ip, ":");
    strcat(gw_ip, portLineEdit->text().toStdString().c_str());

    sprintf(term_id, "%s", (char *)termidLineEdit->text().toStdString().c_str());
    region_id_ = atoi(regionIdLineEdit->text().toStdString().c_str());
    net_type = 4;

    key_standard_id = BlcTermStandardID_Wasu;
}

void Client::plugInit() {
    disablekeysButton();

    if (poweronFlag == 0) {
        showLogs("开机初始化...");
        updateStbConf();
        bbcv_plug_start();
        poweronFlag = 1;
        poweronButton->setText("关机");
        loginButton->setEnabled(true);
        clearLogButton->setEnabled(true);
        showLogs("初始化完毕!");
    } else {
        showLogs("开机关机...");
        poweronFlag = 0;
        poweronButton->setText("开机");
        loginFlag = 0;
        loginButton->setEnabled(false);
        loginButton->setText("登陆");
        clearLogButton->setEnabled(false);

        logoutCloud();
        bbcv_plug_stop();
    }
}

void Client::loginCloud() {

    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    libvlc_log_t *log;

    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);

    // logging
    log = libvlc_log_open (inst);
    libvlc_set_log_verbosity (inst, 2);
    unsigned int level = libvlc_get_log_verbosity (inst);
    printf ("vlc log verbosity level = %d\n", level);

   // QString fileOpen = QFileDialog::getOpenFileName(0
     //                                               ,"Load a file"
       //                                             , "C:/Users/jon/Desktop/vlcok/untitled");
 //   fileOpen.replace("/", "\\");

    /* Create a new item */
    //              m = libvlc_media_new_path (inst, qtu(fileOpen));
//    m = libvlc_media_new_path(inst, "dshow://// :dshow-vdev= :dshow-adev=");
 //ok   m = libvlc_media_new_location(inst, "dshow://// :dshow-vdev= :dshow-adev=");
    m = libvlc_media_new_location(inst, "udp://@:1234");


    // media option
    const char *options[]={"--sout \"#transcode{vcodec=mpeg4,acodec=mpga,vb=800,ab=128} :udp{dst=127.0.0.1,mux=ts}\""};


    for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++)
        libvlc_media_add_option (m, options[i]);


    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);

    /* No need to keep the media now */
    libvlc_media_release (m);

    libvlc_media_player_set_hwnd (mp, NULL);

    /* play the media_player */
    libvlc_media_player_play (mp);

    sleep (1000); /* Let it play a bit */

    /* Stop playing */
    libvlc_media_player_stop (mp);

    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);


    sleep (10000);
    showLogs("开始登陆...");
    updateStbConf();

    if (stb_status != 0) {
        printf("Now is not TV status!\n");
        return;
    }

    BBCV_PlugApi_Param_Login paramlogin;
    memset(&paramlogin, 0, sizeof(paramlogin));
    paramlogin.service_type = 0; // 1;
    // strncpy(paramlogin.url,"www.baidu.com",sizeof(paramlogin.url));

    memset(&semdata, 0, sizeof(semdata));
    semdata.sem_type = BBCV_PlugApi_EventlID_Login;
    semdata.data_len = sizeof(paramlogin);
    memcpy(semdata.data, &paramlogin, semdata.data_len);
    BlcCore_SemPost(all_sem);
}

void Client::logoutCloud() {
    showLogs("开始退出...");
    if (stb_status != 2) {
        printf("Now is not cloud status!\n");
        // demand_status = 0;
        return;
    }

    memset(&semdata, 0, sizeof(semdata));
    semdata.sem_type = BBCV_PlugApi_EventlID_Logout;
    semdata.data_len = 0;

    BlcCore_SemPost(all_sem);

    // demand_status = 0;
}

void Client::handleStbEvent() {
    if (loginFlag == 0) {
        loginCloud();
    } else {
        logoutCloud();

        loginFlag = 0;
        loginButton->setText("登陆");
        disablekeysButton();
    }
}

void Client::keySend() {
    char info[128];

    memset(&semdata, 0, sizeof(semdata));
    semdata.sem_type = BBCV_PlugApi_EventlID_KeyValue;
    semdata.data_len = sizeof(key_value);
    memcpy(semdata.data, &key_value, semdata.data_len);

    BlcCore_SemPost(all_sem);

    sprintf(info, "Send key, key_dev = %d, key = %d, status = %d, x = %d, y = %d",
            key_value.keydev, key_value.keyvalue, key_value.keystate, key_value.x,
            key_value.y);
    showLogs(info);
}

void Client::irrKeySend(int key) {
    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Irr;
    key_value.keyvalue = key;
    key_value.keystate = 2;

    keySend();

    usleep(50 * 1000);

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Irr;
    key_value.keyvalue = key;
    key_value.keystate = 3;

    keySend();
}

void Client::keyPressEvent(QKeyEvent *kEvent) {
    if (simulateKbFlag == 0)
        return;

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Keyboard;
    key_value.keyvalue = kEvent->key();
    key_value.keystate = 2;

    keySend();
    usleep(50 * 1000);
}

void Client::keyReleaseEvent(QKeyEvent *kEvent) {
    if (simulateKbFlag == 0)
        return;

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Keyboard;
    key_value.keyvalue = kEvent->key();
    key_value.keystate = 3;

    keySend();
    usleep(50 * 1000);
}

void Client::mousePressEvent(QMouseEvent *kEvent) {
    if (simulateMouseFlag == 0)
        return;

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Mouse;

    if (kEvent->button() == Qt::LeftButton)
        key_value.keyvalue = 1;
    else if (kEvent->button() == Qt::MidButton)
        key_value.keyvalue = 2;
    else if (kEvent->button() == Qt::RightButton)
        key_value.keyvalue = 3;

    key_value.keystate = 2;
    key_value.x = kEvent->x();
    key_value.y = kEvent->y();

    keySend();
    usleep(50 * 1000);
}

void Client::mouseReleaseEvent(QMouseEvent *kEvent) {
    if (simulateMouseFlag == 0)
        return;

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Mouse;

    if (kEvent->button() == Qt::LeftButton)
        key_value.keyvalue = 1;
    else if (kEvent->button() == Qt::MiddleButton)
        key_value.keyvalue = 2;
    else if (kEvent->button() == Qt::RightButton)
        key_value.keyvalue = 3;

    key_value.keystate = 3;
    key_value.x = kEvent->x();
    key_value.y = kEvent->y();

    keySend();
    usleep(50 * 1000);
}

void Client::mouseMoveEvent(QMouseEvent *kEvent) {
    if (simulateMouseFlag == 0)
        return;

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Mouse;

    if (kEvent->buttons() & Qt::LeftButton) {
        key_value.keyvalue = 1;
        key_value.keystate = 5;
    } else if (kEvent->buttons() & Qt::MiddleButton) {
        key_value.keyvalue = 2;
        key_value.keystate = 1;
    } else if (kEvent->buttons() & Qt::RightButton) {
        key_value.keyvalue = 3;
        key_value.keystate = 6;
    } else {
        key_value.keyvalue = 0;
        key_value.keystate = 1;
    }

    key_value.x = kEvent->x();
    key_value.y = kEvent->y();

    keySend();
    usleep(50 * 1000);
}

void Client::wheelEvent(QWheelEvent *kEvent) {
    if (simulateMouseFlag == 0)
        return;

    memset(&key_value, 0, sizeof(key_value));
    key_value.keydev = BlcTermKeyDevType_Mouse;

    if (kEvent->angleDelta().y() > 0)
        key_value.keyvalue = 1;
    else
        key_value.keyvalue = -1;

    key_value.keystate = 4;
    key_value.x = kEvent->x();
    key_value.y = kEvent->y();

    keySend();
    usleep(50 * 1000);
}

void Client::simulateKeyboard() {
    if (simulateKbFlag == 0) {
        this->grabKeyboard();
        simulateKbButton->setText("停止模拟键盘");
        simulateKbFlag = 1;
    } else {
        this->releaseKeyboard();
        simulateKbButton->setText("开始模拟键盘");
        simulateKbFlag = 0;
    }
}

void Client::simulateMouse() {
    if (simulateMouseFlag == 0) {
        simulateMouseButton->setText("停止模拟鼠标");
        simulateMouseFlag = 1;
    } else {
        simulateMouseButton->setText("开始模拟鼠标");
        simulateMouseFlag = 0;
    }
}

void Client::simulateConsole() {
    if (simulateConsoleFlag == 0) {
        simulateConsoleButton->setText("停止模拟手柄");
        simulateConsoleFlag = 1;
    } else {
        simulateConsoleButton->setText("开始模拟手柄");
        simulateConsoleFlag = 0;
    }
}

void Client::clearLogs() {
    logText->setText("");
    logText->show();
}

void Client::enableLoginButton() {
    loginButton->setEnabled(!ipLineEdit->text().isEmpty() &&
                            !portLineEdit->text().isEmpty());
}

void Client::showLogs(QString logMsg) {
    logText->append(logMsg);
    // logText->show();
}

void Client::showMsg() {
    QPoint *pos = new QPoint;
    pos->setX(100);
    pos->setY(100);
    QToolTip::showText(*pos, "ddd");
}

void Client::proccessLoginResult(bool isOK) {
    if (isOK) {
        loginFlag = 1;
        loginButton->setText("退出");
        enablekeysButton();
        //  demand_status = 2;

        // QMessageBox::information(this, "OSD", "hahah");
    } else {
        //  demand_status = 0;
    }
}

void Client::proccessOSD(QString osdMsg) {
    QMessageBox::information(this, "OSD", osdMsg);
}

void Client::disablekeysButton() {
    irrButton0->setEnabled(false);
    irrButton1->setEnabled(false);
    irrButton2->setEnabled(false);
    irrButton3->setEnabled(false);
    irrButton4->setEnabled(false);
    irrButton5->setEnabled(false);
    irrButton6->setEnabled(false);
    irrButton7->setEnabled(false);
    irrButton8->setEnabled(false);
    irrButton9->setEnabled(false);
    irrButtonFF->setEnabled(false);
    irrButtonFR->setEnabled(false);
    irrButtonUpArrow->setEnabled(false);
    irrButtonDownArrow->setEnabled(false);
    irrButtonLeftArrow->setEnabled(false);
    irrButtonRightArrow->setEnabled(false);
    irrButtonOK->setEnabled(false);
    irrButtonReturn->setEnabled(false);
    irrButtonQuit->setEnabled(false);

    simulateKbButton->setEnabled(false);
    simulateMouseButton->setEnabled(false);
    simulateConsoleButton->setEnabled(false);
}

void Client::enablekeysButton() {
    irrButton0->setEnabled(true);
    irrButton1->setEnabled(true);
    irrButton2->setEnabled(true);
    irrButton3->setEnabled(true);
    irrButton4->setEnabled(true);
    irrButton5->setEnabled(true);
    irrButton6->setEnabled(true);
    irrButton7->setEnabled(true);
    irrButton8->setEnabled(true);
    irrButton9->setEnabled(true);
    irrButtonFF->setEnabled(true);
    irrButtonFR->setEnabled(true);
    irrButtonUpArrow->setEnabled(true);
    irrButtonDownArrow->setEnabled(true);
    irrButtonLeftArrow->setEnabled(true);
    irrButtonRightArrow->setEnabled(true);
    irrButtonOK->setEnabled(true);
    irrButtonReturn->setEnabled(true);
    irrButtonQuit->setEnabled(true);

    simulateKbButton->setEnabled(true);
    simulateMouseButton->setEnabled(true);
    simulateConsoleButton->setEnabled(true);
}

void Client::irrKeySend0() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM0); }

void Client::irrKeySend1() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM1); }

void Client::irrKeySend2() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM2); }

void Client::irrKeySend3() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM3); }

void Client::irrKeySend4() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM4); }

void Client::irrKeySend5() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM5); }

void Client::irrKeySend6() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM6); }

void Client::irrKeySend7() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM7); }

void Client::irrKeySend8() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM8); }

void Client::irrKeySend9() { irrKeySend(IPANEL_CLOUD_IRKEY_NUM9); }

void Client::irrKeySendFF() { irrKeySend(IPANEL_CLOUD_IRKEY_PAGE_DOWN); }

void Client::irrKeySendFW() { irrKeySend(IPANEL_CLOUD_IRKEY_PAGE_UP); }

void Client::irrKeySendUp() { irrKeySend(IPANEL_CLOUD_IRKEY_UP); }

void Client::irrKeySendDown() { irrKeySend(IPANEL_CLOUD_IRKEY_DOWN); }

void Client::irrKeySendLeft() { irrKeySend(IPANEL_CLOUD_IRKEY_LEFT); }

void Client::irrKeySendRight() { irrKeySend(IPANEL_CLOUD_IRKEY_RIGHT); }

void Client::irrKeySendOK() { irrKeySend(IPANEL_CLOUD_IRKEY_SELECT); }

void Client::irrKeySendReturn() { irrKeySend(IPANEL_CLOUD_IRKEY_BACK); }

void Client::irrKeySendQuit() { irrKeySend(IPANEL_CLOUD_IRKEY_EXIT); }

void PlugEvent::run() {
    // Client *pThread = static_cast<Client *>(p);
    int event_ret = -1;
    int event_id = -1;
    char event_value[4224];
    int event_value_size = -1;

    while (1) {
        BlcCore_memset(event_value, 0, sizeof(event_value));
        event_value_size = sizeof(event_value);

        event_ret = bbcv_plug_wait_events_plug(&event_id, event_value,
                                               &event_value_size, -1);
        if (event_ret < 0) {
            BlcCore_printf("%s %d blcplug_wait_events error, event_ret = %d!\n",
                           __FUNCTION__, __LINE__, event_ret);
            continue;
        }

        if (event_id == BBCV_PlugApi_CtrlID_PlayUnBlock_S2 ||
                event_id == BBCV_PlugApi_CtrlID_PlayUnBlock_P) {
            emit log("登陆成功：");
            QString logs(event_value);
            emit log(logs);
            emit sendStatus(true);
        } else if (event_id == BBCV_PlugApi_CtrlID_ReturnMw) {
            if (strncmp("fail", event_value, 4) == 0) {
                emit log("退出到直播\n");
                emit sendStatus(false);
            } else {
                emit log("进入中间件模式:");
                QString logs(event_value);
                emit log(logs);
            }
        } else if (event_id == BBCV_PlugApi_CtrlID_OSDDisplay) {
            QString osd(event_value);
            emit sendOSD(osd);
        } else if (event_id == BBCV_PlugApi_CtrlID_OSDDisplay) {
            // TODO
        } else {
            BlcCore_printf("%s %d unknow event_id = %d!\n", __FUNCTION__, __LINE__,
                           event_id);
        }
    }
}
