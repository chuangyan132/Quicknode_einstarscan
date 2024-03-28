#include "opencreateslndlg.h"
#include "ui_opencreateslndlg.h"
#include "qfiledialog.h"
#include "qjsonobject.h"
#include "qjsondocument.h"
//#include "ScanServiceImp.h"

OpenCreateSlnDlg::OpenCreateSlnDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenCreateSlnDlg),
    m_isCreate(false)
{
    ui->setupUi(this);
}

OpenCreateSlnDlg::~OpenCreateSlnDlg()
{
    delete ui;
}

// Is create project
bool OpenCreateSlnDlg::isCreate()
{
    return m_isCreate;
}

void OpenCreateSlnDlg::on_btnNewWork_clicked(bool checked)
{
    m_isCreate = true;
    ((QDialog*)(this->topLevelWidget()))->accept();
}

void OpenCreateSlnDlg::on_btnOpenWork_clicked(bool checked)
{
    m_isCreate = false;
    ((QDialog*)(this->topLevelWidget()))->accept();
}