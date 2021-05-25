#include "xcamera_widget.h"
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QListWidget>
#include "xdemux_task.h"
#include "xdecode_task.h"
#include "xvideo_view.h"
#include "xcamera_config.h"

XCameraWidget::XCameraWidget(QWidget* parent):QWidget(parent)
{
	//������ק
	this->setAcceptDrops(true);
	isclosed = true;
}

//��Ⱦ
void  XCameraWidget::paintEvent(QPaintEvent* e)
{
	//��Ⱦ��ʽ��
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//��ק����
void XCameraWidget::dragEnterEvent(QDragEnterEvent* e)
{
	//������ק����
	e->acceptProposedAction();
}

//��ק�ɿ�
void XCameraWidget::dropEvent(QDropEvent* e)
{
	//�õ�url
	qDebug() << e->source()->objectName();
	auto wid = (QListWidget*)e->source();
	qDebug() << wid->currentRow();
	auto cam = XCameraConfig::Instance()->GetCam(wid->currentRow());
	Open(cam.url);
} 

bool XCameraWidget::Open(const char* url)
{
	if (demux_)
	{
		demux_->Exit();
		demux_->Wait();
		delete demux_;
		demux_ = nullptr;
	}
	if (decode_)
	{
		decode_->Exit();
		decode_->Wait();
		delete decode_;
		decode_ = nullptr;
	}
	//�򿪽��װ
	if(!demux_) demux_ = new XDemuxTask();
	if (!demux_->Open(url))
	{
		return false;
	}
	//����Ƶ�������߳�
	if (!decode_) decode_ = new XDecodeTask();
	auto para = demux_->CopyVideoPara();
	if (!decode_->Open(para->para))
	{
		return false;
	}
	//�趨�����߳̽��ս��װ����
	demux_->set_next(decode_);

	//��ʼ����Ⱦ
	if (!view_) view_ = XVideoView::Create();
	view_->set_win_id((void*)winId());
	view_->Init(para->para);

	//�����ӷ�װ�ͽ����߳�
	demux_->Start();
	decode_->Start();
	isclosed = false;
	return true;
}

//��Ⱦ��Ƶ
void XCameraWidget::Draw()
{
	if (!demux_ || !decode_ || !view_) return;
	auto f = decode_->GetFrame();
	if (!f) return;
	view_->DrawFrame(f);
	XFreeFrame(&f);
}

//�ر���Ƶ���������ڴ棬��ΪSDL�رմ��ں���������
void XCameraWidget::Close()
{
	isclosed = true;
	if (demux_)
	{
		demux_->Stop();
	}
	if (decode_)
	{
		decode_->Stop();
	}
	this->hide();	 // ���ش���
}

XCameraWidget::~XCameraWidget()
{
	isclosed = true;
	if (demux_)
	{
		demux_->Exit();
		demux_->Wait();
		delete demux_;
		demux_ = nullptr;
	}
	if (decode_)
	{
		decode_->Exit();
		decode_->Wait();
		delete decode_;
		decode_ = nullptr;
	}
	if (view_)
	{
		view_->Close();
		delete view_;
		view_ = nullptr;
	}
}