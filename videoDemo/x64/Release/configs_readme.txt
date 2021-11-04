[input]
//输入文件所在路径
input_folder=E:\Windows\converter\windows\1_0\bin\TestData\video_img
//0：按文件名后缀过滤，1：按正则表达式过滤
file_filter_type=0
//文件名后缀
file_ext_filter=.jpg
//正则表达式
file_regex_filter=[a-z0-9A-Z]+_[a-z0-9A-Z]+_[\d]+(_[\w|\.]*)*
delim=_

[process]
//是否进行单元测试，根据输入图片所在路径遍历所有图进行格式转换/crop/rotate/mirrorm=,下面的参数将无效
do_unit_test=1
//do_unit_test=0，特定参数处理
format=nv21
downsale=1.5
mirror=1
rotate=90

//是否处理video
do_video=1
//是否是但张图编码成视频
do_encode_video=1
//结果后缀名：容器
container_name=mp4
//编码器：H264=1，H265=2
codec_id=2
//crf参数
crf=28
//gopsize
gop_size=12
//
/*****************************************
	AISF_COLOR_RANGE_UNSPECIFIED =0,
	AISF_COLOR_RANGE_LIMITED=1,
	AISF_COLOR_RANGE_FULL,
*****************************************/
color_range=0

/****************************************
	AISF_COLOR_PRI_RESERVED0 = 0,
	AISF_COLOR_PRI_BT709 = 1,
	AISF_COLOR_PRI_UNSPECIFIED = 2,
	AISF_COLOR_PRI_BT601_625 = 5,
	AISF_COLOR_PRI_BT601_525 = 6,
	AISF_COLOR_PRI_BT2020 = 9, 
****************************************/
color_primary=0

/*****************************************
	AISF_COLOR_SPACE_RGB = 0,
	AISF_COLOR_SPACE_BT709 = 1,
	AISF_COLOR_SPACE_UNSPECIFIED = 2,
	AISF_COLOR_SPACE_BT601_625 = 5,
	AISF_COLOR_SPACE_BT601_525 = 6,
	AISF_COLOR_SPACE_BT2020_NCL = 9, 
	AISF_COLOR_SPACE_BT2020_CL = 10, 
*****************************************/	
color_space=0

/****************************************
	AISF_COLOR_TRS_RESERVED0 = 0,
	AISF_COLOR_TRS_BT709 = 1,
	AISF_COLOR_TRS_UNSPECIFIED = 2,
	AISF_COLOR_TRS_BT601 = 6,
	AISF_COLOR_TRS_SMPTE2084 = 16,
****************************************/	
color_transfer=0

framerate=30


[output]
output_folder=E:\Windows\converter\windows\1_0\bin\TestData\res_video
save_src_name=0
file_regex_filter=[a-z0-9A-Z]+_[a-z0-9A-Z]+_[\d]+(_[\w|\.]*)*
file_type=IMG
group_uid=20201014
file_index=0


