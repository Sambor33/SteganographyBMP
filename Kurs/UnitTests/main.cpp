#include <UnitTest++/UnitTest++.h>
#include "Steganography.h"
#include "Exception.h"
SUITE (DegreeCheck)
{
    TEST (CorrectDegree) {
        string key ("2");
        Steganography testdegree(key);
        CHECK(true);
    }
    TEST(EmptyDegree) {
        string key ("");
        CHECK_THROW(Steganography testdegree(key),StegException);
    }
    TEST(CharsInDegree) {
        string key ("3d");
        CHECK_THROW(Steganography testdegree(key),StegException);
    }
    TEST(SpaceInDegree) {
        string key ("1 0");
        CHECK_THROW(Steganography testdegree(key),StegException);
    }
    TEST(PunctCharsInDegree) {
        string key ("2!");
        CHECK_THROW(Steganography testdegree(key),StegException);
    }
    TEST(InvalidDegree) {
        string key ("3");
        CHECK_THROW(Steganography testdegree(key),StegException);
    }
}
struct Degree {
    Steganography *d;
    Degree ()
    {
        d = new Steganography("2");
    }
    ~Degree ()
    {
        delete d;
    }
};
SUITE (ContainerFileCheck)
{
    TEST_FIXTURE (Degree,CorrectContainerFile) {
        string BMP = "image.bmp";
        string Hidden = "testfile.txt";
        d->Concealment (BMP,Hidden);
        CHECK(true);
    }
    TEST_FIXTURE (Degree,InvalidContainerFormat) {
        string BMP = "imagejpg.jpg";
        string Hidden = "testfile.txt";
        CHECK_THROW(d->Concealment(BMP,Hidden),StegException);
    }
    TEST_FIXTURE (Degree,NonexistentContainerFile) {
        string BMP = "nonexistentimage.bmp";
        string Hidden = "testfile.txt";
        CHECK_THROW(d->Concealment(BMP,Hidden),StegException);
    }
}
SUITE (HiddenFileCheck)
{
    TEST_FIXTURE (Degree,CorrectHiddenFile) {
        string BMP = "image.bmp";
        string Hidden = "testfile.txt";
        d->Concealment (BMP,Hidden);
        CHECK(true);
    }
    TEST_FIXTURE (Degree,NonexistentHiddenFile) {
        string BMP = "image.bmp";
        string Hidden = "nonexistentfile";
        CHECK_THROW(d->Concealment (BMP,Hidden),StegException);
    }
    TEST_FIXTURE (Degree,TooMuchBytesToConceal) {
        string BMP = "image.bmp";
        string Hidden = "BIGTESTFILE.txt";
        CHECK_THROW(d->Concealment(BMP,Hidden),StegException);
    }
}

SUITE (HiddenMessageCheck)
{
    TEST_FIXTURE (Degree,TooMuchBytesToExtract) {
        string BMP = "image.bmp";
        string Hidden = "another.txt";
        string MessageSize = "4726";
        CHECK_THROW(d->Extraction(BMP,Hidden,MessageSize),StegException);
    }
    TEST_FIXTURE (Degree,EmptyNumberofExctractBytes) {
        string BMP = "image.bmp";
        string Hidden = "another.txt";
        string MessageSize = "";
        CHECK_THROW(d->Extraction (BMP,Hidden,MessageSize),StegException);
    }
    TEST_FIXTURE (Degree,IncorrectNumberofExctractBytes) {
        string BMP = "image.bmp";
        string Hidden = "another.txt";
        string MessageSize = "десять";
        CHECK_THROW(d->Extraction (BMP,Hidden,MessageSize),StegException);
    }
    TEST_FIXTURE (Degree,CorrectHiddenMessage) {
        string BMP = "image.bmp";
        string Hidden = "another.txt";
        string MessageSize = "70";
        d->Extraction(BMP,Hidden,MessageSize);
        CHECK(true);
    }
}
SUITE (CorrectProgrammWork)
{
    TEST_FIXTURE (Degree,CorrectProgrammWork) {
        string BMP = "image.bmp";
        string Hidden = "testfile.txt";
        string NewFile = "newfile.txt";
        fstream hidden;
        hidden.open (Hidden,fstream::in|fstream::app|fstream::binary);
        hidden.seekg(0,ios::end);
        int HiddenSize = hidden.tellg();
        hidden.seekg(0);
        hidden.close();
        string MessageSize = to_string(HiddenSize);
        d->Concealment(BMP,Hidden);
        d->Extraction (BMP,NewFile,MessageSize);
        StegException::FileComparsion(Hidden,NewFile);
        CHECK(true);
    }
}
int main()
{
    return UnitTest::RunAllTests();
}
