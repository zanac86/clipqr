#include "maindialog.h"
#include "ui_maindialog.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QClipboard>

#include "QRCodeGenerator.h"

QString trayMessage = "The program will keep running in the "
                      "system tray. To terminate the program, "
                      "choose <b>Quit</b> in the context menu "
                      "of the system tray entry.";


MainDialog::MainDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);


    updateQrCodeFromClipboard();

    createActions();
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &MainDialog::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainDialog::iconActivated);

}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::setVisible(bool visible)
{
    QDialog::setVisible(visible);
}

void MainDialog::on_lineEdit_textChanged(const QString& arg1)
{
    QString text = ui->lineEdit->text();
    if (text.isEmpty())
    {
        text = " ";
    }
    updateQrCode(text);
}

void MainDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            updateQrCodeFromClipboard();
            restoreAction->trigger();
            break;
        case QSystemTrayIcon::DoubleClick:
            showMessage();
            break;
        case QSystemTrayIcon::MiddleClick:
            showMessage();
            break;
        default:
            ;
    }
}

void MainDialog::showMessage()
{
    trayIcon->showMessage(
        "clipqr",
        trayMessage,
        QIcon(":/images/barcode_2d_32.png"),
        5000);

}

void MainDialog::messageClicked()
{

}

void MainDialog::closeEvent(QCloseEvent* event)
{
    //    if (trayIcon->isVisible())
    //    {
    //        QMessageBox::information(this, "clipqr", trayMessage);
    //        hide();
    //        event->ignore();
    //    }
    hide();
    event->ignore();
}

void MainDialog::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/images/barcode_2d_16.png");
    trayIcon->setIcon(icon);
    trayIcon->setToolTip(trayMessage);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void MainDialog::createActions()
{
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QIcon(":/images/door.png"));

    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainDialog::updateQrCode(QString text)
{
    CQR_Encode qr;
    int maxImageSize = 500;
    int normalImageSize = 400;
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    QString encodeString = text;

    bool okEncoding = qr.EncodeData(levelIndex, versionIndex, bExtent, maskIndex, encodeString.toUtf8().data());
    if (!okEncoding)
    {
        ui->label->clear();
        ui->label->setText(tr("QR ecoding failed\nText too long"));
        return;
    }

    int qrImageSize = qr.m_nSymbleSize;

    int encodeImageSize = qrImageSize + (QR_MARGIN * 2);
    QImage encodeImage(encodeImageSize, encodeImageSize, QImage::Format_Mono);
    encodeImage.fill(1);

    for (int i = 0; i < qrImageSize; i++)
        for (int j = 0; j < qrImageSize; j++)
            if (qr.m_byModuleData[i][j])
            {
                encodeImage.setPixel(i + QR_MARGIN, j + QR_MARGIN, 0);
            }

    if (encodeImageSize > maxImageSize)
    {
        ui->label->clear();
        ui->label->setText(tr("QR ecoding failed\nResult image too large"));
        return;
    }

    int scale = normalImageSize / encodeImageSize;
    if (scale == 0)
    {
        scale = 1;
    }

    int scale_size = encodeImageSize * scale;

    QPixmap p = QPixmap::fromImage(encodeImage);

    QPixmap scale_image = p.scaled(scale_size, scale_size);

    ui->label->setPixmap(scale_image);

}

void MainDialog::updateQrCodeFromClipboard()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    QString s = clipboard->text(QClipboard::Clipboard);
    if (!s.isEmpty())
    {
        ui->lineEdit->setText(s);
    }
}
