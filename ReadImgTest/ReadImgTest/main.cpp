#include <QtCore/QCoreApplication>
#include <jpeglib.h>
#include <turbojpeg.h>
#include <time.h>
#include <QImage>
#include <QImageReader>
#include <windows.h>


std::unique_ptr<unsigned char[]> get_jpeg_decompress_data2(const char* image_name, int& width, int& height, int& channels)
{
	FILE* infile = fopen(image_name, "rb");
	if (infile == nullptr) {
		fprintf(stderr, "can't open %s\n", image_name);
		return nullptr;
	}

	fseek(infile, 0, SEEK_END);
	unsigned long srcSize = ftell(infile);
	std::unique_ptr<unsigned char[]> srcBuf(new unsigned char[srcSize]);
	fseek(infile, 0, SEEK_SET);
	fread(srcBuf.get(), srcSize, 1, infile);
	fclose(infile);

	tjhandle handle = tjInitDecompress();
	int subsamp, cs;
	int ret = tjDecompressHeader3(handle, srcBuf.get(), srcSize, &width, &height, &subsamp, &cs);
	if (cs == TJCS_GRAY) channels = 1;
	else channels = 3;

	int pf = TJCS_RGB;
	int ps = tjPixelSize[pf];
	std::unique_ptr<unsigned char[]> data(new unsigned char[width * height * channels]);
	ret = tjDecompress2(handle, srcBuf.get(), srcSize, data.get(), width, width * channels, height, TJPF_RGB, TJFLAG_NOREALLOC);

	tjDestroy(handle);

	return data;
}

std::unique_ptr<unsigned char[]> get_jpeg_decompress_data(const char* image_name, int& width, int& height, int& channels)
{
	FILE* infile = fopen(image_name, "rb");
	if (infile == nullptr) {
		fprintf(stderr, "can't open %s\n", image_name);
		return nullptr;
	}

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* Step 1: allocate and initialize JPEG decompression object */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* Step 2: specify data source (eg, a file) */
	jpeg_stdio_src(&cinfo, infile);
	/* Step 3: read file parameters with jpeg_read_header() */
	jpeg_read_header(&cinfo, TRUE);
	/* Step 4: set parameters for decompression */
	cinfo.scale_num = 1;
	cinfo.scale_denom = 4;
	/* Step 5: Start decompressor */
	jpeg_start_decompress(&cinfo);

	cinfo.out_color_space = JCS_RGB; //JCS_EXT_BGR;

	int row_stride = cinfo.output_width * cinfo.output_components;
	/* Output row buffer */
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	width = cinfo.output_width;
	height = cinfo.output_height;
	channels = cinfo.output_components;
	std::unique_ptr<unsigned char[]> data(new unsigned char[width * height * channels]);

	/* Step 6: while (scan lines remain to be read) */
	for (int j = 0; j < cinfo.output_height; ++j) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
		unsigned char* p = data.get() + j * row_stride;
		memcpy(p, buffer[0], row_stride);
	}

	/* Step 7: Finish decompression */
	jpeg_finish_decompress(&cinfo);
	/* Step 8: Release JPEG decompression object */
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	return data;
}


int test_libjpeg_turbo_decompress()
{
#ifdef _MSC_VER
	std::string image_path{ "D:\\素材\\2-MarkTool\\MarkTool_Image\\无法读取\\" };
#else
	std::string image_path{ "test_data/" };
#endif

	std::string image_name = image_path + "xiaomi_0.jpg";

	//===========================================================算法1
	int width, height, channels;
	DWORD now_time1,now_time2;
	now_time1 = GetTickCount();
	std::unique_ptr<unsigned char[]> data = get_jpeg_decompress_data(image_name.c_str(), width, height, channels);
	if (data == nullptr)
	{
		fprintf(stderr, "fail to decompress: %s\n", image_name.c_str());
		return -1;
	}
	now_time2 = GetTickCount();


	//fprintf(stdout, "decompress time 1: %lldms, width: %d, height: %d, channels: %d\n", now_time2 - now_time1, width, height, channels);
	//std::string result_image = image_path + "result_tirg.png";
	QImage img(data.get(), width, height, QImage::Format_RGB888);
	if (!img.isNull())
	{	
		fprintf(stdout, "decompress QImage time 1: %lldms,img Size: %d,%d\n", now_time2 - now_time1,img.width(),img.height());
	}

	img.save("1.jpg");

	//==========================================================算法2
	int width2, height2, channels2;
	DWORD now_time3, now_time4;
	now_time3 = GetTickCount();
	std::unique_ptr<unsigned char[]> data2 = get_jpeg_decompress_data2(image_name.c_str(), width2, height2, channels2);
	if (data2 == nullptr) 
	{
		fprintf(stderr, "fail to decompress: %s\n", image_name.c_str());
		return -1;
	}
	now_time4 = GetTickCount();


	//fprintf(stdout, "decompress time 2: %lldms, width2: %d, height2: %d, channels2: %d\n", now_time4 - now_time3, width2, height2, channels2);
	//std::string result_image2 = image_path + "result_tirg2.png";
	QImage img2(data2.get(), width, height, QImage::Format_RGB888);
	if (!img2.isNull())
	{
		fprintf(stdout, "decompress QImage time 2: %lldms,img Size: %d,%d\n", now_time4 - now_time3, img2.width(), img2.height());
	}
	img.save("2.jpg");



	//===========================================================算法3
	DWORD now_time5,now_time6;
	QImageReader read(QString::fromLocal8Bit(image_name.c_str()));
	now_time5 = GetTickCount();
	QImage img3 = read.read();
	now_time6 = GetTickCount();
	if (!img3.isNull())
	{
		fprintf(stdout, "decompress QImage time 3: %lldms,img Size: %d,%d\n", now_time6 - now_time5, img3.width(), img3.height());
	}

	img3.save("3.jpg");
	
	return 0;
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	test_libjpeg_turbo_decompress();

    return a.exec();
}
