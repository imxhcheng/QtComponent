#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <math.h>

#include <sys/time.h>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/interface.h>

using namespace std;
using namespace cv;

#define CV_8UC1   CV_MAKETYPE(CV_8U,1)

    unsigned char  sbox[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
  };
  unsigned char mat[16]={
   0x32,0x88,0x31,0xe0,
   0x43,0x5a,0x31,0x37,
   0xf6,0x30,0x98,0x07,
   0xa8,0x8d,0xa2,0x34
   };
   unsigned char iv[16]={
   0x32,0x88,0x31,0xe0,
   0x43,0x5a,0x31,0x37,
   0xf6,0x30,0x98,0x07,
   0xa8,0x8d,0xa2,0x34
   };
   unsigned char key[16]={
   0x2b,0x28,0xab,0x09,
   0x7e,0xae,0xf7,0xcf,
   0x15,0xd2,0x15,0x4f,
   0x16,0xa6,0x88,0x3c
   };
   
   uint8_t rcon[4][10]={
    {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36}, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

unsigned char SubmatrixResult[16];
void SubBytes(unsigned char subMatrix[16])  
{  
    for(int i=0; i<16; i++)  
    {  
    
      unsigned char r = subMatrix[i] & 0xf;
      unsigned char l = (subMatrix[i]>>4) & 0xf;
        SubmatrixResult[i] = sbox[l][r];   
    }  
} 

unsigned char ShiftmatrixResult[16];
void ShiftRows(unsigned char mtx[16])  
{  
   for(int i=0;i<16;i++){
     ShiftmatrixResult[i]=mtx[i];
   }
    
    unsigned char temp = ShiftmatrixResult[4];  
    for(int i=0; i<3; ++i)  
        ShiftmatrixResult[i+4] = ShiftmatrixResult[i+5];  
    ShiftmatrixResult[7] = temp;  
    
    for(int i=0; i<2; ++i)  
    {  
        temp = ShiftmatrixResult[i+8];  
        ShiftmatrixResult[i+8] = ShiftmatrixResult[i+10];  
        ShiftmatrixResult[i+10] = temp;  
    }  
    
    temp = ShiftmatrixResult[15];  
    for(int i=3; i>0; --i)  
        ShiftmatrixResult[i+12] = ShiftmatrixResult[i+11];  
    ShiftmatrixResult[12] = temp; 
    
}  
unsigned char MixtmatrixResult[16];
unsigned char arr1[4];


unsigned char Multiplication(unsigned char v1,unsigned char v2){
  unsigned char result=0x00;

  unsigned char hi=v1 & 0x80;

    if(v2==0x02)
    result=v1<<1;
    if(v2==0x03){
      unsigned char rr1;
      rr1=v1<<1;
      result=rr1^v1;
    }
  if(hi==0x80)
  result = result^0x1B;
  
  return result;
}
void MixCoulmns(unsigned char matrix[16]){
  
 
    for(int i=0; i<4; ++i)  
    {  
        for(int j=0; j<4; ++j){
          arr1[j] = matrix[i+j*4]; 
        } 
       
MixtmatrixResult[i] = ((Multiplication(arr1[0],0x02))^(Multiplication(arr1[1],0x03))^(arr1[2])^(arr1[3])) & (0x0ff);

MixtmatrixResult[i+4] =((arr1[0])^(Multiplication(arr1[1],0x02))^(Multiplication(arr1[2],0x03))^(arr1[3])) & (0x0ff);

MixtmatrixResult[i+8] = ((arr1[0])^(arr1[1])^(Multiplication(arr1[2],0x02))^(Multiplication(arr1[3],0x03))) & (0x0ff);

MixtmatrixResult[i+12] = ((Multiplication(arr1[0],0x03))^(arr1[1])^(arr1[2])^(Multiplication(arr1[3],0x02))) & (0x0ff);

    
    } 

}
unsigned char  addrResult[16];
void AddRoundKey(unsigned char  matrix[16],unsigned char  keyy[16]){
  unsigned char arr2;
  unsigned char arr3;
    int a=0;
   for(int i=0; i<4; ++i)  
    {  
        for(int j=0; j<4; ++j){
          
          arr2 = matrix[i+j*4];
          arr3 = keyy[i+j*4];
          addrResult[i+j*4] = (arr2 ^ arr3);
          a++;
        } 
         
    }

}


uint8_t ColIndex = 0;
Mat new_key(4,4,CV_8UC1);
unsigned char rr;

void KeyExpension(Mat My){
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (i == 0) //first column of the new_key
            {
                unsigned char r =((My.at<uint8_t>((j+1)%(4),4-1)) & 0xf);
                unsigned char l = ((My.at<uint8_t>((j+1)%(4),4-1)>>4) & 0xf);
                rr = sbox[l][r];
                new_key.at<uint8_t>(j,i) = My.at<uint8_t>(j,i) ^ rr ^ rcon[j][ColIndex];
            }
            else
            {
                new_key.at<uint8_t>(j,i) = My.at<uint8_t>(j,i) ^ new_key.at<uint8_t>(j,i-1);
            }
        }
    }
    ColIndex++;
}
unsigned char EncryptedResult[16];
unsigned char copyKey[16]={
   0x2b,0x28,0xab,0x09,
   0x7e,0xae,0xf7,0xcf,
   0x15,0xd2,0x15,0x4f,
   0x16,0xa6,0x88,0x3c
   };
   
void AESOperations(unsigned char mat[16]){

 
AddRoundKey(mat,key);

int a=0;
ColIndex=0;

for(int i=0;i<9;i++){
SubBytes(addrResult);
ShiftRows(SubmatrixResult);
MixCoulmns(ShiftmatrixResult);

Mat My(4,4, CV_8UC1);
memcpy(My.data, key, 4*4*sizeof(unsigned char));

KeyExpension(My);

for(int j=0;j<4;j++){
  for(int k=0;k<4;k++){
    key[a] = new_key.at<uint8_t>(j,k);
    a++;
  }
}
a=0;


AddRoundKey(MixtmatrixResult,key);

}

SubBytes(addrResult);
ShiftRows(SubmatrixResult);
Mat My(4,4, CV_8UC1);
memcpy(My.data, key, 4*4*sizeof(unsigned char));
KeyExpension(My);

for(int j=0;j<4;j++){
  for(int k=0;k<4;k++){
    key[a] = new_key.at<uint8_t>(j,k);
    a++;
  }
}
a=0;

AddRoundKey(ShiftmatrixResult,key);

for(int i=0;i<16;i++){
    EncryptedResult[i]=addrResult[i];
    key[i] = copyKey[i];
}
}

Mat CropFourXFourImagePortion(Mat data, int x, int y){
    Mat block(4,4,CV_8UC1);
    block = Mat::zeros(4,4,CV_8UC1);
    for (int i=y; i<(y+4); i++){
        for (int j=x; j<(x+4); j++){
            if (i<data.rows && j<data.cols){
                block.at<uint8_t>(i-y,j-x) = data.at<uint8_t>(i,j);
            }
        }
    }
    return block;
}
void MergInImageSizeMatrix(Mat * destination, Mat source, int x, int y){
    for (int i=y; i<(y+4); i++){
        for (int j=x; j<(x+4); j++){
            destination->at<uint8_t>(i,j) = source.at<uint8_t>(i-y,j-x);
        }
    }
}
Mat PassRGBECB(Mat block){
Mat crop,encrypted_block = Mat::zeros(block.rows-(block.rows%4)+(block.rows%4 ? 4 : 0),block.cols-(block.cols%4)+(block.cols%4 ? 4 : 0),CV_8UC1);

    for(int y_axis=0; y_axis <block.rows; y_axis+=4){
        for (int x_axis=0; x_axis < block.cols; x_axis+=4){
          
             crop = CropFourXFourImagePortion(block, x_axis, y_axis); 

            int a=0;
             for(int j=0;j<4;j++){
                for(int k=0;k<4;k++){
                mat[a] = crop.at<uint8_t>(j,k);
                a++;
                }
            }
            a=0;
            AESOperations(mat);
            Mat cropp(4,4, CV_8UC1);
            memcpy(cropp.data, EncryptedResult, 4*4*sizeof(unsigned char));

            MergInImageSizeMatrix(&encrypted_block, cropp, x_axis, y_axis);
          
        }
       
}

return encrypted_block;
}
Mat PassRGBCBC(Mat block){
Mat crop,encrypted_block = Mat::zeros(block.rows-(block.rows%4)+(block.rows%4 ? 4 : 0),block.cols-(block.cols%4)+(block.cols%4 ? 4 : 0),CV_8UC1);

    for(int y_axis=0; y_axis <block.rows; y_axis+=4){
        for (int x_axis=0; x_axis < block.cols; x_axis+=4){
          
             crop = CropFourXFourImagePortion(block, x_axis, y_axis); 

            int a=0;
             for(int j=0;j<4;j++){
                for(int k=0;k<4;k++){
                mat[a] = crop.at<uint8_t>(j,k);
                a++;
                }
            }
            a=0;
            AddRoundKey(mat,iv);
            AESOperations(addrResult);
            Mat cropp(4,4, CV_8UC1);
            memcpy(cropp.data, EncryptedResult, 4*4*sizeof(unsigned char));

            MergInImageSizeMatrix(&encrypted_block, cropp, x_axis, y_axis);
          
        }
       
}

return encrypted_block;
}

void SavinginTextfile(Mat data, const char * file){
    fstream file_handler;
    file_handler.open(file, ios::out | ios::binary | ios::trunc);

    int number_of_char = data.rows * data.cols;
    char str[number_of_char];

    int k=0;
    if (file_handler.is_open()){
        string data_to_string;
        for (int i=0; i<data.cols; i++){
            for (int j=0; j<data.rows; j++){
                if (i >= data.cols-4 && data.at<uint8_t>(j,i) == 0){
                    break;
                }
                str[k] = data.at<uint8_t>(j,i); 
                k++;
            }
        }
        file_handler.write(str,k);
        file_handler.close(); 
    }
}
int main()
{
    char CBCECB;
    cout<<"You want to do CBC press c/C or ECB press e/E "<<endl;
    cin>>CBCECB;
    if(CBCECB=='c' || CBCECB=='C'){
    char imagetext;
    cout<<"Enter i/I for image Encryption t/T for text Encryption "<<endl;
    cin>>imagetext;
    if(imagetext=='i' || imagetext=='I'){
    Mat src = imread("lenna.jpg",1);
   if(src.empty()){ 
    cout<<"Cannot load image....!"<<endl;
    return -1;
    }
imshow("lenna",src);
vector<Mat> spl(3);
split(src,spl);
Mat SeparateChannResult[3];

for (int i=0; i<3; i++){
        SeparateChannResult[i] = PassRGBCBC(spl[i]);    
    }
Mat encrypted_image;

    merge(SeparateChannResult, 3, encrypted_image);
    imwrite( "EncImageCBC.png", encrypted_image );
    imshow("encrypted image", encrypted_image);
    }
    
    else if(imagetext=='t' || imagetext=='T'){
        Mat encrypted_data;
        std::ifstream file("myfile.txt");
        fstream file_handler;
    file_handler.open("myfile.txt",ios::in | ios::binary);

    if (file_handler.is_open()){
        ////////// count number of characters //////////
        int number_of_char=0;
        char a;
        file_handler.read(&a,1);
        number_of_char++;
        while (!file_handler.eof()){
        number_of_char++;
        file_handler.read(&a,1);
        }
        file_handler.clear();
        file_handler.seekg(0,ios::beg);
        ////////////////////////////////////////////////
        int number_of_columns = ceil(number_of_char/4);
        char str[number_of_char];
        file_handler.read(str,number_of_char);
         Mat data = Mat::zeros(4, number_of_columns,CV_8UC1); 
        int k=0;
        for (int i=0; i<number_of_columns; i++){
            for (int j=0; j<4; j++){
                data.at<uint8_t>(j,i) = (uint8_t)str[k];
                k++;
            }
        }
        encrypted_data = PassRGBCBC(data);
     //   cout<<hex<<data<<endl;
        const char * encrypted_file_name = "encrypteddataCBC.txt";
        SavinginTextfile(encrypted_data,encrypted_file_name);
        file_handler.close();
        
   //     cout<<hex<<encrypted_data;
        }
    }
    else{
        cout<<"Program terminated "<<endl;
    }
    }
    else if(CBCECB=='e' || CBCECB=='E'){
        char imagetext;
    cout<<"Enter i/I for image Encryption t/T for text Encryption "<<endl;
    cin>>imagetext;
    if(imagetext=='i' || imagetext=='I'){
    Mat src = imread("lenna.jpg",1);
   if(src.empty()){ 
    cout<<"Cannot load image....!"<<endl;
    return -1;
    }
imshow("lenna",src);
vector<Mat> spl(3);
split(src,spl);
Mat SeparateChannResult[3];

for (int i=0; i<3; i++){
        SeparateChannResult[i] = PassRGBECB(spl[i]);    
    }
Mat encrypted_image;

    merge(SeparateChannResult, 3, encrypted_image);
    imwrite( "EncImageECB.png", encrypted_image );
    imshow("encrypted image", encrypted_image);
    }
    
    else if(imagetext=='t' || imagetext=='T'){
        Mat encrypted_data;
        std::ifstream file("myfile.txt");
        fstream file_handler;
    file_handler.open("myfile.txt",ios::in | ios::binary);

    if (file_handler.is_open()){
        ////////// count number of characters //////////
        int number_of_char=0;
        char a;
        file_handler.read(&a,1);
        number_of_char++;
        while (!file_handler.eof()){
        number_of_char++;
        file_handler.read(&a,1);
        }
        file_handler.clear();
        file_handler.seekg(0,ios::beg);
        ////////////////////////////////////////////////
        int number_of_columns = ceil(number_of_char/4);
        char str[number_of_char];
        file_handler.read(str,number_of_char);
         Mat data = Mat::zeros(4, number_of_columns,CV_8UC1); 
        int k=0;
        for (int i=0; i<number_of_columns; i++){
            for (int j=0; j<4; j++){
                data.at<uint8_t>(j,i) = (uint8_t)str[k];
                k++;
            }
        }
        encrypted_data = PassRGBECB(data);
     //   cout<<hex<<data<<endl;
        const char * encrypted_file_name = "encrypteddataECB.txt";
        SavinginTextfile(encrypted_data,encrypted_file_name);
        file_handler.close();
        
   //     cout<<hex<<encrypted_data;
        }
    }
    else{
        cout<<"Program terminated "<<endl;
    }
    }
    else
    {
        cout<<"Mode not choose"<<endl;
    }
    
    cout<<"Your Encryption is completed "<<endl;
   waitKey(1000000);


}