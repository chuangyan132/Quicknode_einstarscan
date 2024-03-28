#include "sn3ddialogback.h"
#include "ui_sn3ddialogback.h"

Sn3DDialogBack::Sn3DDialogBack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sn3DDialogBack)
{
    setWindowFlags (Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);

    isDraggingWindow =false;
    m_content = ui->widget_content;
}

Sn3DDialogBack::~Sn3DDialogBack()
{
    delete ui;
}

void Sn3DDialogBack::mouseReleaseEvent( QMouseEvent *event )
{
    isDraggingWindow = false;
}

void Sn3DDialogBack::hideEvent(QHideEvent *event)
{
	int type = event->type();
	emit sigHide();
	QWidget::hideEvent(event);
}

void Sn3DDialogBack::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape )
	{
		if (!m_content->isEnabled())
		{
			event->ignore();
			return;
		}
		else
		{
			event->accept();
		}
	}
	QDialog::keyPressEvent(event);
}

void Sn3DDialogBack::setWidgetContent(QWidget *content)
{
	m_content->setParent(nullptr);
	QSize size = QSize(content->width(), content->height()+50);
	this->resize(size);
	ui->widget_back->layout()->addWidget(content);
	content->show();
	m_content = content;
}

void Sn3DDialogBack::setTitleText(QString title)
{
    ui->widget_top->setText(title);
}
void Sn3DDialogBack::setOffsetY(int offset)
{
}


void Sn3DDialogBack::mouseMoveEvent( QMouseEvent *event )
{
    if(isDraggingWindow) {
        this->move(event->globalPos()-lastMousePos);
    }
}

void Sn3DDialogBack::mousePressEvent( QMouseEvent *event )
{
    /*if(isMaxWindow) {
        return;
    }*/
    // current
    int x = event->pos().x();
    int y = event->pos().y();
    // top left
    int x1 = ui->widget_top->pos().x();
    int y1 = ui->widget_top->pos().y();
    // bottom right
    int x2 = x1 + ui->widget_top->width();
    int y2 = y1 + ui->widget_top->height();
    if(x > x1 && x < x2 && y > y1 && y < y2) {
        lastMousePos = event->pos();
        isDraggingWindow = true;
    }
    /*if(phoneDlg && !phoneDlg->isHidden()){
    ui->widget_phone->setStyleSheet("#widget_phone{background-color:none;border:none;}");
    phoneDlg->hide();
    }*/
}

