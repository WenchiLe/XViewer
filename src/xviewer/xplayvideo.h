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
	void SetSpeed();    //���Ʋ����ٶ�
	void PlayPos();     //���Ʋ��Ž��� �������ɿ�
	void Pause();       //���ź���ͣ
	void Move();        //�������϶�

private:
	Ui::XPlayVideo ui;
	XPlayer player;

	//QMediaPlayer* player = new QMediaPlayer(this);;
	//QVideoWidget* v = new QVideoWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(this);
};
