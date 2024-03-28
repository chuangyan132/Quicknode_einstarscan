#ifndef OPENCREATESLNDLG_H
#define OPENCREATESLNDLG_H

#include <QWidget>

class ScanServiceImp;

namespace Ui {
class OpenCreateSlnDlg;
}

class OpenCreateSlnDlg : public QWidget
{
    Q_OBJECT

public:
    explicit OpenCreateSlnDlg(QWidget *parent = 0);
    ~OpenCreateSlnDlg();
    // Is create project
    bool isCreate();
private:
    Ui::OpenCreateSlnDlg *ui;
    bool m_isCreate;
private slots:
    void on_btnNewWork_clicked(bool checked = false);
    void on_btnOpenWork_clicked(bool checked = false);
};

#endif // OPENCREATESLNDLG_H
