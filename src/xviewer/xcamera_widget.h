#pragma once
#include <QWidget>

struct XDecodeTask;
struct XDemuxTask;
struct XVideoView;

class XCameraWidget:public QWidget
{
	Q_OBJECT

public:
	XCameraWidget(QWidget* parent = nullptr);

	//拖拽进入
	void dragEnterEvent(QDragEnterEvent* e) override;

	//拖拽松开
	void dropEvent(QDropEvent* e) override;

	//渲染
	void paintEvent(QPaintEvent* e) override;

	//打开rtsp 开始解封装解码
	bool Open(const char* url);

	//渲染视频
	void Draw();

	//清理资源
	~XCameraWidget();

	//关闭视频，不清理内存，因为SDL关闭窗口函数有问题
	void Close();
	//是否关闭
	bool IsClosed() { return isclosed; };
private:
	XDecodeTask* decode_ = nullptr;
	XDemuxTask* demux_ = nullptr;
	XVideoView* view_ = nullptr;
	bool isclosed = true;
};

