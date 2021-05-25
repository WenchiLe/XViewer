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

//�����������
#define C(s) QString::fromLocal8Bit(s)
#define CAM_CONF_PATH "cam.db"

static XCameraWidget* cam_wids[16] = { 0 };
//��Ƶ¼��
static vector<XCameraRecord*> records;


//�洢��Ƶ����ʱ��
struct XCamVideo
{
	QString filepath;
	QDateTime datetime;
};
static map<QDate, vector<XCamVideo> > cam_videos;

void XViewer::SelectCamera(QModelIndex index)//ѡ�������
{
	qDebug() << "SelectCamera" << index.row();
	auto conf = XCameraConfig::Instance();
	auto cam = conf->GetCam(index.row()); //��ȡ�������
	if (cam.name[0] == '\0')
	{
		return;
	}
	//�����Ƶ�洢·��
	stringstream ss;
	ss << cam.save_path << "/" << index.row() << "/";

	//������Ŀ¼
	QDir dir(C(ss.str().c_str()));
	if (!dir.exists())
		return;
	//��ȡĿ¼���ļ��б�
	QStringList filters;
	filters << "*.mp4" << "*.avi";
	dir.setNameFilters(filters);//ɸѡ

	//�����������������
	ui.cal->ClearDate();
	cam_videos.clear();

	//�����ļ��б�
	auto files = dir.entryInfoList();
	for (auto file : files)
	{
		//"cam_2020_09_04_17_54_58.mp4"
		QString filename = file.fileName();

		//ȥ��cam_ �� .mp4
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

	//������ʾ����
	ui.cal->showNextMonth();
	ui.cal->showPreviousMonth();


}
void XViewer::SelectDate(QDate date)        //ѡ������
{
	qDebug() << "SelectDate" << date.toString();
	auto dates = cam_videos[date];
	ui.time_list->clear();
	for (auto d : dates)
	{
		auto item = new QListWidgetItem(d.datetime.time().toString());

		//item ����Զ������� �ļ�·��
		item->setData(Qt::UserRole, d.filepath);
		ui.time_list->addItem(item);
	}
}
//static XPlayVideo* play = nullptr;
void XViewer::PlayVideo(QModelIndex index)  //ѡ��ʱ�䲥����Ƶ
{
	qDebug() << "PlayVideo" << index.row();
	auto item = ui.time_list->currentItem();
	if (!item)return;
	QString path = item->data(Qt::UserRole).toString();
	qDebug() << path;
	//��Ƶ�ط�
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
	//ȷ������ ����
	int cols = sqrt(count);
	//�ܴ�������
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);

	//��ʼ��������
	auto lay = (QGridLayout *)ui.cams->layout();
	if (!lay)
	{
		lay = new QGridLayout();
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(2);//Ԫ�ؼ��
		ui.cams->setLayout(lay);
	}
	//��ʼ������
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

	//������ര��
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

//��ʱ����Ⱦ��Ƶ �ص�����
void XViewer::timerEvent(QTimerEvent* ev)
{
	//�ܴ�������
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
	for (int i = 0; i < wid_size; i++)
	{
		if (cam_wids[i])
		{
			if(!cam_wids[i]->IsClosed())
			//��Ⱦ�ര����Ƶ
			{
				cam_wids[i]->Draw();
			}
		}
	}
}

void XViewer::StartRecord() //��ʼȫ������ͷ¼��
{
	StopRecord();
	qDebug() << "��ʼȫ������ͷ¼��";
	ui.status->setText(C("¼���С�����"));
	//��ȡ�����б� 
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

	//����¼��Ŀ¼
	//�ֱ�ʼ¼���߳�
}
void XViewer::StopRecord()  //ֹͣȫ������ͷ¼��
{
	ui.status->setText(C("����С�����"));
	for (auto rec : records)
	{
		rec->Stop();
		delete rec;
	}
	records.clear();
}

//�Ҽ��˵�
void XViewer::contextMenuEvent(QContextMenuEvent* ev)
{
	//���λ����ʾ�Ҽ��˵�
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

//�༭�����
void XViewer::SetCam(int index)
{
	auto c = XCameraConfig::Instance();
	QDialog dlg(this);
	dlg.resize(800, 200);
	QFormLayout lay;
	dlg.setLayout(&lay);
	// ����1 �����1
	// ����2 �����2
	QLineEdit name_edit;
	lay.addRow(C("����"), &name_edit);

	QLineEdit url_edit;
	lay.addRow(C("������"), &url_edit);

	QLineEdit sub_url_edit;
	lay.addRow(C("������"), &sub_url_edit);

	QLineEdit save_path_edit;
	lay.addRow(C("����Ŀ¼"), &save_path_edit);

	QPushButton save;
	save.setText(C("����"));

	lay.addRow("", &save);

	//�༭ ����ԭ������ʾ
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
				QMessageBox::information(0, "error", C("����������"));
				continue;
			}
			if (url_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("������������"));
				continue;
			}
			if (sub_url_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("�����븨����"));
				continue;
			}
			if (save_path_edit.text().isEmpty())
			{
				QMessageBox::information(0, "error", C("�����뱣��Ŀ¼"));
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
	if (index >= 0) //�޸�
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



//�޸����������
void XViewer::SetCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0)
	{
		QMessageBox::information(this,"error",C("��ѡ�������"));
		return;
	}
	SetCam(row);
}

//ɾ�����������
void XViewer::DelCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0)
	{
		QMessageBox::information(this, "error", C("��ѡ�������"));
		return;
	}
	QString name = ui.cam_list->currentItem()->text();
	QString cs = C("��ȷ��ɾ�������");
	cs.append(name);
	cs.append(C("��"));
	if(QMessageBox::information(this,"confirm",cs,QMessageBox::Yes, QMessageBox::No )!= QMessageBox::Yes)
	{
		return;
	}
	XCameraConfig::Instance()->DelCam(row);
	XCameraConfig::Instance()->Save(CAM_CONF_PATH);
	RefreshCams();
}

void XViewer::Preview()//Ԥ������
{
	ui.cams->show();
	ui.playback_wid->hide();
	ui.preview->setChecked(true);
}

void XViewer::Playback()//�طŽ���
{
	ui.cams->hide();
	ui.playback_wid->show();
	ui.playback->setChecked(true);
}

XViewer::XViewer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//ȥ��ԭ���ڱ߿�
	setWindowFlags(Qt::FramelessWindowHint);

	//����head��body ��ֱ������
	auto vlay = new QVBoxLayout();
	//�߿���
	vlay->setContentsMargins(0,0,0,0);
	//Ԫ�ؼ��
	vlay->setSpacing(0);
	vlay->addWidget(ui.head);
	vlay->addWidget(ui.body);
	this->setLayout(vlay);

	//����б� �� ���Ԥ��
	//ˮƽ������
	auto hlay = new QHBoxLayout();
	ui.body->setLayout(hlay);
	//�߿���
	hlay->setContentsMargins(0, 0, 0, 0);
	hlay->addWidget(ui.left);		//�������б�
	hlay->addWidget(ui.cams);		//�Ҳ�Ԥ������
	hlay->addWidget(ui.playback_wid);//�طŴ���

	ui.max->setVisible(true);
	ui.normal->setVisible(false);

	//////////////////////////////////////////////
	///��ʼ���Ҽ��˵�
	// ��ͼ=��1����
	//		  4����
	auto m = left_menu_.addMenu(C("��ͼ"));
	auto a = m->addAction(C("1����"));
	connect(a,SIGNAL(triggered()),this,SLOT(View1()));
	a = m->addAction(C("4����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View4()));
	a = m->addAction(C("9����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View9()));
	a = m->addAction(C("16����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View16()));

	a = left_menu_.addAction(C("ȫ����ʼ¼��"));
	connect(a, SIGNAL(triggered()), this, SLOT(StartRecord()));
	a = left_menu_.addAction(C("ȫ��ֹͣ¼��"));
	connect(a, SIGNAL(triggered()), this, SLOT(StopRecord()));

	//Ĭ��9����
	View9();

	//ˢ���������б�
	XCameraConfig::Instance()->Load(CAM_CONF_PATH);
	RefreshCams();

	//������ʱ����Ⱦ��Ƶ
	startTimer(1);

	Preview();//Ĭ����ʾԤ��
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
///����϶�����
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