#include <Steganography.h>
#include <Exception.h>
Steganography::Steganography (const string&key)
{
    StegException::DegreeCheck(key);
    degree = stoi(key);
}
unsigned char Steganography::hmask ()
{
    unsigned char hidden_mask = 0b11111111;
    hidden_mask<<=(8-degree);
    return hidden_mask;
}
unsigned char Steganography::imask ()
{
    unsigned char img_mask = 0b11111111;
    img_mask>>=degree;
    img_mask<<=degree;
    return img_mask;
}
int Steganography::Concealment (const string&bmp_path,const string&hidden_path)
{
    fstream bmp;
    fstream hidden;
    StegException::FileCheck(bmp_path);
    StegException::FileCheck(hidden_path);
    StegException::BMPCheck (bmp_path);
    StegException::ConFileSizeCheck (bmp_path,hidden_path,degree);
    bmp.open (bmp_path,fstream::out| fstream::in | fstream::binary);
    hidden.open (hidden_path,fstream::in|fstream::binary);
    hidden.seekg(0,ios::end);
    int hiddensize = hidden.tellg();
    hidden.seekg(0);
    bmp.seekg(54);
    unsigned char Hidden;
    unsigned char Image;
    unsigned char HiddenByte;
    unsigned char ImageByte;
    for (int i=0; i<hiddensize; i++) {
        hidden.read (reinterpret_cast<char*>(&Hidden),sizeof(unsigned char));
        for (int i=0; i<8; i+=degree) {
            bmp.read(reinterpret_cast <char*>(&Image), sizeof(char));
            bmp.seekg(-1,fstream::cur);
            ImageByte = Image&imask();
            HiddenByte = Hidden&hmask();
            HiddenByte >>= (8-degree);
            ImageByte |= HiddenByte;
            bmp.write (reinterpret_cast <char*>(&ImageByte), sizeof(char));
            Hidden<<=degree;
        }
    }
    bmp.close ();
    hidden.close ();
    return 0;
}
int Steganography::Extraction (const string&bmp_path,const string&hidden_path,const string&hiddensize)
{
    fstream bmp;
    fstream hidden;
    StegException::FileCheck(bmp_path);
    StegException::NewFileCheck(hidden_path);
    StegException::BMPCheck (bmp_path);
    StegException::ExFileSizeCheck (bmp_path,hidden_path,degree,hiddensize);
    int DigitHiddenSize = stoi(hiddensize);
    bmp.open(bmp_path,fstream::in | fstream::binary);
    hidden.open(hidden_path,fstream::out | fstream::binary);
    unsigned char Image;
    unsigned char ImageByte;
    bmp.seekg(54);
    unsigned char image_mask = ~imask();
    for (int i=0; i<DigitHiddenSize; i++) {
        unsigned char Symbol = 0;
        for (int i=0; i<8; i+=degree) {
            bmp.read(reinterpret_cast <char*>(&Image),sizeof(char));
            ImageByte = Image&image_mask;
            Symbol<<=degree;
            Symbol |= ImageByte;
        }
        hidden.write (reinterpret_cast<char*>(&Symbol),sizeof(unsigned char));
    }
    bmp.close ();
    hidden.close ();
    return 0;
}
