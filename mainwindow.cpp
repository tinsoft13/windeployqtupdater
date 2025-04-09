#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include <QPalette>
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupStyle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    mainGroup = new QGroupBox("Qt DLL Deployer", this);
    mainGroup->setMinimumWidth(500);
    groupLayout = new QVBoxLayout(mainGroup);
    groupLayout->setSpacing(15);

    // File selection area
    QFrame *fileFrame = new QFrame(this);
    fileFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    fileLayout = new QHBoxLayout(fileFrame);
    fileLayout->setContentsMargins(10, 5, 10, 5);

    filePathEdit = new QLineEdit(this);
    filePathEdit->setReadOnly(true);
    filePathEdit->setPlaceholderText("Выберите исполняемый файл...");
    fileLayout->addWidget(filePathEdit);

    selectButton = new QPushButton("Обзор...", this);
    selectButton->setFixedWidth(80);
    fileLayout->addWidget(selectButton);

    groupLayout->addWidget(fileFrame);

    // Progress area
    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    groupLayout->addWidget(progressBar);

    // Status label
    statusLabel = new QLabel("Готов к работе", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    groupLayout->addWidget(statusLabel);

    // Deploy button
    deployButton = new QPushButton("Установить DLL", this);
    deployButton->setEnabled(false);
    groupLayout->addWidget(deployButton);

    mainLayout->addWidget(mainGroup);

    connect(selectButton, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(deployButton, &QPushButton::clicked, this, &MainWindow::deployDlls);

    setWindowTitle("Qt DLL Deployer");
    resize(600, 300);
}

void MainWindow::setupStyle()
{
    // Устанавливаем современный стиль
    qApp->setStyle(QStyleFactory::create("Fusion"));

    // Настраиваем цветовую схему
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    // Стилизация кнопок
    QString buttonStyle = "QPushButton {"
                         "background-color: #2a82da;"
                         "border: none;"
                         "color: white;"
                         "padding: 8px 16px;"
                         "border-radius: 4px;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: #1a72ca;"
                         "}"
                         "QPushButton:disabled {"
                         "background-color: #666666;"
                         "}";

    selectButton->setStyleSheet(buttonStyle);
    deployButton->setStyleSheet(buttonStyle);

    // Стилизация прогресс-бара
    progressBar->setStyleSheet(
        "QProgressBar {"
        "border: 2px solid #2a82da;"
        "border-radius: 5px;"
        "text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "background-color: #2a82da;"
        "}"
    );
}

void MainWindow::selectFile()
{
    selectedFilePath = QFileDialog::getOpenFileName(this, "Выберите исполняемый файл", "", "Executable Files (*.exe)");
    if (!selectedFilePath.isEmpty()) {
        filePathEdit->setText(selectedFilePath);
        deployButton->setEnabled(true);
        statusLabel->setText("Файл выбран. Готов к установке DLL.");
    }
}

void MainWindow::deployDlls()
{
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите файл");
        return;
    }

    statusLabel->setText("Установка DLL файлов...");
    progressBar->setValue(25);

    QFileInfo fileInfo(selectedFilePath);
    QString directory = fileInfo.absolutePath();
    QString program = "windeployqt";
    QStringList arguments;
    arguments << "--no-translations" << "--no-system-d3d-compiler" << "--no-opengl-sw" << selectedFilePath;

    QProcess process;
    process.setWorkingDirectory(directory);
    process.start(program, arguments);
    
    progressBar->setValue(50);

    if (!process.waitForFinished()) {
        progressBar->setValue(0);
        statusLabel->setText("Ошибка: Не удалось запустить windeployqt");
        QMessageBox::critical(this, "Ошибка", "Не удалось запустить windeployqt");
        return;
    }

    progressBar->setValue(75);

    if (process.exitCode() == 0) {
        progressBar->setValue(100);
        statusLabel->setText("DLL файлы успешно установлены!");
        QMessageBox::information(this, "Успех", "DLL файлы успешно установлены");
    } else {
        progressBar->setValue(0);
        statusLabel->setText("Ошибка при установке DLL файлов");
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при установке DLL файлов");
    }
}
