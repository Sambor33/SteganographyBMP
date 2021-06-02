#include <Exception.h>
StegException::StegException (const string&err,const string&desc)
{
    error = err;
    description = desc;
}
string StegException::what () noexcept
{
    return "Ошибка : "+error+"Описание ошибки : "+description;
}
bool StegException::DegreeCheck (const string&key)
{
    int len = key.size();
    if (key.empty()) {
        throw StegException("Некорректный ввод степени кодирования: "," Пустая строка");
    }
    for (int i=0; i<len; i++) {
        if (!isdigit(key[i])) {
            throw StegException("Некорректный ввод степени кодирования: "," Были введены недопустимые символы");
        }
    }
    int digitkey = stoi(key);
    if (digitkey !=1 and digitkey !=2 and digitkey !=4 and digitkey !=8) {
        throw StegException("Некорректный ввод степени кодирования: "," Программой поддерживаются следующие степени кодирования: 1/2/4/8");
    }
    return 1;
}
bool StegException::FileCheck (const string&filepath)
{
    fstream file;
    file.open (filepath,fstream::in|fstream::out|fstream::binary);
    if (!file.is_open()) {
        throw StegException ("Ошибка открытия файла: "," Введен некорректный путь к файлу или такого файла не существует");
    }
    file.close();
    return 1;
}
bool StegException::BMPCheck (const string&bmp_path)
{
    fstream file;
    file.open (bmp_path,fstream::in|fstream::out|fstream::binary);
    char byte;
    file.read (reinterpret_cast<char*>(&byte),sizeof (char));
    if (byte!='B') {
        throw StegException ("Ошибка открытия файла: "," Файл-контейнер имеет недопустимый формат");
    }
    file.read (reinterpret_cast<char*>(&byte),sizeof (char));
    if (byte!='M') {
        throw StegException ("Ошибка открытия файла: "," Файл-контейнер имеет недопустимый формат");
    }
    file.close ();
    return 1;
}
bool StegException::ConFileSizeCheck(const string&bmp_path,const string&hidden_path,const int&key)
{
    fstream bmp,hidden;
    bmp.open (bmp_path,fstream::in|fstream::out|fstream::binary);
    hidden.open (hidden_path,fstream::in|fstream::out|fstream::binary);
    bmp.seekg(0,fstream::end);
    int BmpSize = bmp.tellg();
    bmp.seekg(0);
    hidden.seekg(0,fstream::end);
    int HiddenSize = hidden.tellg();
    hidden.seekg(0);
    double limit = ((BmpSize * key/8) - 54);
    if (HiddenSize>limit) {
        throw StegException ("Ошибка выполнения операции: "," Скрываемый файл невозможно спрятать в данном контейнере");
    }
    bmp.close();
    hidden.close();
    return 1;
}
bool StegException::NewFileCheck(const string&hidden_path)
{
    fstream newfile;
    newfile.open(hidden_path,fstream::out | fstream::binary);
    if (!newfile.is_open()) {
        throw StegException ("Ошибка создания файла: "," Не удалось создать файл для записи скрываемой информации");
    }
    return 1;
}
bool StegException::ExFileSizeCheck(const string&bmp_path,const string&hidden_path,const int&key,const string&hiddensize)
{
    int len = hiddensize.size();
    if (hiddensize.empty()) {
        throw StegException("Некорректный ввод длины спрятанного сообщения: "," Пустая строка");
    }
    for (int i=0; i<len; i++) {
        if (!isdigit(hiddensize[i])) {
            throw StegException("Некорректный ввод длины спрятанного сообщения: "," Были введены недопустимые символы");
        }
    }
    int DigitHiddenSize = stoi(hiddensize);
    fstream bmp;
    bmp.open (bmp_path,fstream::in|fstream::out|fstream::binary);
    bmp.seekg(0,fstream::end);
    int BmpSize = bmp.tellg();
    bmp.seekg(0);
    double limit  = ((BmpSize * key/8) - 54);
    if (DigitHiddenSize>limit) {
        throw StegException ("Ошибка выполнения операции: "," Невозможно извлечь данное кол-во байтов при данной степени кодировки");
    }
    bmp.close();
    return 1;
}
bool StegException::FileComparsion (const string&hidden_path,const string&newfile_path)
{
    fstream hidden,newfile;
    hidden.open (hidden_path,fstream::in|fstream::app|fstream::binary);
    newfile.open (newfile_path,fstream::in|fstream::app|fstream::binary);
    hidden.seekg(0,fstream::end);
    newfile.seekg(0,fstream::end);
    int HidSize = hidden.tellg();
    int NewSize = newfile.tellg();
    hidden.seekg(0);
    newfile.seekg(0);
    char *HidBuffer = new char [HidSize];
    char *NewBuffer = new char [NewSize];
    char Byte;
    for (int i=0; i<HidSize; i++) {
        hidden.read(reinterpret_cast<char*>(&Byte),sizeof(char));
        HidBuffer[i] = Byte;
    }
    for (int i=0; i<NewSize; i++) {
        newfile.read(reinterpret_cast<char*>(&Byte),sizeof(char));
        NewBuffer[i] = Byte;
    }
    for (int i=0; i<HidSize; i++) {
        if (HidBuffer[i]!=NewBuffer[i]) {
            throw StegException ("Ошибка логики работы программы: "," Файлы не совпадают");
        }
    }
    delete [] HidBuffer;
    delete [] NewBuffer;
    hidden.close();
    newfile.close();
    return 1;
}
