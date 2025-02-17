#include "xviewer.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>
#include <QDebug>
#include <QContextMenuEvent>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMEssageBox>
#include <sstream>
#include <QDir>
#include <map>
#include <vector>
#include "xcamera_config.h"
#include "xcamera_widget.h"
#include "xcamera_record.h"
#include "xplayvideo.h"

using namespace std;

//解决中文乱码
#define C(s) QString::fromLocal8Bit(s)
#define CAM_CONF_PATH "cam.db"

static XCameraWidget* cam_wids[16] = { 0 };
//视频录制
static vector<XCameraRecord*> records;


//存储视频日期时间
struct XCamVideo
{
	QString filepath;
	QDateTime datetime;
};
static map<QDate, vector<XCamVideo> > cam_videos;

void XViewer::SelectCamera(QModelIndex index)//选择摄像机
{
	qDebug() << "SelectCamera" << index.row();
	auto conf = XCameraConfig::Instance();
	auto cam = conf->GetCam(index.row()); //获取相机参数
	if (cam.name[0] == '\0')
	{
		return;
	}
	//相机视频存储路径
	stringstream ss;
	ss << cam.save_path << "/" << index.row() << "/";

	//遍历此目录
	QDir dir(C(ss.str().c_str()));
	if (!dir.exists())
		return;
	//获取目录下文件列表
	QStringList filters;
	filters << "*.mp4" << "*.avi";
	dir.setNameFilters(filters);//筛选

	//清理其他相机的数据
	ui.cal->ClearDate();
	cam_videos.clear();

	//所有文件列表
	auto files = dir.entryInfoList();
	for (auto file : files)
	{
		//"cam_2020_09_04_17_54_58.mp4"
		QString filename = file.fileName();

		//去掉cam_ 和 .mp4
		auto tmp = filename.left(filename.size() - 4);
		tmp = tmp.right(tmp.length() - 4);
		//2020_09_04_17_54_58
		auto dt = QDateTime::fromString(tmp, "yyyy_MM_dd_hh_mm_ss");
		qDebug() << dt.date();
		ui.cal->AddDate(dt.date());
		//qDebug() << file.fileName();

		XCamVideo video;
		video.datetime = dt;
		video.filepath = file.absoluteFilePath();
		cam_videos[dt.date()].push_back(video);
	}

	//重新显示日期
	ui.cal->showNextMonth();
	ui.cal->showPreviousMonth();


}
void XViewer::SelectDate(QDate date)        //选择日期
{
	qDebug() << "SelectDate" << date.toString();
	auto dates = cam_videos[date];
	ui.time_list->clear();
	for (auto d : dates)
	{
		auto item = new QListWidgetItem(d.datetime.time().toString());

		//item 添加自定义数据 文件路径
		item->setData(Qt::UserRole, d.filepath);
		ui.time_list->addItem(item);
	}
}
//static XPlayVideo* play = nullptr;
void XViewer::PlayVideo(QModelIndex index)  //选择时间播放视频
{
	qDebug() << "PlayVideo" << index.row();
	auto item = ui.time_list->currentItem();
	if (!item)return;
	QString path = item->data(Qt::UserRole).toString();
	qDebug() << path;
	//视频回放
	//if (play)
	//{
	//	delete play;
	//	play = nullptr;
	//}
	//play = new XPlayVideo();
	//play->Open(path.toLocal8Bit());
	//play->show();
	XPlayVideo play;
	play.Open(path.toLocal8Bit());
	play.show();
	play.exec();
}

void XViewer::View1() 
{
	View(1);
}

void XViewer::View4() 
{
	View(4);
}

void XViewer::View9() 
{
	View(9);
}

void XViewer::View16() 
{
	View(16);
}

void XViewer::View(int count)
{
	qDebug() << "View" << count;
	//2x2 3x3 4x4
	//确定列数 根号
	int cols = sqrt(count);
	//总窗口数量
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);

	//初始化布局器
	auto lay = (QGridLayout *)ui.cams->layout();
	if (!lay)
	{
		lay = new QGridLayout();
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(2);//元素间距
		ui.cams->setLayout(lay);
	}
	//初始化窗口
	for (int i = 0; i < count; i++)
	{
		if (!cam_wids[i])
		{
			cam_wids[i] = new XCameraWidget();
			cam_wids[i]->setStyleSheet("background-color:rgb(51,51,51)");
		}
		else
		{
			cam_wids[i]->show();
		}
		lay->addWidget(cam_wids[i],i/cols,i%cols);
	}

	//清理多余窗体
	for (int i = count; i < wid_size; i++)
	{
		if (cam_wids[i])
		{
			//delete cam_wids[i];
			//cam_wids[i] = nullptr;
			cam_wids[i]->Close();
		}
	}
}

//定时器渲染视频 回调函数
void XViewer::timerEvent(QTimerEvent* ev)
{
	//总窗口数量
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
	for (int i = 0; i < wid_size; i++)
	{
		if (cam_wids[i])
		{
			if(!cam_wids[i]->IsClosed())
			//渲染多窗口视频
			{
				cam_wids[i]->Draw();
			}
		}
	}
}

void XViewer::StartRecord() //开始全部摄像头录制
{
	StopRecord();
	qDebug() << "开始全部摄像头录制";
	ui.status->setText(C("录制中。。。"));
	//获取配置列表 
	auto conf = XCameraConfig::Instance();
	int count = conf->GetCamCount();
	for (int i = 0; i < count; i++)
	{
		auto cam = conf->GetCam(i);
		stringstream ss;
		ss << cam.save_path << "/" << i << "/";
		QDir dir;
		dir.mkpath(ss.str().c_str());

		XCameraRecord* rec = new XCameraRecord();
		rec->set_rtsp_url(cam.url);
		rec->set_save_path(ss.str());
		rec->set_file_sec(5);
		rec->Start();
		records.push_back(rec);
	}

	//创建录制目录
	//分别开始录制线程
}
void XViewer::StopRecord()  //停止全部摄像头录制
{
	ui.status->setText(C("监控中。。。"));
	for (auto rec : records)
	{
		rec->Stop();
		delete rec;
	}
	records.clear();
}

//右键菜单
void XViewer::contextMenuEvent(QContextMenuEvent* ev)
{
	//鼠标位置显示右键菜单
	left_menu_.exec(QCursor::pos());
	ev->accept();
}

void XViewer::RefreshCams()
{
	auto c = XCameraConfig::Instance();
	ui.cam_list->clear();
	int count = c->GetCamCount();
	for (int i = 0; i < count; i++)
	{
		auto cam = c->GetCam(i);
		//auto p = new QPixmap(":/XViewer/img/circularmirror_114596.png",100,100);
		auto item = new QListWidgetItem(
			QIcon(":/XViewer/img/circularmirror_114596.png"),
			C(cam.name));
		ui.cam_list->addItem(item);
	}
}

//编辑摄像机
void XViewer::SetCam(int index)
{
	auto c = XCameraConfig::Instance();
	QDialog dlg(this);
	dlg.resize(800, 200);
	QFormLayout lay;
	dlg.setLayout(&lay);
	// 标题1 输入框1
	// 标题2 输入框2
	QLineEdit name_edit;
	lay.addRow(C("名称"), &name_edit);

	QLineEdit url_edit;
	lay.addRow(C("主码流"), &url_edit);

	QLineEdit sub_url_edit;
	lay.addRow(C("辅码流"), &sub_url_edit);

	QLineEdit save_path_edit;
	lay.addRow(C("保存目录"), &save_path_edit);

	QPushButton save;
	save.setText(C("保存"));

	lay.addRow("", &save);

	//编辑 读入原数据显示
	if (index >= 0)
	{
		auto cam = c->GetCam(index);
		name_edit.setText(C(cam.name));
		url_edit.setText(C(cam.url));
		sub_url_edit.setText(C(cam.sub_url));
		save_path_edit.setText(C(cam.save_path));
	}

	connect(&save, SIGNAL(clicked()), &dlg, SLOT(accept()));
	while (true)
	{
		if (dlg.exec() == QDialog::Accepted)
		{
			if (name_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("请输入名称"));
				continue;
			}
			if (url_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("请输入主码流"));
				continue;
			}
			if (sub_url_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("请输入辅码流"));
				continue;
			}
			if (save_path_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("请输入保存目录"));
				continue;
			}
			break;
		}
		return;
	}

	XCameraData cd1;
	strcpy(cd1.name, name_edit.text().toLocal8Bit());
	strcpy(cd1.url, url_edit.text().toLocal8Bit());
	strcpy(cd1.sub_url, sub_url_edit.text().toLocal8Bit());
	strcpy(cd1.save_path, save_path_edit.text().toLocal8Bit());
	if (index >= 0) //修改
	{
		c->SetCam(index, cd1);
	}
	else
	{
		c->Push(cd1);
	}
	c->Save(CAM_CONF_PATH);
	RefreshCams();
}

void XViewer::AddCam()
{
	SetCam(-1);
}



//修改摄像机配置
void XViewer::SetCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0)
	{
		QMessageBox::information(this,"error",C("请选择摄像机"));
		return;
	}
	SetCam(row);
}

//删除摄像机配置
void XViewer::DelCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0)
	{
		QMessageBox::information(this, "error", C("请选择摄像机"));
		return;
	}
	QString name = ui.cam_list->currentItem()->text();
	QString cs = C("您确认删除摄像机");
	cs.append(name);
	cs.append(C("吗？"));
	if(QMessageBox::information(this,"confirm",cs,QMessageBox::Yes, QMessageBox::No )!= QMessageBox::Yes)
	{
		return;
	}
	XCameraConfig::Instance()->DelCam(row);
	XCameraConfig::Instance()->Save(CAM_CONF_PATH);
	RefreshCams();
}

void XViewer::Preview()//预览界面
{
	ui.cams->show();
	ui.playback_wid->hide();
	ui.preview->setChecked(true);
}

void XViewer::Playback()//回放界面
{
	ui.cams->hide();
	ui.playback_wid->show();
	ui.playback->setChecked(true);
}

XViewer::XViewer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//去除原窗口边框
	setWindowFlags(Qt::FramelessWindowHint);

	//布局head和body 垂直布局器
	auto vlay = new QVBoxLayout();
	//边框间距
	vlay->setContentsMargins(0,0,0,0);
	//元素间距
	vlay->setSpacing(0);
	vlay->addWidget(ui.head);
	vlay->addWidget(ui.body);
	this->setLayout(vlay);

	//相机列表 和 相机预览
	//水平布局器
	auto hlay = new QHBoxLayout();
	ui.body->setLayout(hlay);
	//边框间距
	hlay->setContentsMargins(0, 0, 0, 0);
	hlay->addWidget(ui.left);		//左侧相机列表
	hlay->addWidget(ui.cams);		//右侧预览窗口
	hlay->addWidget(ui.playback_wid);//回放窗口

	ui.max->setVisible(true);
	ui.normal->setVisible(false);

	//////////////////////////////////////////////
	///初始化右键菜单
	// 视图=》1窗口
	//		  4窗口
	auto m = left_menu_.addMenu(C("视图"));
	auto a = m->addAction(C("1窗口"));
	connect(a,SIGNAL(triggered()),this,SLOT(View1()));
	a = m->addAction(C("4窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View4()));
	a = m->addAction(C("9窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View9()));
	a = m->addAction(C("16窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View16()));

	a = left_menu_.addAction(C("全部开始录制"));
	connect(a, SIGNAL(triggered()), this, SLOT(StartRecord()));
	a = left_menu_.addAction(C("全部停止录制"));
	connect(a, SIGNAL(triggered()), this, SLOT(StopRecord()));

	//默认9窗口
	View9();

	//刷新左侧相机列表
	XCameraConfig::Instance()->Load(CAM_CONF_PATH);
	RefreshCams();

	//启动定时器渲染视频
	startTimer(1);

	Preview();//默认显示预览
}

void XViewer::MaxWindow()
{
	ui.max->setVisible(false);
	ui.normal->setVisible(true);
	showMaximized();
}

void XViewer::NormalWindow()
{
	ui.max->setVisible(true);
	ui.normal->setVisible(false);
	showNormal();
}

void XViewer::resizeEvent(QResizeEvent* ev)
{
	int x = width() - ui.head_button->width();
	int y = ui.head_button->y();
	ui.head_button->move(x, y);
	auto psize = ui.playback_wid->size();
	auto tsize = ui.time_list->size();
	ui.time_list->resize(tsize.width(), psize.height());
	ui.cal->resize(psize.width() - tsize.width(), psize.height());
}
































////////////////////////////////////////////////////////////////////////
///鼠标拖动窗口
static bool mouse_press = false;
static QPoint mouse_point;

void XViewer::mouseMoveEvent(QMouseEvent* ev)
{
	if (!mouse_press)
	{
		QWidget::mouseMoveEvent(ev);
		return;
	}
	this->move(ev->globalPos() - mouse_point);
}

void XViewer::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		mouse_press = true;
		mouse_point = ev->pos();
	}
}

void XViewer::mouseReleaseEvent(QMouseEvent* ev)
{
	mouse_press = false;
}