//////////////////////////////////  @��Ȩ˵��  //////////////////////////////////////////////////
///						Jiedi(China nanjing)Ltd.                                    
/// @��Ȩ˵�� ����Ϳγ̰�Ȩ���Ĳܿ���ӵ�в��Ѿ���������Ȩ���˴��������Ϊѧϰ�ο���������Ŀ��ʹ�ã�
/// �γ����漰����������Դ���������������Ӧ����Ȩ
/// �γ�Դ�벻����ֱ��ת�ص������Ĳ��ͣ�������������ƽ̨�������������������߿γ̡�
/// �γ����漰����������Դ���������������Ӧ����Ȩ               
/////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Դ��˵��  //////////////////////////////////////////////////
/// ��Ŀ����      : FFmpeg 4.2 �ӻ���ʵս-��·H265���¼�ſ��� ʵѵ��
/// Contact       : xiacaojun@qq.com
///  ����   :				http://blog.csdn.net/jiedichina
///	��Ƶ�γ� : �����ƿ���	http://study.163.com/u/xiacaojun		
///			   ��Ѷ����		https://jiedi.ke.qq.com/				
///			   csdnѧԺ		http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961	
///           51ctoѧԺ		https://edu.51cto.com/sd/d3b6d
///			   ���Ŀ���		http://www.laoxiaketang.com 
/// �����������ڴ���ҳ����  http://ffmpeg.club
/// �����������γ�qqȺ ��639014264����ͬѧ�������������� 
/// ΢�Ź��ں�: jiedi2007
/// ͷ����	 : xiacaojun
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// �γ̽���qqȺ 639014264  //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "xtools.h"
#include "xdecode.h"
class XCODEC_API XDecodeTask :public XThread
{
public:
    /// <summary>
    /// �򿪽�����
    /// </summary>
    bool Open(AVCodecParameters* para);

    //������������
    void Do(AVPacket* pkt) override; 

    //�߳�������
    void Main() override;

    // �̰߳�ȫ�����ص�ǰ��Ҫ��Ⱦ��AVFrame�����û�з���nullptr
    // need_view_������Ⱦ
    // ���ؽ����Ҫ�� XFreeFrame �ͷ�
    AVFrame* GetFrame();

	void set_stream_index(int i) { stream_index_ = i; }

	void set_frame_cache(bool is) { frame_cache_ = is; }

	bool is_open() { return is_open_; }

	//����ͬ��ʱ��
	void set_syn_pts(long long p) { syn_pts_ = p; }
	void set_block_size(int s) { block_size_ = s; }

	/// <summary>
	/// ������
	/// </summary>
	void Clear();
	void Stop();

	//��ǰ����λ�õĺ���
	long long cur_ms() { return cur_ms_; }
	void set_time_base(AVRational* time_base);



private:
	long long cur_pts_ = -1;//��ǰ���뵽��pts���Խ�������Ϊ׼��
	AVRational* time_base_ = nullptr;
	long long cur_ms_ = 0; //��ǰ����λ�õĺ�����
	int block_size_ = 0;  //������С
	long long syn_pts_ = -1;//ͬ��ʱ�� -1��ͬ��
	bool is_open_ = false;//�Ƿ��
	int stream_index_ = 0;
    std::mutex mux_;
    XDecode decode_;
    XAVPacketList pkt_list_;
    AVFrame* frame_ = nullptr;//�����洢
    bool need_view_ = false;    //�Ƿ���Ҫ��Ⱦ��ÿֻ֡��Ⱦһ�Σ�ͨ��GetFrame
	std::list<AVFrame*> frames_;    //�洢��Ƶ����
	bool frame_cache_ = false;      //�Ƿ񻺳�frame����
};

