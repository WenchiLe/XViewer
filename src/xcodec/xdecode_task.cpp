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

#include "xdecode_task.h"
#include <iostream>
#include "xtools.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

}
using namespace std;

/// <summary>
/// 清理缓存
/// </summary>
void XDecodeTask::Clear()
{
	pkt_list_.Clear();
	unique_lock<mutex> lock(mux_);
	while (!frames_.empty())
	{
		av_frame_free(&frames_.front());
		frames_.pop_front();
	}
	cur_pts_ = -1;
	decode_.Clear();
}

void XDecodeTask::Stop()
{
	pkt_list_.Clear();
	unique_lock<mutex> lock(mux_);
	decode_.set_c(nullptr);
	is_open_ = false;
	if (time_base_)
	{
		delete time_base_;
		time_base_ = nullptr;
	}
	while (!frames_.empty())
	{
		av_frame_free(&frames_.front());
		frames_.pop_front();
	}
	XThread::Stop();
}

/// <summary>
/// 打开解码器
/// </summary>
bool XDecodeTask::Open(AVCodecParameters* para)
{
	is_open_ = false;
    if (!para)
    {
        LOGERROR("para is null!");
        return false;
    }
    unique_lock<mutex> lock(mux_);
    auto c = decode_.Create(para->codec_id, false);
    if (!c)
    {
        LOGERROR("decode_.Create failed!");
        return false;
    }
    //复制视频参数
    avcodec_parameters_to_context(c, para);
    decode_.set_c(c);
    if (!decode_.Open())
    {
        LOGERROR("decode_.Open() failed!");
        return false;
    }
    LOGINFO("Open decode success!");
	is_open_ = true;
    return true;
}

//责任链处理函数
void XDecodeTask::Do(AVPacket* pkt)
{
    cout << "#" << flush;

    if (!pkt || pkt->stream_index != stream_index_) //判断是否是视频
    {
        return;
    }
    pkt_list_.Push(pkt);
	if (block_size_ <= 0)return;
	while (!is_exit_)
	{
		if (pkt_list_.Size() > block_size_)
		{
			MSleep(1);
			continue;
		}
		break;
	}
}

AVFrame* XDecodeTask::GetFrame()
{
    unique_lock<mutex> lock(mux_);
	if (frame_cache_)
	{
		if (frames_.empty())return nullptr;
		auto f = frames_.front();
		frames_.pop_front();
		return f;
	}
    if (!need_view_ || !frame_ || !frame_->buf[0])return nullptr;
    auto f = av_frame_alloc();
    auto re = av_frame_ref(f, frame_);//引用加1
    if (re != 0)
    {
        av_frame_free(&f);
        PrintErr(re);
        return nullptr;
    }
    need_view_ = false;
    return f;
}
//线程主函数
void XDecodeTask::Main()
{
    {
    unique_lock<mutex> lock(mux_);
    if(!frame_)
        frame_ = av_frame_alloc();
    }
    while (!is_exit_)
    {
		if (is_pause()) //暂停
		{
			MSleep(1);
			continue;
		}
		//同步
		while (!is_exit_)
		{
			if (syn_pts_ >= 0 && cur_pts_ > syn_pts_)
			{
				MSleep(1);
				continue;
			}
			break;
		}

        auto pkt = pkt_list_.Pop();
        if (!pkt)
        {
            this_thread::sleep_for(1ms);
            continue;
        }

        //发送到解码线程
        bool re = decode_.Send(pkt);
        av_packet_free(&pkt);
        if (!re)
        {
            this_thread::sleep_for(1ms);
            continue;
        }
        {
            unique_lock<mutex> lock(mux_);
            if(decode_.Recv(frame_))
            { 
                cout << "@" << flush;
                need_view_ = true;
				cur_pts_ = frame_->pts;
				if (time_base_)
				{
					cur_ms_ = av_rescale_q(frame_->pts, *time_base_, { 1,1000 });
				}
            }
			if (frame_cache_)
			{
				auto f = av_frame_alloc();
				av_frame_ref(f, frame_);//引用计数加1
				frames_.push_back(f);
			}
        }
        this_thread::sleep_for(1ms);
    }
    {
    unique_lock<mutex> lock(mux_);
    if(frame_)
        av_frame_free(&frame_);
    }
}

void XDecodeTask::set_time_base(AVRational* time_base)
{
	if (!time_base) return;
	unique_lock<mutex> lock(mux_);
	if (time_base_) delete time_base_;
	time_base_ = new AVRational();
	time_base_->den = time_base->den;
	time_base_->num = time_base->num;
}