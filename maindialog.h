#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>

namespace Ui
{
    class MainDialog;
}

class MainDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit MainDialog(QWidget* parent = nullptr);
        ~MainDialog();

        void setVisible(bool visible) override;

    private slots:
        void on_lineEdit_textChanged(const QString& arg1);

        void iconActivated(QSystemTrayIcon::ActivationReason reason);
        void showMessage();
        void messageClicked();

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        Ui::MainDialog* ui;

        void createTrayIcon();
        void createActions();
        void updateQrCode(QString text);

        void updateQrCodeFromClipboard();

        QSystemTrayIcon* trayIcon;
        QMenu* trayIconMenu;
        QAction* quitAction;
        QAction* restoreAction;
        QAction* saveJpgeAction;
        QAction* savePngAction;



};

#endif // MAINDIALOG_H
