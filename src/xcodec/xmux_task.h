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
#include "xmux.h"
class XCODEC_API XMuxTask :public XThread
{
public:
    void Main() override;
    /// <summary>
    /// �򿪷�װ�ļ�
    /// </summary>
    /// <param name="url">�����ַ</param>
    /// <param name="video_para">��Ƶ����</param>
    /// <param name="video_time_base">��Ƶʱ�����</param>
    /// <param name="audio_para">��Ƶ����</param>
    /// <param name="audio_time_base">��Ƶ��ʱ�����</param>
    /// <returns></returns>
    bool Open(const char* url,
        AVCodecParameters* video_para = nullptr,
        AVRational *video_time_base = nullptr,
        AVCodecParameters *audio_para = nullptr,
        AVRational *audio_time_base = nullptr
        );

    //��������
    void Do(AVPacket* pkt);
private:
    XMux xmux_;
    XAVPacketList pkts_;
    std::mutex mux_;

};

