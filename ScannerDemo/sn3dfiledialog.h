#ifndef __SN3DFILEDIALOG_H__
#define __SN3DFILEDIALOG_H__
#include  <QObject>
#include  <QFileDialog>
#include <QStandardPaths>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QToolTip>

class file_name_validator :public  QRegExpValidator
{
	Q_OBJECT
public:
	explicit file_name_validator(const QRegExp& rx, QObject *parent = 0);

	virtual QValidator::State validate(QString& input, int& pos) const;
	virtual void fixup(QString &) const;
};

class sn3d_file_dialog : public QFileDialog
    {
        Q_OBJECT

    public:
        sn3d_file_dialog(QString protocol, QWidget *parent = 0);
		~sn3d_file_dialog();
        QString selectedFile();
        QString getCheckedsState();
		void setScanWithTexture(bool isTexture);

        bool isExistsSameFile(QString proname = "");

        bool isExistsSameFiles(QVariantList listname);

        void setSavePara(bool isAsc, QVariantList proList, QVariantList fixList);

    protected:
        void  accept();
    protected slots:
        void check_box_update_asc(bool change_);
        void check_box_update_sasc(bool change_);
        void check_box_update_stl(bool change_);
        void check_box_update_ply(bool change_);
        void check_box_update_obj(bool change_);
        void check_box_update_p3(bool change_);
        void check_box_update_3mf(bool change_);
    /*	void currentDirChanged(const QString &path);
        void currentfilesSelected(const QStringList &files);
        void currentfilesSelected2(const QUrl &files);*/

    private:
		void initCheckbox(QString protol);
        QCheckBox* box_0_; //ascw
        QCheckBox* box_1_; //ascs
        QCheckBox* box_2_; //stl
        QCheckBox* box_3_; //ply
        QCheckBox* box_4_; //obj
        QCheckBox* box_5_;  //p3
        QCheckBox* box_3MF_;//3MF
        QString    str_save;
        QLineEdit* file_name_edit;
        QString m_checkedsState;
		bool m_withTexture = false;
        QVariantList m_projList;
        QVariantList m_fixList;
        bool m_isAsc = true;




    };




#endif
