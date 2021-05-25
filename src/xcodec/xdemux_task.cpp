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

#include "xdemux_task.h"
extern "C"
{
#include <libavformat/avformat.h>
}
using namespace std;

void XDemuxTask::Stop()
{
	XThread::Stop();
	demux_.set_c(nullptr);
}

bool XDemuxTask::Open(std::string url, int timeout_ms)
{
 
    LOGDEBUG("XDemuxTask::Open begin!");
    demux_.set_c(nullptr);//�Ͽ�֮ǰ������
    this->url_ = url;
    this->timeout_ms_ = timeout_ms;
    auto c = demux_.Open(url.c_str());
    if (!c)return false;
    demux_.set_c(c);
    demux_.set_time_out_ms(timeout_ms);
    LOGDEBUG("XDemuxTask::Open end!");
    return true;
}
void XDemuxTask::Main()
{
    AVPacket pkt;
    while (!is_exit_)
    {
		if (is_pause())
		{
			MSleep(1);
			continue;
		}
        if (!demux_.Read(&pkt))
        {
            //��ȡʧ��
            cout << "-" << flush;
            if (!demux_.is_connected())
            {
                Open(url_, timeout_ms_);
            }

            this_thread::sleep_for(1ms);
            continue;
        }
		//�����ٶȿ���
		cout << "." << flush;
		if (syn_type_ == XSYN_VIDEO &&
			pkt.stream_index == demux_.video_index())
		{
			auto dur = demux_.RescaleToMs(pkt.duration, pkt.stream_index);
			if (dur <= 0)
				dur = 40;
			//pkt.duration
			MSleep(dur);
		}
        Next(&pkt);
        av_packet_unref(&pkt);

        this_thread::sleep_for(1ms);
    }
}

bool XDemuxTask::Seek(long long ms)
{
	auto vp = demux_.CopyVideoPara();
	if (!vp)return false;
	auto pts = av_rescale_q(ms, { 1,1000 }, *vp->time_base);
	return demux_.Seek(pts, video_index());
}