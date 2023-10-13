#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPainterPath>
#include <iostream>
#include <fstream>
#include <processthreadsapi.h>
#include <sstream>

#include <windows.h>
#include <stdlib.h>
#include <assert.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::enter_pressed);
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::onTextChange);
    ui->lineEdit->setFocus();
    setWindowState(windowState() | Qt::WindowActive);

    uint radius = 20;
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, width(),height()), radius, radius);
    ui->lineEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);

    QRegion region(path.toFillPolygon().toPolygon());

    setMask(region);
    setStyleSheet("MainWindow {border: 8px solid #66a5ad;  border-radius: 20px; background-color: #003b46;}");
    ui->lineEdit->setStyleSheet("QLineEdit {border: none; border-radius: 20px; background-color: #003b46}");
    //setStyleSheet(".QFrame{border: 10px solid white; border-radius: 20px;}");
    loadApps();

}

void MainWindow::loadApps(){
    // Datei komplett einlesen:
    std::ifstream f("applocations.conf");
    std::stringstream buffer;
    buffer << f.rdbuf();


    // Datenstruktur aufbauen mit den gelesenen Daten:
    std::string name, path;
    while (std::getline(buffer, name)) {
        std::getline(buffer, path);
        std::cout << "Program: " << name << "\n\t" << path << std::endl;
        if (installedApps.find(std::tolower(name[0])) != installedApps.end()) {
            installedApps[std::tolower(name[0])].push_back({ name, path });
        }
        else {
            installedApps.insert(std::pair<int, std::vector<std::pair<std::string, std::string>>>(std::tolower(name[0]), { {name, path} }));
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enter_pressed(){
    std::cout << "enter pressed" << std::endl;
    this->hide();
    if (ApplicationMode && !currentExecutablePath.empty()) runExecutable();
    winHiddenCallback(this->ui->lineEdit->text().toStdString());
    resetStates();
    return;
}

void MainWindow::onTextChange(QString str){
    static uint programIndex        = 0;
    static int  lastPressedLetter   = 0;
    std::cout << "text changed" << std::endl;
    if (ApplicationMode){
        // gucken ob überhaupt ein neuer Buchstabe gedrückt wurde, weil event wird auch ausgelöst, wenn wir den text vom code aus ändern.
        if (lastPressedLetter != 0 && "Anwendung: " + QString::fromStdString(installedApps[lastPressedLetter][programIndex].first) == str) return;
        int pressedLetter = std::tolower(str.toStdString().back());
        if (installedApps.find(pressedLetter) == installedApps.end()){ // falls kein Programm mit diesem Anfangsbuchstaben bekannt ist:
            ui->lineEdit->setText("Anwendung: "); // getippten buchstaben wieder löschen
        }
        else{
            // falls buchstabe wiederholt gedrückt wurde und es noch mehr anwendungen anzuzeigen gibt.
            if (installedApps[pressedLetter].size() > 1 && programIndex < installedApps[pressedLetter].size()-1 && pressedLetter == lastPressedLetter){
                programIndex ++;
            }
            else{
                //ansonsten zeige einfach die erste bekannte Anwendung mit dem Anfangsbuchstaben an.
                programIndex        = 0;
                lastPressedLetter   = pressedLetter;
            }
            std::string app = installedApps[pressedLetter][programIndex].first;
            std::cout << "selected app: " << app << std::endl;
            ui->lineEdit->setText("Anwendung: " + QString::fromStdString(app));
            currentExecutablePath = installedApps[pressedLetter][programIndex].second;
        }

        return;
    }

    if (str == "y") ui->lineEdit->setText("YouTube: ");
    if (str == "w") ui->lineEdit->setText("Web-Suche: ");
    if (str == "d") ui->lineEdit->setText("Downloads: ");
    if (str == "a"){
        ui->lineEdit->setText("Anwendung: ");
        ApplicationMode = true;
    }
}


void MainWindow::runExecutable(){
    std::cout << "executing: " << this->currentExecutablePath << std::endl;
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;
    std::wstring cmd(currentExecutablePath.begin(), currentExecutablePath.end());
    ShellExecute(NULL, L"open", cmd.c_str(), NULL, NULL, SW_SHOWDEFAULT);

}

void MainWindow::EscapeWindow(){
    QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
    resetStates();
    return;
}

void MainWindow::resetStates(){
    ApplicationMode = false;
    QMetaObject::invokeMethod(ui->lineEdit, "clear", Qt::QueuedConnection);
    currentExecutablePath = "";
}
