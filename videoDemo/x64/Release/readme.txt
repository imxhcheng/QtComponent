产品介绍：
1.ArcCompareView.exe  OPENGL版本至少要在3.3以上
  用于图片比较和Video视频的播放比较，支持普通的视频格式，YUV数据流。
  1、支持1个，2个，3个或4个图片/视频的比较播放，支持拖动打开。单文件打开时，支持在文件所在目录切换文件，支持快捷键切换文件（Ctrl+<-和Ctrl+->）。
  2、播放模式有普通播放模式和高级播放模式两种
     1）普通播放模式为视频自由播放，
	    -不支持帧同步，
		-支持视频播放/暂停，重播
		-支持视频图像放大，缩小，1:1， Fitin，旋转
		-支持Ctrl+UI操作控制单视频，不按Ctrl键时为所有视频统一控制
		-支持空格键进行所有视频的暂停/播放
		-两个视频比较时，暂停状态支持覆盖式的before/after,按下B|A按钮时，显示另一个视频正在播放的数据，放开B|A按钮时显示当前视频数据。
	 2）高级播放模式为视频同步播放
	    -支持帧同步
		-支持视频播放/暂停，重播
		-支持视频图像放大，缩小，1:1， Fitin，旋转		
		-支持空格键进行所有视频的暂停/播放
		-两个视频比较时，暂停状态支持覆盖式的before/after,按下B|A按钮时，显示另一个视频正在播放的数据，放开B|A按钮时显示当前视频数据。
		-支持视频按照步长前进/后退，只在暂停播放状态起效，步长可编辑，输入步长后，回车或者前进后退确认步长修改
		-支持全屏模式，当只有两个视频比较时，支持左右各半的before/after形式
		-支持Ctrl+UI操作控制单视频，不按Ctrl键时为所有视频统一控制
		-支持<-和->快捷键控制所有视频按照步长前进/后退，暂停播放状态有效
  3、YUV数据流的需要是String_wxh.MN21类似格式
  4、arccompareview_setting.txt配置文件，位于exe相同目录下
     ShowDrawPanel为是否显示标记画框面板，1为显示，0为不显示
	 RGBColorSpace为显示时候的颜色空间，支持JPG，BT601，BT709和BT2020。
	 ShowWatermark为是否显示水印，1为显示，0为不显示。
  5、快捷键：
   Esc键：	    		比较页面下退出全屏模式
   空格键:	    		控制所有视频的暂停/播放
   方向左键/右键:	    高级播放模式下视频的步退/步进
   方向上键/下键：      多帧数据流播放时控制播放FPS增加/减小
   Ctrl + 方向左键/右键:单文件打开时，切换同一文件夹下的文件
   Ctrl + 加号/减号：   显示数据放大/缩小
   Ctrl + 所有UI操作：  单视图操作

2.ArcConverter.exe
  1、主要功能
	1) 支持YUV(NV21/NV12/I420/YV12/YUYV/I444/I422H/I422V/ARGB32/RGBA32/BGRA32/RGB24/BGR24/R5G6B5) 格式转换/旋转/缩放/镜像/Mirror/Crop
	2) 支持PNG/JPG格式转换
	3) 支持Raw2RGB转换（需metadata）
	4) 支持BMP转换
	5) 支持Depth2RGB转换（需metadata）
	6) 支持视频参数转换（编码器/crf/gopszize/framerate/bitrate）
	7) 支持图片编码成视频
	8) 支持图片重命名
  2、setting.txt配置文件
	默认过滤image格式
	imagefilter=*.jpg *.png *.bmp *.nv21 *.nv12 *.i420 *.yv12 *.yuyv *.i444 *.i422h *.i422v *.rgba32 *.bgra32 *.argb32 *.bgr24 *.rgb24 *..R5G6B5 *.B5G6R5 *.gray *.GRAY16 *.p010_lsb *.p010_msb 
	*.RAW10RGGB10 *.RAW10GRBG10 *.RAW10GBRG10 *.RAW10BGGR10 *.RAW12RGGB12 *.RAW12GRBG12 *.RAW12GBRG12 *.RAW12BGGR12 *.RAW14RGGB14 *.RAW14GRBG14 *.RAW14GBRG14 *.RAW14BGGR14 
	*.RAW10RGGB16 *.RAW10GRBG16 *.RAW10GBRG16 *.RAW10BGGR16 *.RAW12RGGB16 *.RAW12GRBG16 *.RAW12GBRG16 *.RAW12BGGR16 *.RAW14RGGB16 *.RAW14GRBG16 *.RAW14GBRG16 *.RAW14BGGR16
	*.MI420 *.MNV21 *.MYV12 *.MNV12 *.MYUYV *.MGRAY *.MGRAY16 *.MP010_LSB *.MP010_MSB
	*.depth16
	默认过滤video格式
	videofilter=*.mp4 *.avi *.mov *.mpg *.wmv
	支持的image格式转换列表
	formats=.nv21 .jpg .jpeg .png .nv12 .i420 .yv12 .yuyv .i444 .rgb32 .bgr32 .bgr24 .rgb24 .gray8 .p010be .p010le
	支持的Rotate列表
	rotate=0 90 180 270
	支持的Scale列表
	scale=0.5 1.0 1.5 2.0 2.5 3.0
	支持的Mirror列表
	mirror=NO Horizontal Vertical
	支持的video codeid列表
	codeid=H.264 H.265
	支持的video fps列表
	fps=AUTO 15 24 30 60 120
	支持的video bitrate列表
	bitrate=AUTO

	RGBColorSpace为YUV和RGB转换时候的颜色空间，支持JPG，BT601，BT709和BT2020。