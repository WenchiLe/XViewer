#pragma once

#include <QtWidgets/QWidget>
#include "ui_xviewer.h"
#include <QMenu>

class XViewer : public QWidget
{
    Q_OBJECT

public:
    XViewer(QWidget *parent = Q_NULLPTR);

	//鼠标事件 用于拖动窗口
	void mouseMoveEvent(QMouseEvent* ev) override;
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;

	//窗口大小发生变化
	void resizeEvent(QResizeEvent* ev) override;
	//右键菜单
	void contextMenuEvent(QContextMenuEvent* ev) override;

	//预览视频窗口
	void View(int count);

	//刷新左侧相机列表
	void RefreshCams();

	//编辑摄像机
	void SetCam(int index);

	//定时器渲染视频 回调函数
	void timerEvent(QTimerEvent* ev) override;

public slots:
	void MaxWindow();
	void NormalWindow();
	void View1();
	void View4();
	void View9();
	void View16();
	void AddCam();//新增摄像机配置
	void SetCam();//修改摄像机配置
	void DelCam();//删除摄像机配置

	void StartRecord();//开始全部摄像头录制
	void StopRecord();//停止全部摄像头录制

	void Preview();//预览界面
	void Playback();//回放界面

	void SelectCamera(QModelIndex index);//选择摄像机
	void SelectDate(QDate date);        //选择日期
	void PlayVideo(QModelIndex index);  //选择时间播放视频

private:
    Ui::XViewerClass ui;
	QMenu left_menu_;
};
