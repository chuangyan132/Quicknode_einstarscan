#include "sn3dfiledialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QRegExp>

file_name_validator::file_name_validator(const QRegExp& rx, QObject *parent) :QRegExpValidator(rx, parent)
{

}
void file_name_validator::fixup(QString & s) const
{
	QRegExpValidator::fixup(s);

}
QValidator::State file_name_validator::validate(QString& input, int& pos) const
{
	QValidator::State state = QRegExpValidator::validate(input, pos);
	int char_pos = input.indexOf("\\");
	if (state == Invalid || char_pos >= 0)
	{
		state = Invalid;
		QWidget* parent = dynamic_cast<QWidget*>(this->parent());
		QRect rt = parent->rect();

		int width = rt.width();
		int height = rt.height();

		QString msg = tr("File name cannot contain any of following characters:\n"
			"                     \\/:*?\"<>|");
		QPoint  left_top;
		left_top.setX(width / 4);
		left_top.setY(height);

		left_top = parent->mapToGlobal(left_top);

		rt.setTopLeft(left_top);
		rt.setHeight(60);
		rt.setWidth(200);
		QToolTip::showText(left_top, msg, parent, rt, 1000);
	}

	return state;
}

sn3d_file_dialog::sn3d_file_dialog(QString protocol, QWidget *parent) :QFileDialog(parent)
    {

    /*	QStringList dir_lsit;
        dir_lsit << "e:\\" << "f:\\";*/

        this->setObjectName("file_dialog_save");
        this->resize(650, 560);
           file_name_edit = nullptr;
//           std::string last_dir = SysConfig::getSysConfig().get<std::string>("config.last_prj_save_path","");// ("config.last_save_path", "");
           QString str_dir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

           setAttribute(Qt::WA_AlwaysStackOnTop, true);


           setDirectory(str_dir);
        //   dir_lsit << str_dir;

            //1 判断当前扫描类型
            //int scan_type_ = SysConfig::getSysConfig().get<size_t>("config.common_para_config.scan_type",0);
           /// int stripe_scan_type_ = SysConfig::getSysConfig().get<int>("config.common_para_config.stripe_scan_type", 1);
            //2 判断当前是否有网格模型
          //  int mesh_type_ = SysConfig::getSysConfig().get<size_t>("config.common_para_config.have_mesh_type", 1);
            //3 当前的纹理情况
           // int scan_texture_type_ = SysConfig::getSysConfig().get<size_t>("config.common_para_config.scan_texture_type", 1);
		   int flags = Qt::Dialog;
		   flags |= (int)(Qt::WindowCloseButtonHint); //WindowContextHelpButtonHint
		   this->setWindowFlags((Qt::WindowFlags)flags);

            this->setOption(QFileDialog::DontUseNativeDialog , true);
            if (this->testOption(QFileDialog::DontUseNativeDialog))
                printf("cuccee\n");

            this->setAcceptMode(QFileDialog::AcceptSave );
            this->setViewMode(QFileDialog::List);
            this->selectFile(tr("Scan"));
        //	this->setFileMode(QFileDialog::AnyFile);
            //dialog_save_->setNameFilter(tr("(*.asc);;(*.stl);;(*.obj);;(*.ply)"));
            QGridLayout *grid_layout = static_cast<QGridLayout*>( this->layout());
            QHBoxLayout* layout_type_ = new  QHBoxLayout;

            initCheckbox(protocol);
          //  SysConfig::getSysConfig().put("config.common_para_config.save_type_3mf", false);
           /* if (1 == mesh_type_){
                box_3_->setDisabled(false);
                //box_2_->setDisabled(true);
                box_2_->setChecked(true);
                box_3MF_->setEnabled(true);
                box_0_->setDisabled(false);
                box_5_->setDisabled(false);
                box_5_->setChecked(false);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_asc",  false);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_sasc", false);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_stl",  true);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_ply",  false);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_obj",  false);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_p3",   false);
                SysConfig::getSysConfig().put("config.common_para_config.save_type_3mf", false);
                if (1 == scan_texture_type_)
                {
                    box_4_->setChecked(true);
                    SysConfig::getSysConfig().put("config.common_para_config.save_type_obj", true);
                    box_2_->setChecked(false);
                    SysConfig::getSysConfig().put("config.common_para_config.save_type_stl", false);
                }

                */
            layout_type_->addWidget(box_0_);
            layout_type_->addWidget(box_1_);
            layout_type_->addWidget(box_2_);
            layout_type_->addWidget(box_3_);
            layout_type_->addWidget(box_4_);
            layout_type_->addWidget(box_5_);
            layout_type_->addWidget(box_3MF_);

            QObject::connect(box_3MF_, SIGNAL(toggled(bool)), SLOT(check_box_update_3mf(bool)));
            QObject::connect(box_0_, SIGNAL(toggled(bool)), SLOT(check_box_update_asc(bool)));
            QObject::connect(box_1_, SIGNAL(toggled(bool)), SLOT(check_box_update_sasc(bool)));
            QObject::connect(box_2_, SIGNAL(toggled(bool)), SLOT(check_box_update_stl(bool)));
            QObject::connect(box_3_, SIGNAL(toggled(bool)), SLOT(check_box_update_ply(bool)));
            QObject::connect(box_4_, SIGNAL(toggled(bool)), SLOT(check_box_update_obj(bool)));
            QObject::connect(box_5_, SIGNAL(toggled(bool)), SLOT(check_box_update_p3(bool)));



            int rowCount = grid_layout->rowCount();
            int colCount = grid_layout->columnCount();
            for (int row = 0; row <= rowCount; row++)
            {
                for (int col = 0; col < colCount; col++)
                {
                    QLayoutItem* item = grid_layout->itemAtPosition(row, col);
                    if (!item) continue;
                    QWidget* pwidget = item->widget();

                    if (pwidget && file_name_edit==nullptr)
                    {


                        file_name_edit = qobject_cast<QLineEdit*>(pwidget);
                        if (file_name_edit != NULL)
                        {
                            QRegExp rx("[^/\\:*?\"<>|]*");
                            file_name_validator* validator = new file_name_validator(rx, file_name_edit);
                            file_name_edit->setValidator(validator);
                        }
                    }
                }
            }
            grid_layout->addLayout(layout_type_, rowCount, 0, 1, colCount, Qt::AlignBottom | Qt::AlignCenter);

            str_save = this->labelText(QFileDialog::Accept);
            this->setLabelText(QFileDialog::Reject, tr("Cancel"));

          //  changeNameFillter();

            QList<QUrl> urls;
            QString file_name;
            QUrl mycomputer;
            mycomputer.setUrl("file:");
            urls << mycomputer;
            urls << QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::HomeLocation, file_name, QStandardPaths::LocateDirectory));
            urls << QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::DesktopLocation, file_name, QStandardPaths::LocateDirectory));
            this->setSidebarUrls(urls);

            //this->setStyleSheet("background-color: red; border: none;");
        }//wan

void sn3d_file_dialog::check_box_update_asc(bool change_)
{
     //m_checkedsState[0] = change_;
     m_checkedsState[0] = change_ ? '1' : '0';
}

void sn3d_file_dialog::check_box_update_sasc(bool change_)
{
     m_checkedsState[1] = change_ ? '1' : '0';
}

void sn3d_file_dialog::check_box_update_stl(bool change_)
{
     m_checkedsState[2] = change_ ? '1' : '0';
}

void sn3d_file_dialog::check_box_update_ply(bool change_)
{
    qDebug() << "m_checkedsState = " << m_checkedsState;
     m_checkedsState[3] = change_ ? '1' : '0';

    qDebug() << "m_checkedsState = " << m_checkedsState;
}

void sn3d_file_dialog::check_box_update_obj(bool change_)
{
     m_checkedsState[4] = change_ ? '1' : '0';
}

void sn3d_file_dialog::check_box_update_p3(bool change_)
{
    m_checkedsState[5] = change_ ? '1' : '0';
}

void sn3d_file_dialog::check_box_update_3mf(bool change_)
{
     m_checkedsState[6] = change_ ? '1' : '0';
}

void sn3d_file_dialog::initCheckbox(QString protol)
{
         qDebug() << "initCheckbox =" << protol;
            if(protol.length() != 21)
                return;
                m_checkedsState = "0000000";
                 box_0_ = new QCheckBox;
                box_0_->setText(tr(".asc(whole)"));
                box_0_->setVisible((protol[0] == '1'));               
                box_0_->setEnabled((protol[1] == '1'));
				box_0_->setChecked((protol[2] == '1'));
                m_checkedsState[0] = box_0_->isChecked() ? '1' : '0';

                 box_1_ = new QCheckBox;
                box_1_->setText(tr(".asc(separate)"));
				box_1_->setVisible((protol[3] == '1'));
				box_1_->setEnabled((protol[4] == '1'));
				box_1_->setChecked((protol[5] == '1'));
                m_checkedsState[1] = box_1_->isChecked()? '1' : '0';


                 box_2_ = new QCheckBox;
                box_2_->setText(tr(".stl"));
				box_2_->setVisible((protol[6] == '1'));
				box_2_->setEnabled((protol[7] == '1'));
				box_2_->setChecked((protol[8] == '1'));
                 m_checkedsState[2] = box_2_->isChecked()? '1' : '0';

                 box_3_ = new QCheckBox;
                box_3_->setText(tr(".ply"));
				box_3_->setVisible((protol[9] == '1'));
				box_3_->setEnabled((protol[10] == '1'));
				box_3_->setChecked((protol[11] == '1'));
                 m_checkedsState[3] = box_3_->isChecked()? '1' : '0';

                 box_4_ = new QCheckBox;
                box_4_->setText(tr(".obj"));

				box_4_->setVisible((protol[12] == '1'));
				box_4_->setEnabled((protol[13] == '1'));
				box_4_->setChecked((protol[14] == '1'));
                 m_checkedsState[4] = box_4_->isChecked()? '1' : '0';

                box_5_ = new QCheckBox;
                box_5_->setText(tr(".p3"));

				box_5_->setVisible((protol[15] == '1'));
				box_5_->setEnabled((protol[16] == '1'));
				box_5_->setChecked((protol[17] == '1'));
                 m_checkedsState[5] = box_5_->isChecked()? '1' : '0';

                box_3MF_ = new QCheckBox;
                box_3MF_->setText(tr(".3mf"));
				box_3MF_->setVisible((protol[18] == '1'));
				box_3MF_->setEnabled((protol[19] == '1'));
				box_3MF_->setChecked((protol[20] == '1'));
                 m_checkedsState[6] = box_3MF_->isChecked()? '1' : '0';

       
				 


}//wan

QString sn3d_file_dialog::selectedFile()
{
    return this->selectedFiles()[0];
}

QString sn3d_file_dialog::getCheckedsState()
{
    return m_checkedsState;
}

sn3d_file_dialog::~sn3d_file_dialog()
{

}

void  sn3d_file_dialog::accept()
    {
    if (!box_0_->isChecked() &&
            !box_1_->isChecked() &&
            !box_2_->isChecked() &&
            !box_3_->isChecked() &&
            !box_4_->isChecked() &&
            !box_5_->isChecked() &&
            !box_3MF_->isChecked()
            )
        {
            QMessageBox::information(NULL, tr(""), tr("Please select file type"), tr("OK"));
            return;
        }

        QStringList s = this->selectedFiles();
        if (s.size() <= 0)
            return;
        int index = s[0].indexOf("/");
        if (index == 0){
            QMessageBox::warning(this, tr("Save"), tr("Unable to save to this location. Please select an alternative location."), tr("OK"));
            return;
        }
        qDebug() <<"selectedFiles:"<< s;
        QString str_accept = this->labelText(QFileDialog::Accept);
       // std::cout << "\n save= " << str_save.toStdString().c_str() << "  accept=" << str_accept.toStdString().c_str() << "\n";
        if (str_save == str_accept)
        {
            //std::cout << "\n str_save=str_accept " << "\n";
             QString sDir = selectedFile();
             QString  file_name = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
             QString test_file = sDir + file_name;
             QFile file(test_file);
             if (!file.open(QFile::WriteOnly))
             {

                QMessageBox::warning(this, tr("Save"), tr("You can't save the file here.\nPlease reselect."), tr("OK"));
                return;
             }
             file.close();
             file.remove();

			 bool save_type_obj = box_4_->isChecked();
             if (save_type_obj)//校验文件名字只能有数字和字母
             {
                 QString file_name = this->selectedFile();
                 int pos=file_name.lastIndexOf("\\");
                 if (pos<0)
                     pos = file_name.lastIndexOf("/");
                 if (pos>0)
                     file_name = file_name.right(file_name.length()-pos-1);


                 //QRegExp rxtt("[-_0-9a-zA-Z.]*");

                 //bool match = rxtt.exactMatch(file_name);
                // bool match=file_name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
                 bool match=file_name.contains(QRegExp("^[A-Za-z0-9_-]{1,256}$"));
                 if (!match)
                 {

                     QMessageBox::warning(this, tr("Save"), tr("Only letters and number allowed in .obj file name."), tr("OK"));
                     return;
                 }

				 bool match1 = file_name.contains(" ");
                 if (match1) // 不能带空格
				 {
                     QMessageBox::warning(this, tr("Save"), tr("No Chinese or Space allowed in .obj file name."), tr("OK"));
					 return;
				 }

             }

        }
//        if()//在扫描界面
//            {} //工程数组
        bool isExists = false;
		 
        if(m_fixList.size() > 0)
            isExists = isExistsSameFiles(m_fixList);
        else
            isExists = isExistsSameFiles(m_projList);//m_projList

        if(isExists)
        {
            QMessageBox msg(QMessageBox::Information, tr("save"), tr("File already exists.\nDo you want to replace it?"), QMessageBox::NoButton, 0, (Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint));
            msg.setModal(true);
            msg.addButton(tr("Yes"), QMessageBox::YesRole);
            msg.addButton(tr("No"), QMessageBox::NoRole);
            int ret = msg.exec();
            if (0 != ret){//no
                return;
            }
        }
        QFileDialog::accept();
    }

void sn3d_file_dialog::setScanWithTexture(bool isTexture)
{
	 m_withTexture = isTexture;
}




bool sn3d_file_dialog::isExistsSameFile(QString proname)
{
    bool isHas = false;
    QString selectf;
    if(proname == "")
    {
        selectf = selectedFile();
        if(selectf.contains(".asc") || selectf.contains(".p3"));
        {
           selectf = selectf.remove(QRegExp(".asc$"));
           //selectf = selectf.remove(QRegExp(".p3"));
        }
    }
    else
    {
        selectf = selectedFile();
        if(selectf.contains(".asc") || selectf.contains(".p3"));
        {
            selectf =selectf.remove(QRegExp(".asc$"));
           //selectf = selectf.remove(QRegExp(".p3"));
        }
        selectf = selectf + "_" + proname;
    }
    qDebug() << "selectfile = " << selectf;
    if(box_0_->isChecked() && box_0_->isVisible())
    {
        QString filepath = selectf + ".asc";
        qDebug() << "filepath = " << filepath;
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }else if(box_1_->isChecked() && box_1_->isVisible())
    {
        QString filepath = selectf + ".asc";
        qDebug() << "filepath = " << filepath;
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }else if(box_2_->isChecked() && box_2_->isVisible())
    {
        QString filepath = selectf + ".stl";
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }
    else if(box_3_->isChecked() && box_3_->isVisible())
    {
        QString filepath = selectf + ".ply";
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }
    else if(box_4_->isChecked() && box_4_->isVisible())
    {
        QString filepath = selectf + ".obj";
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }
    else if(box_5_->isChecked() && box_5_->isVisible())
    {
        QString filepath = selectf + ".p3";
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }
    else if(box_3MF_->isChecked() && box_3MF_->isVisible())
    {
        QString filepath = selectf + ".3MF";
        if(QFile::exists(filepath))
        {
            isHas = true;
        }

    }

    return isHas;

}

bool sn3d_file_dialog::isExistsSameFiles(QVariantList listname)
{
    if(m_isAsc)
    {
         qDebug() << "sign 1120 isExistsSameFiles start listname.count()=" << listname.count() ;
        for(int i = 0 ; i < listname.count(); i++)
        {
            QString pathname = listname.at(i).toString();
            int index1 = pathname.lastIndexOf(".");
            int index2 = pathname.lastIndexOf("/");
            QString name = pathname.mid(index2 + 1, index1 - index2 - 1);
            qDebug() << "sign 1113 save name =" << name;

            if(isExistsSameFile(name))
            {
                return true;
            }
        }

    }
    else
    {

        if(isExistsSameFile())
        {
            return true;
        }
    }
    return false;
}

void sn3d_file_dialog::setSavePara(bool isAsc, QVariantList proList, QVariantList fixList)
{
    m_isAsc = isAsc;
    m_projList.clear();
    m_fixList.clear();
    for(int i = 0; i < proList.size(); ++i)
    {
        m_projList.push_back(proList.at(i));
    }

    if(fixList.size() > 0)
    {

        for(int j = 0; j < fixList.size(); ++j)
        {
            qDebug() << "sign 1120 fixList=" << fixList.at(j);
            m_fixList.push_back(fixList.at(j));
        }
    }

}
