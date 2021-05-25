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

#pragma once
#include "xtools.h"
#include "xdecode.h"
class XCODEC_API XDecodeTask :public XThread
{
public:
    /// <summary>
    /// 打开解码器
    /// </summary>
    bool Open(AVCodecParameters* para);

    //责任链处理函数
    void Do(AVPacket* pkt) override; 

    //线程主函数
    void Main() override;

    // 线程安全，返回当前需要渲染的AVFrame，如果没有返回nullptr
    // need_view_控制渲染
    // 返回结果需要用 XFreeFrame 释放
    AVFrame* GetFrame();

	void set_stream_index(int i) { stream_index_ = i; }

	void set_frame_cache(bool is) { frame_cache_ = is; }

	bool is_open() { return is_open_; }

	//设置同步时间
	void set_syn_pts(long long p) { syn_pts_ = p; }
	void set_block_size(int s) { block_size_ = s; }

	/// <summary>
	/// 清理缓存
	/// </summary>
	void Clear();
	void Stop();

	//当前播放位置的毫秒
	long long cur_ms() { return cur_ms_; }
	void set_time_base(AVRational* time_base);



private:
	long long cur_pts_ = -1;//当前解码到的pts（以解码数据为准）
	AVRational* time_base_ = nullptr;
	long long cur_ms_ = 0; //当前播放位置的毫秒数
	int block_size_ = 0;  //阻塞大小
	long long syn_pts_ = -1;//同步时间 -1不同步
	bool is_open_ = false;//是否打开
	int stream_index_ = 0;
    std::mutex mux_;
    XDecode decode_;
    XAVPacketList pkt_list_;
    AVFrame* frame_ = nullptr;//解码后存储
    bool need_view_ = false;    //是否需要渲染，每帧只渲染一次，通过GetFrame
	std::list<AVFrame*> frames_;    //存储音频缓冲
	bool frame_cache_ = false;      //是否缓冲frame队列
};

