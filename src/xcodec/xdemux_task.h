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
#include "xdemux.h"
enum XSYN_TYPE
{
	XSYN_NONE = 0,  //����ͬ��
	XSYN_VIDEO = 1, //������Ƶͬ������������Ƶ
};
class XCODEC_API XDemuxTask :public XThread
{
public:
	int audio_index() { return demux_.audio_index(); }
	int video_index() { return demux_.video_index(); }

    void Main();
    /// <summary>
    /// �򿪽��װ
    /// </summary>
    /// <param name="url">rtsp��ַ</param>
    /// <param name="timeout_ms">��ʱʱ�� ��λ����</param>
    /// <returns></returns>
    bool Open(std::string url,int timeout_ms = 1000);

    //������Ƶ����
    std::shared_ptr<XPara> CopyVideoPara()
    {
        return demux_.CopyVideoPara();
    }
    std::shared_ptr<XPara> CopyAudioPara()
    {
        return demux_.CopyAudioPara();
    }

	void set_syn_type(XSYN_TYPE t) { syn_type_ = t; }

	void Stop();

	bool Seek(long long ms);
private:
    XDemux demux_;
    std::string url_;
    int timeout_ms_ = 0;//��ʱʱ��
	XSYN_TYPE syn_type_ = XSYN_NONE;
};

