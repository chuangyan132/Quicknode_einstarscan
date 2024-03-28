 #include "newopenprojdialog.h"
#include "ui_newopenprojdialog.h"
#include "qfiledialog.h"

NewOpenProjDialog::NewOpenProjDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewOpenProjDialog)
{
    ui->setupUi(this);
}

NewOpenProjDialog::~NewOpenProjDialog()
{
    delete ui;
}

void NewOpenProjDialog::setProjExt(const QString& projExt)
{
    m_strProjExt = projExt;
}

void NewOpenProjDialog::on_btnNewProject_clicked(bool checked)
{
    m_isCreate = true;
    this->accept();
}

void NewOpenProjDialog::on_btnOpenProject_clicked(bool checked)
{
    m_isCreate = false;
    /*QFileDialog dlg(this);
    dlg.setNameFilter("project files (*." + m_strProjExt + ")");
    QString projectFile = dlg.getOpenFileName(this, "open project");
    if (!projectFile.isEmpty()){
        m_strProjectFile = projectFile;
        this->accept();
    }*/
    this->accept();
}

bool NewOpenProjDialog::isCreate()
{
    return m_isCreate;
}
