//simplest_ffmpeg_audio_player
//
//������ Lei Xiaohua
//leixiaohua1020@126.com
//�й���ý��ѧ/���ֵ��Ӽ���
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

//ȫ�ֱ���---------------------
	static  Uint8  *audio_chunk; 
	static  Uint32  audio_len; 
	static  Uint8  *audio_pos; 
//-----------------
	/*  The audio function callback takes the following parameters: 
	stream: A pointer to the audio buffer to be filled 
	len: The length (in bytes) of the audio buffer (���ǹ̶���4096��)
	�ص�����
	ע�⣺mp3Ϊʲô���Ų�˳����
	len=4096;audio_len=4608;�������512��Ϊ����512�������ٵ���һ�λص�����������
	m4a,aac�Ͳ����ڴ�����(����4096)��
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

	//֧������������
	avformat_network_init();
	//��ʼ��
	pFormatCtx = avformat_alloc_context();
	//�в���avdic
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
		//ԭΪcodec_type==CODEC_TYPE_AUDIO
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

	//�ѽṹ���Ϊָ��
	AVPacket *packet=(AVPacket *)malloc(sizeof(AVPacket));
	av_init_packet(packet);

	//��Ƶ����Ƶ�������ͳһ��
	//�¼�
	AVFrame	*pFrame;
	pFrame=avcodec_alloc_frame();

	//---------SDL--------------------------------------
	//��ʼ��
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		exit(1);
	}

	//�ṹ�壬����PCM���ݵ������Ϣ
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = pCodecCtx->sample_rate; 
	wanted_spec.format = AUDIO_S16SYS; 
	wanted_spec.channels = pCodecCtx->channels; 
	wanted_spec.silence = 0; 
	wanted_spec.samples = 1024; //����AAC��M4a���������Ĵ�С
	//wanted_spec.samples = 1152; //����MP3��WMAʱ����
	wanted_spec.callback = fill_audio; 
	wanted_spec.userdata = pCodecCtx; 

	if (SDL_OpenAudio(&wanted_spec, NULL)<0)//���裨2������Ƶ�豸 
	{ 
		printf("can't open audio.\n"); 
		return 0; 
	} 
	//-----------------------------------------------------
	printf("������ %3d\n", pFormatCtx->bit_rate);
	printf("���������� %s\n", pCodecCtx->codec->long_name);
	printf("time_base  %d \n", pCodecCtx->time_base);
	printf("������  %d \n", pCodecCtx->channels);
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

				const uint8_t * pInput = pFrame->data[0];
				const uint8_t ** ppInput = &pInput;

				const uint8_t ** in = (const uint8_t **)pFrame->data;

				int len=swr_convert
					(
					pSwrCtx,
					ppChunk,
					4096 / pCodecCtx->channels / av_get_bytes_per_sample(AV_SAMPLE_FMT_S16),  
                    in,
					pFrame->linesize[0] / pCodecCtx->channels / av_get_bytes_per_sample((AVSampleFormat)pFrame->format)
					);  

                len=len*pCodecCtx->channels*av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

				//memcpy(audio_chunk + audio_len, pFrame->data[1], pFrame->linesize[0]);
				audio_len += len;
			}

			//for(int i=0; i<4096; i++)
			//{
			//	*(audio_chunk+audio_len+i) = pFrame->data[0][i];
			//}
			//printf("begin....\n"); 
			//������Ƶ���ݻ���,PCM����
			//audio_chunk = (Uint8*) pFrame->data[0]; 
			//������Ƶ���ݳ���
			//audio_len = pFrame->linesize[0];
			//audio_len = 4096;
			//����mp3��ʱ���Ϊaudio_len = 4096
			//���Ƚ���������������������MP3һ֡����4608
			//ʹ��һ�λص�������4096�ֽڻ��壩���Ų��꣬���Ի�Ҫʹ��һ�λص����������²��Ż���������
			//���ó�ʼ����λ��
			
			//�ط���Ƶ���� 
			//printf("don't close, audio playing...\n"); 
			//---------------------------------------
		}
		//while(audio_len>0)//�ȴ�ֱ����Ƶ���ݲ������! 
		//		Sleep(1);
		// Free the packet that was allocated by av_read_frame
		//�Ѹ�
		av_free_packet(packet);
	}
	audio_pos = audio_chunk;
	SDL_PauseAudio(0);

	while(1)Sleep(1000);

	SDL_CloseAudio();//�ر���Ƶ�豸 
	// Close the codec
	avcodec_close(pCodecCtx);
	// Close the video file
	av_close_input_file(pFormatCtx);

	delete audio_chunk;

	return 0;
}


