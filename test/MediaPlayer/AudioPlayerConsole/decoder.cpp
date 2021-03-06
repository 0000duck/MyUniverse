//simplest_ffmpeg_audio_player
//
//雷霄骅 Lei Xiaohua
//leixiaohua1020@126.com
//中国传媒大学/数字电视技术
//Communication University of China / Digital TV Technology
//
//
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
//SDL
#include "SDL.h"
#include "SDL_thread.h"
};
#include "decoder.h"
//#include "wave.h"

//#define _WAVE_

//全局变量---------------------
	static  Uint8  *audio_chunk; 
	static  Uint32  audio_len; 
	static  Uint8  *audio_pos; 
//-----------------
	/*  The audio function callback takes the following parameters: 
	stream: A pointer to the audio buffer to be filled 
	len: The length (in bytes) of the audio buffer (这是固定的4096？)
	回调函数
	注意：mp3为什么播放不顺畅？
	len=4096;audio_len=4608;两个相差512！为了这512，还得再调用一次回调函数。。。
	m4a,aac就不存在此问题(都是4096)！
	*/ 
	void  fill_audio(void *udata,Uint8 *stream,int len)
	{ 
		memcpy(stream, audio_pos, len);
		audio_pos += len; 
	} 
//-----------------


int decode_audio(char* no_use)
{
	AVFormatContext	*pFormatCtx;
	int				i, audioStream;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;

	char url[300]={0};
	strcpy(url,no_use);
	//Register all available file formats and codecs
	av_register_all();

	//支持网络流输入
	avformat_network_init();
	//初始化
	pFormatCtx = avformat_alloc_context();
	//有参数avdic
	//if(avformat_open_input(&pFormatCtx,url,NULL,&avdic)!=0){
	if(avformat_open_input(&pFormatCtx,url,NULL,NULL)!=0){
		printf("Couldn't open file.\n");
		return -1;
	}
	
	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	// Dump valid information onto standard error
	av_dump_format(pFormatCtx, 0, url, false);

	// Find the first audio stream
	audioStream=-1;
	for(i=0; i < pFormatCtx->nb_streams; i++)
		//原为codec_type==CODEC_TYPE_AUDIO
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			audioStream=i;
			break;
		}

	if(audioStream==-1)
	{
		printf("Didn't find a audio stream.\n");
		return -1;
	}

	// Get a pointer to the codec context for the audio stream
	pCodecCtx=pFormatCtx->streams[audioStream]->codec;

	// Find the decoder for the audio stream
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}

	// Open codec
	if(avcodec_open2(pCodecCtx, pCodec,0)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	//把结构体改为指针
	AVPacket *packet=(AVPacket *)malloc(sizeof(AVPacket));
	av_init_packet(packet);

	//音频和视频解码更加统一！
	//新加
	AVFrame	*pFrame;
	pFrame=avcodec_alloc_frame();

	//---------SDL--------------------------------------
	//初始化
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		exit(1);
	}

	//结构体，包含PCM数据的相关信息
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = pCodecCtx->sample_rate; 
	wanted_spec.format = AUDIO_S16SYS; 
	wanted_spec.channels = pCodecCtx->channels; 
	wanted_spec.silence = 0; 
	wanted_spec.samples = 1024; //播放AAC，M4a，缓冲区的大小
	//wanted_spec.samples = 1152; //播放MP3，WMA时候用
	wanted_spec.callback = fill_audio; 
	wanted_spec.userdata = pCodecCtx; 

	if (SDL_OpenAudio(&wanted_spec, NULL)<0)//步骤（2）打开音频设备 
	{ 
		printf("can't open audio.\n"); 
		return 0; 
	} 
	//-----------------------------------------------------
	printf("比特率 %3d\n", pFormatCtx->bit_rate);
	printf("解码器名称 %s\n", pCodecCtx->codec->long_name);
	printf("time_base  %d \n", pCodecCtx->time_base);
	printf("声道数  %d \n", pCodecCtx->channels);
	printf("sample per second  %d \n", pCodecCtx->sample_rate);

	uint32_t ret,len = 0;
	int got_picture;
	int index = 0;

	audio_chunk = new Uint8[1024*1024*10];
	audio_len = 0;		


	int64_t wanted_channel_layout = 0;  
    int wanted_nb_channels;  
    wanted_channel_layout =   
        (pCodecCtx->channel_layout && pCodecCtx->channels == av_get_channel_layout_nb_channels(pCodecCtx->channel_layout)) ? 
		pCodecCtx->channel_layout 
		: av_get_default_channel_layout(pCodecCtx->channels);  

    wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;  
    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
	SwrContext *pSwrCtx = swr_alloc_set_opts
		(
		NULL,
		wanted_channel_layout,
		AV_SAMPLE_FMT_S16,
		pCodecCtx->sample_rate,  
        pCodecCtx->channel_layout,
		pCodecCtx->sample_fmt,  
        pCodecCtx->sample_rate,
		0,
		NULL
		);  
    if(swr_init(pSwrCtx)<0) return -1;  

	while(av_read_frame(pFormatCtx, packet)>=0  && audio_len < 1024*1024*9)
	{
		if(packet->stream_index==audioStream)
		{
			int len;
			if(len = avcodec_decode_audio4( pCodecCtx, pFrame, &got_picture, packet)<0) return -1;

			if ( got_picture > 0 )
			{
				printf("index %3d\n", index);
				printf("pts %5d\n", packet->pts);
				printf("dts %5d\n", packet->dts);
				printf("packet_size %5d\n", packet->size);

				uint8_t *pChunk = audio_chunk + audio_len;
				uint8_t **ppChunk = &pChunk;

				const uint8_t ** in = (const uint8_t **)pFrame->data;

				int out_count = (1024*1024*9-audio_len) / pCodecCtx->channels / av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
				//int in_count1 = pFrame->linesize[0] / pCodecCtx->channels / av_get_bytes_per_sample((AVSampleFormat)pFrame->format);
				int in_count = pFrame->nb_samples;

				int len=swr_convert
					(
					pSwrCtx,
					ppChunk,
					out_count,  
                    in,
					in_count
					);  

                len=len*pCodecCtx->channels*av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

				audio_len += len;
			}


		}

		av_free_packet(packet);
	}
	audio_pos = audio_chunk;
	SDL_PauseAudio(0);

	while(1)Sleep(1000);

	SDL_CloseAudio();//关闭音频设备 
	// Close the codec
	avcodec_close(pCodecCtx);
	// Close the video file
	av_close_input_file(pFormatCtx);
	 swr_free(&pSwrCtx);
	delete audio_chunk;

	return 0;
}


