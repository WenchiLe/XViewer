//////////////////////////////////  @版权说明  //////////////////////////////////////////////////
///						Jiedi(China nanjing)Ltd.                                    
/// @版权说明 代码和课程版权有夏曹俊所拥有并已经申请著作权，此代码可用作为学习参考并可在项目中使用，
/// 课程中涉及到的其他开源软件，请遵守其相应的授权
/// 课程源码不可以直接转载到公开的博客，或者其他共享平台，不可以用以制作在线课程。
/// 课程中涉及到的其他开源软件，请遵守其相应的授权               
/////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  源码说明  //////////////////////////////////////////////////
/// 项目名称      : FFmpeg 4.2 从基础实战-多路H265监控录放开发 实训课
/// Contact       : xiacaojun@qq.com
///  博客   :				http://blog.csdn.net/jiedichina
///	视频课程 : 网易云课堂	http://study.163.com/u/xiacaojun		
///			   腾讯课堂		https://jiedi.ke.qq.com/				
///			   csdn学院		http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961	
///           51cto学院		https://edu.51cto.com/sd/d3b6d
///			   老夏课堂		http://www.laoxiaketang.com 
/// 更多资料请在此网页下载  http://ffmpeg.club
/// ！！！请加入课程qq群 【639014264】与同学交流和下载资料 
/// 微信公众号: jiedi2007
/// 头条号	 : xiacaojun
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// 课程交流qq群 639014264  //////////////////////////////////////
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
    demux_.set_c(nullptr);//断开之前的连接
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
            //读取失败
            cout << "-" << flush;
            if (!demux_.is_connected())
            {
                Open(url_, timeout_ms_);
            }

            this_thread::sleep_for(1ms);
            continue;
        }
		//播放速度控制
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