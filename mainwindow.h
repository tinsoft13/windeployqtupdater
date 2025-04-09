#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QFrame>
#include <QProgressBar>
#include <QGroupBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectFile();
    void deployDlls();

private:
    void setupUI();
    void setupStyle();
    
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QGroupBox *mainGroup;
    QVBoxLayout *groupLayout;
    QHBoxLayout *fileLayout;
    QLineEdit *filePathEdit;
    QPushButton *selectButton;
    QPushButton *deployButton;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    QString selectedFilePath;
};
#endif // MAINWINDOW_H
