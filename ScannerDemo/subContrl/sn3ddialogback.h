#ifndef SN3DDIALOGBACK_H
#define SN3DDIALOGBACK_H

#include <QDialog>
#include <QMouseEvent>
namespace Ui {
class Sn3DDialogBack;
}

class Sn3DDialogBack : public QDialog
{
    Q_OBJECT

public:
    explicit Sn3DDialogBack(QWidget *parent = 0);
    ~Sn3DDialogBack();

    void setWidgetContent(QWidget* content);
    void setTitleText(QString title);
	void setOffsetY(int offset);
	QWidget* m_content;
signals:
	void sigHide();
protected:
	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	virtual void hideEvent(QHideEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::Sn3DDialogBack *ui;
    bool isDraggingWindow ;
    QPoint lastMousePos;
};

#endif // SN3DDIALOGBACK_H
