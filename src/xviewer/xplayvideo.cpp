#include "xplayvideo.h"
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>

void XPlayVideo::timerEvent(QTimerEvent* ev)
{
	//if (!view_)return;
	//auto f = decode_.GetFrame();
	//if (!f)return;
	//view_->DrawFrame(f);
	//XFreeFrame(&f);
	if (player.is_pause())
	{
		ui.play->setStyleSheet(
			"border-image: url(:/XViewer/img/playbutton_120627.png);");
	}
	else
	{
		ui.play->setStyleSheet(
			"border-image: url(:/XViewer/img/pausebutton_120631.png);");
	}

	if (player.is_pause())return;
	player.Update();
	auto pos = player.pos_ms();
	auto total = player.total_ms();
	ui.pos->setMaximum(total);
	ui.pos->setValue(pos);
}
void XPlayVideo::Close()
{
	//关闭上次数据
	//demux_.Stop();
	//decode_.Stop();

	player.Stop();
}

void XPlayVideo::closeEvent(QCloseEvent* ev)
{
	//if (v) delete v;
	//if (player) delete player;
	Close();
	//this->hide();	 // 隐藏窗口
	//ev->ignore(); // 忽视原来的关闭事件
}

bool XPlayVideo::Open(const char* url)
{
	//player->setMedia(QUrl::fromLocalFile(url));
	//player->play();

	//demux_.Stop();
	//decode_.Stop();
	//if (!demux_.Open(url)) //解封装
	//{
	//	return false;
	//}
	//auto vp = demux_.CopyVideoPara();
	//if (!vp)
	//	return false;
	//if (!decode_.Open(vp->para))//解码
	//{
	//	return false;
	//}
	//demux_.set_next(&decode_);

	//if(!view_) view_ = XVideoView::Create();
	//view_->set_win_id((void*)(ui.VideoWidget->winId()));
	//if (!view_->Init(vp->para)) //SDL渲染
	//	return false;
	//demux_.set_syn_type(XSYN_VIDEO);
	//demux_.Start();
	//decode_.Start();

	if (!player.Open(url,(void*)ui.video->winId())) //ui.VideoWidget->
		return false;
	player.Start();
	player.Pause(false);//播放状态
	startTimer(10);
	return true;
}
XPlayVideo::XPlayVideo(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//startTimer(10);
	//layout->addWidget(ui.video);
	//layout->setMargin(0);
	//this->setLayout(layout);

	//player = new QMediaPlayer(this);
	//player->setNotifyInterval(2000);

	//layout->addWidget(v);
	//this->setLayout(layout);

	//player->setVideoOutput(v);


}

XPlayVideo::~XPlayVideo()
{
	Close();
}

void XPlayVideo::SetSpeed()
{
	float speed = 1;
	int s = ui.speed->value();
	speed = (float)s / 10;
	ui.speedtxt->setText(QString::number(speed));
	player.SetSpeed(speed);

}

void XPlayVideo::Move()        //进度条拖动
{
	player.Pause(true);
}

void XPlayVideo::PlayPos()     //控制播放进度
{
	player.Seek(ui.pos->value());
	player.Pause(false);
}

void XPlayVideo::Pause()
{
	player.Pause(!player.is_pause());
}