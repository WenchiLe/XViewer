#pragma once

#include <QDialog>
#include <QMediaPlayer>
#include "ui_xplayvideo.h"
#include <QVideoWidget>
#include <QVBoxLayout>
#include "xplayer.h"
class XPlayVideo : public QDialog
{
	Q_OBJECT

public:
	XPlayVideo(QWidget* parent = Q_NULLPTR);
	~XPlayVideo();
	bool Open(const char* url);

	void timerEvent(QTimerEvent* ev) override;
	void Close();
	void closeEvent(QCloseEvent* ev) override;

public slots:
	void SetSpeed();    //控制播放速度
	void PlayPos();     //控制播放进度 进度条松开
	void Pause();       //播放和暂停
	void Move();        //进度条拖动

private:
	Ui::XPlayVideo ui;
	XPlayer player;

	//QMediaPlayer* player = new QMediaPlayer(this);;
	//QVideoWidget* v = new QVideoWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(this);
};
