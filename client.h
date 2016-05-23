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

#ifndef CLIENT_H
#define CLIENT_H

#include <qbuttongroup.h>
#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <QTextEdit>
#include <QToolButton>
#include <QtWidgets>
#include <QThread>
#include <QToolTip>
#include <QPoint>

QT_BEGIN_NAMESPACE
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class PlugEvent : public QThread {
    Q_OBJECT

public:
    virtual void run();

signals:
    void log(QString logMsg);
    void sendStatus(bool isOK);
    void sendOSD(QString osdMsg);

private:
};

class Client : public QWidget {
    Q_OBJECT

public:
    Client(QWidget *parent = 0);

protected:
    void StbInit();
    void loadDefautConf();
    void updateStbConf();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *kEvent);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *kEvent);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

    void createButtonRightLayout();
    void createUpperRightLayout();
    void createUpperLeftLayout();
    void createButtonLeftLayout();
    void createLeftLayout();
    void createRightLayout();

    void showMsg();
private slots:
    void plugInit();
    void handleStbEvent();

    void showLogs(QString logMsg);
    void proccessLoginResult(bool isOK);
    void proccessOSD(QString osdMsg);

    void irrKeySend0();
    void irrKeySend1();
    void irrKeySend2();
    void irrKeySend3();
    void irrKeySend4();
    void irrKeySend5();
    void irrKeySend6();
    void irrKeySend7();
    void irrKeySend8();
    void irrKeySend9();
    void irrKeySendFF();
    void irrKeySendFW();
    void irrKeySendUp();
    void irrKeySendDown();
    void irrKeySendLeft();
    void irrKeySendRight();
    void irrKeySendOK();
    void irrKeySendReturn();
    void irrKeySendQuit();

    void simulateKeyboard();
    void simulateMouse();
    void simulateConsole();
    void clearLogs();

private:
    void loginCloud();
    void logoutCloud();
    void keySend();
    void irrKeySend(int key);

    void enableLoginButton();
    void enablekeysButton();
    void disablekeysButton();

    // left - param set
    QLabel *hostLabel;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;

    QLabel *termidLabel;
    QLineEdit *termidLineEdit;

    QLabel *regionIdLabel;
    QLineEdit *regionIdLineEdit;

    // left - run log
    QTextEdit *logText;

    // right - login
    QPushButton *poweronButton;
    QPushButton *loginButton;

    // right - irr keys
    QPushButton *irrButton1;
    QPushButton *irrButton2;
    QPushButton *irrButton3;
    QPushButton *irrButton4;
    QPushButton *irrButton5;
    QPushButton *irrButton6;
    QPushButton *irrButton7;
    QPushButton *irrButton8;
    QPushButton *irrButton9;
    QPushButton *irrButton0;

    QPushButton *irrButtonFF;
    QToolButton *irrButtonUpArrow;
    QPushButton *irrButtonFR;
    QToolButton *irrButtonLeftArrow;
    QPushButton *irrButtonOK;
    QToolButton *irrButtonRightArrow;
    QToolButton *irrButtonDownArrow;
    QPushButton *irrButtonReturn;
    QPushButton *irrButtonQuit;

    // right - keyborad mouse console keys
    QPushButton *simulateKbButton;
    QPushButton *simulateMouseButton;
    QPushButton *simulateConsoleButton;
    QPushButton *clearLogButton;

    QGridLayout *upperLeftLayout;
    QGridLayout *bottomLeftLayout;
    QVBoxLayout *leftLayout;
    QGridLayout *upperRightLayout;
    QGridLayout *bottomRightLayout;
    QVBoxLayout *rightLayout;

    int poweronFlag;
    int loginFlag;
    int simulateKbFlag;
    int simulateMouseFlag;
    int simulateConsoleFlag;

    PlugEvent plugEventThread;
};

#endif
