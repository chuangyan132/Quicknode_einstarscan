#ifndef NEWOPENPROJDIALOG_H
#define NEWOPENPROJDIALOG_H

#include <QDialog>

namespace Ui {
class NewOpenProjDialog;
}

class NewOpenProjDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewOpenProjDialog(QWidget *parent = 0);
    ~NewOpenProjDialog();
    void setProjExt(const QString& projExt);
    QString projectFile();
    bool isCreate();
private:
    Ui::NewOpenProjDialog *ui;
    QString m_strProjExt;
    QString m_strProjectFile;
    bool m_isCreate = false;
private slots:
    void on_btnNewProject_clicked(bool checked = false);
    void on_btnOpenProject_clicked(bool checked = false);
};

#endif // NEWOPENPROJDIALOG_H
