��Ʒ���ܣ�
1.ArcCompareView.exe  OPENGL�汾����Ҫ��3.3����
  ����ͼƬ�ȽϺ�Video��Ƶ�Ĳ��űȽϣ�֧����ͨ����Ƶ��ʽ��YUV��������
  1��֧��1����2����3����4��ͼƬ/��Ƶ�ıȽϲ��ţ�֧���϶��򿪡����ļ���ʱ��֧�����ļ�����Ŀ¼�л��ļ���֧�ֿ�ݼ��л��ļ���Ctrl+<-��Ctrl+->����
  2������ģʽ����ͨ����ģʽ�͸߼�����ģʽ����
     1����ͨ����ģʽΪ��Ƶ���ɲ��ţ�
	    -��֧��֡ͬ����
		-֧����Ƶ����/��ͣ���ز�
		-֧����Ƶͼ��Ŵ���С��1:1�� Fitin����ת
		-֧��Ctrl+UI�������Ƶ���Ƶ������Ctrl��ʱΪ������Ƶͳһ����
		-֧�ֿո������������Ƶ����ͣ/����
		-������Ƶ�Ƚ�ʱ����ͣ״̬֧�ָ���ʽ��before/after,����B|A��ťʱ����ʾ��һ����Ƶ���ڲ��ŵ����ݣ��ſ�B|A��ťʱ��ʾ��ǰ��Ƶ���ݡ�
	 2���߼�����ģʽΪ��Ƶͬ������
	    -֧��֡ͬ��
		-֧����Ƶ����/��ͣ���ز�
		-֧����Ƶͼ��Ŵ���С��1:1�� Fitin����ת		
		-֧�ֿո������������Ƶ����ͣ/����
		-������Ƶ�Ƚ�ʱ����ͣ״̬֧�ָ���ʽ��before/after,����B|A��ťʱ����ʾ��һ����Ƶ���ڲ��ŵ����ݣ��ſ�B|A��ťʱ��ʾ��ǰ��Ƶ���ݡ�
		-֧����Ƶ���ղ���ǰ��/���ˣ�ֻ����ͣ����״̬��Ч�������ɱ༭�����벽���󣬻س�����ǰ������ȷ�ϲ����޸�
		-֧��ȫ��ģʽ����ֻ��������Ƶ�Ƚ�ʱ��֧�����Ҹ����before/after��ʽ
		-֧��Ctrl+UI�������Ƶ���Ƶ������Ctrl��ʱΪ������Ƶͳһ����
		-֧��<-��->��ݼ�����������Ƶ���ղ���ǰ��/���ˣ���ͣ����״̬��Ч
  3��YUV����������Ҫ��String_wxh.MN21���Ƹ�ʽ
  4��arccompareview_setting.txt�����ļ���λ��exe��ͬĿ¼��
     ShowDrawPanelΪ�Ƿ���ʾ��ǻ�����壬1Ϊ��ʾ��0Ϊ����ʾ
	 RGBColorSpaceΪ��ʾʱ�����ɫ�ռ䣬֧��JPG��BT601��BT709��BT2020��
	 ShowWatermarkΪ�Ƿ���ʾˮӡ��1Ϊ��ʾ��0Ϊ����ʾ��
  5����ݼ���
   Esc����	    		�Ƚ�ҳ�����˳�ȫ��ģʽ
   �ո��:	    		����������Ƶ����ͣ/����
   �������/�Ҽ�:	    �߼�����ģʽ����Ƶ�Ĳ���/����
   �����ϼ�/�¼���      ��֡����������ʱ���Ʋ���FPS����/��С
   Ctrl + �������/�Ҽ�:���ļ���ʱ���л�ͬһ�ļ����µ��ļ�
   Ctrl + �Ӻ�/���ţ�   ��ʾ���ݷŴ�/��С
   Ctrl + ����UI������  ����ͼ����

2.ArcConverter.exe
  1����Ҫ����
	1) ֧��YUV(NV21/NV12/I420/YV12/YUYV/I444/I422H/I422V/ARGB32/RGBA32/BGRA32/RGB24/BGR24/R5G6B5) ��ʽת��/��ת/����/����/Mirror/Crop
	2) ֧��PNG/JPG��ʽת��
	3) ֧��Raw2RGBת������metadata��
	4) ֧��BMPת��
	5) ֧��Depth2RGBת������metadata��
	6) ֧����Ƶ����ת����������/crf/gopszize/framerate/bitrate��
	7) ֧��ͼƬ�������Ƶ
	8) ֧��ͼƬ������
  2��setting.txt�����ļ�
	Ĭ�Ϲ���image��ʽ
	imagefilter=*.jpg *.png *.bmp *.nv21 *.nv12 *.i420 *.yv12 *.yuyv *.i444 *.i422h *.i422v *.rgba32 *.bgra32 *.argb32 *.bgr24 *.rgb24 *..R5G6B5 *.B5G6R5 *.gray *.GRAY16 *.p010_lsb *.p010_msb 
	*.RAW10RGGB10 *.RAW10GRBG10 *.RAW10GBRG10 *.RAW10BGGR10 *.RAW12RGGB12 *.RAW12GRBG12 *.RAW12GBRG12 *.RAW12BGGR12 *.RAW14RGGB14 *.RAW14GRBG14 *.RAW14GBRG14 *.RAW14BGGR14 
	*.RAW10RGGB16 *.RAW10GRBG16 *.RAW10GBRG16 *.RAW10BGGR16 *.RAW12RGGB16 *.RAW12GRBG16 *.RAW12GBRG16 *.RAW12BGGR16 *.RAW14RGGB16 *.RAW14GRBG16 *.RAW14GBRG16 *.RAW14BGGR16
	*.MI420 *.MNV21 *.MYV12 *.MNV12 *.MYUYV *.MGRAY *.MGRAY16 *.MP010_LSB *.MP010_MSB
	*.depth16
	Ĭ�Ϲ���video��ʽ
	videofilter=*.mp4 *.avi *.mov *.mpg *.wmv
	֧�ֵ�image��ʽת���б�
	formats=.nv21 .jpg .jpeg .png .nv12 .i420 .yv12 .yuyv .i444 .rgb32 .bgr32 .bgr24 .rgb24 .gray8 .p010be .p010le
	֧�ֵ�Rotate�б�
	rotate=0 90 180 270
	֧�ֵ�Scale�б�
	scale=0.5 1.0 1.5 2.0 2.5 3.0
	֧�ֵ�Mirror�б�
	mirror=NO Horizontal Vertical
	֧�ֵ�video codeid�б�
	codeid=H.264 H.265
	֧�ֵ�video fps�б�
	fps=AUTO 15 24 30 60 120
	֧�ֵ�video bitrate�б�
	bitrate=AUTO

	RGBColorSpaceΪYUV��RGBת��ʱ�����ɫ�ռ䣬֧��JPG��BT601��BT709��BT2020��