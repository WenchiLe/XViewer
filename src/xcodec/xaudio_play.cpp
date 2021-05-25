#include "xaudio_play.h"
#include <iostream>
#include <sdl/SDL.h>
extern "C"
{
#include <libavformat/avformat.h>
}

using namespace std;
class CXAudioPlay :public XAudioPlay 
{
public:

	//��ͣ
	void Pause(bool is_pause)
	{
		if (is_pause)
		{
			SDL_PauseAudio(1);
			pause_begin = NowMs();
		}
		else
		{
			//ȥ����ͣ���¼�
			if (pause_begin > 0)
				last_ms_ += (NowMs() - pause_begin);
			SDL_PauseAudio(0);
		}

	}

    void Close()
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        unique_lock<mutex> lock(mux_);
        audio_datas_.clear();
        cur_pts_ = 0; //��ǰ����λ��
        last_ms_ = 0;  //�ϴε�ʱ���
        pause_begin = 0;//��ͣ��ʼʱ���
    }

    bool Open(XAudioSpec& spec)
    {
		this->spec_ = spec;
        //�˳���һ����Ƶ
        SDL_QuitSubSystem(SDL_INIT_AUDIO);

        SDL_AudioSpec sdl_spec;
        sdl_spec.freq = spec.freq;
        sdl_spec.format = spec.format;
        sdl_spec.channels = spec.channels;
        sdl_spec.samples = spec.samples;
        sdl_spec.silence = 0;
        sdl_spec.userdata = this;
		sdl_spec.callback = AudioCallback;
        if (SDL_OpenAudio(&sdl_spec, nullptr) < 0)
        {
            cerr << SDL_GetError() << endl;
            return false;
        }
		//��ʼ����
		SDL_PauseAudio(0);
        return true;
    }
    void Callback(unsigned char* stream, int len)
    {
		SDL_memset(stream, 0, len);
		unique_lock<mutex> lock(mux_);
		if (audio_datas_.empty())return;
		auto buf = audio_datas_.front();
		// 1 buf ����stream����  offset��¼λ��
		// 2 buf С��stream ����  ƴ��
		int mixed_size = 0;     //�Ѿ�������ֽ���
		int need_size = len;    //��Ҫ������ֽ���
		cur_pts_ = buf.pts;     //��ǰ���ŵ�pts
		last_ms_ = NowMs();     //��ʱ��ʼ����

		while (mixed_size < len)
		{
			if (audio_datas_.empty())break;
			buf = audio_datas_.front();
			int size = buf.data.size() - buf.offset;//ʣ��δ���������
			if (size > need_size)
			{
				size = need_size;
			}
			SDL_MixAudio(stream + mixed_size,
				buf.data.data() + buf.offset,
				size, volume_);
			need_size -= size;
			mixed_size += size;
			buf.offset += size;
			if (buf.offset >= buf.data.size())
			{
				audio_datas_.pop_front();
			}
		}
    }
	long long cur_pts() override
	{
		double ms = 0;
		if (last_ms_ > 0)
			ms = NowMs() - last_ms_;//�����ϴ�д�뻺��Ĳ���ʱ�����
		//pts ���뻻��pts��ʱ�����
		if (time_base_ > 0)
			ms = ms / (double)1000 / (double)time_base_;
		return cur_pts_ + speed_*ms;
	}
private:
	long long cur_pts_ = 0; //��ǰ����λ��
	long long last_ms_ = 0;  //�ϴε�ʱ���
	long long pause_begin = 0;//��ͣ��ʼʱ���
};

XAudioPlay* XAudioPlay::Instance()
{
    static CXAudioPlay cx;
    return &cx;
}

XAudioPlay::XAudioPlay()
{
    SDL_Init(SDL_INIT_AUDIO);
}

bool XAudioPlay::Open(AVCodecParameters* para)
{
	XAudioSpec spec;
	spec.channels = para->channels;
	spec.freq = para->sample_rate;

	AVSampleFormat;
	switch (para->format)
	{
	case AV_SAMPLE_FMT_S16:         ///< signed 16 bits
	case AV_SAMPLE_FMT_S16P:        ///< signed 16 bits, planar
		spec.format = AUDIO_S16;
		break;
	case AV_SAMPLE_FMT_S32:         ///< signed 32 bits
	case AV_SAMPLE_FMT_S32P:        ///< signed 32 bits, planar
		spec.format = AUDIO_S32;
		break;
	case AV_SAMPLE_FMT_FLT:         ///< float
	case AV_SAMPLE_FMT_FLTP:        ///< float, planar
		spec.format = AUDIO_F32;
		break;
	default:
		break;
	}
	return Open(spec);
}

bool XAudioPlay::Open(XPara& para)
{
	if (para.time_base->num > 0)
		time_base_ = (double)para.time_base->den / (double)para.time_base->num;
	return Open(para.para);
}

void XAudioPlay::Push(AVFrame* frame)
{
	if (!frame || !frame->data[0])return;
	vector<unsigned char> buf;
	int sample_size = av_get_bytes_per_sample((AVSampleFormat)frame->format);
	int channels = frame->channels;//ͨ������
	unsigned char* L = frame->data[0];
	unsigned char* R = frame->data[1];
	unsigned char* data = nullptr;
	if (channels == 1)
	{
		Push(frame->data[0], frame->nb_samples* sample_size, frame->pts);
		return;
	}
	//��ʱ֧��˫ͨ��
	switch (frame->format)
	{
		//case AV_SAMPLE_FMT_S16P:        ///< signed 16 bits, planar
	case AV_SAMPLE_FMT_S32P:        ///< signed 32 bits, planar
	case AV_SAMPLE_FMT_FLTP:        ///< float, planar
		buf.resize(frame->linesize[0]);
		data = buf.data();
		// LLLL RRRR  32�ֽ�
		// LR LR LR LR 4�α���
		for (int i = 0; i < frame->nb_samples; i++)
		{
			memcpy(data + i * sample_size * channels,
				L + i * sample_size, sample_size);
			memcpy(data + i * sample_size * channels + sample_size,
				R + i * sample_size, sample_size);
		}
		Push(data, frame->linesize[0],frame->pts);
		return;
		break;
	default:
		break;
	}
	Push(frame->data[0], frame->linesize[0], frame->pts);
}
