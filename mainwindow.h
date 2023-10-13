#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <map>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void(*winHiddenCallback)(std::string prompt);

    void EscapeWindow();

    std::map<int, std::vector<std::pair<std::string, std::string>>> installedApps;

public slots:
    void enter_pressed();
    void onTextChange(QString str);

private:
    void runExecutable();
    void resetStates();
    void loadApps();

    bool ApplicationMode = false;
    Ui::MainWindow *ui;

    std::string currentExecutablePath;
};
#endif // MAINWINDOW_H
