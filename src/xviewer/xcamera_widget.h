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

	//��ק����
	void dragEnterEvent(QDragEnterEvent* e) override;

	//��ק�ɿ�
	void dropEvent(QDropEvent* e) override;

	//��Ⱦ
	void paintEvent(QPaintEvent* e) override;

	//��rtsp ��ʼ���װ����
	bool Open(const char* url);

	//��Ⱦ��Ƶ
	void Draw();

	//������Դ
	~XCameraWidget();

	//�ر���Ƶ���������ڴ棬��ΪSDL�رմ��ں���������
	void Close();
	//�Ƿ�ر�
	bool IsClosed() { return isclosed; };
private:
	XDecodeTask* decode_ = nullptr;
	XDemuxTask* demux_ = nullptr;
	XVideoView* view_ = nullptr;
	bool isclosed = true;
};

