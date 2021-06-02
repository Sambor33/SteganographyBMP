#include <Steganography.h>
#include <Exception.h>
int main()
{
    setlocale(LC_ALL, "Russian");
    int operation;
    string help = "Справка о работе программы:\nСокрытие - операция сокрытия информации в файле-контенере формата BMP.\nИзвлечение - операция извлечения информации из файла-контейнера формата BMP.\nВыход - завершить работу программы.\n";
    string key;
    do {
        cout<<"Введите операцию : 1 - Сокрытие , 2 - Извлечение , 3 - Справка о работе программы , 0 - Выход = ";
        cin>>operation;
        if (operation==1) {
            try {
                cout<<"Введите степень кодирования (1/2/4/8) = ";
                cin.ignore();
                getline(cin,key);
                Steganography thisoperation(key);
                string bmp_path,hidden_path;
                cout<<"Введите путь к bmp-контейнеру : ";
                getline(cin,bmp_path);
                cout<<"Введите путь к файлу, данные из которого будут зашифровываться : ";
                getline(cin,hidden_path);
                thisoperation.Concealment (bmp_path,hidden_path);
                cout<<"Операция успешно выполнена!"<<endl;
            } catch (StegException&ex) {
                cout<<ex.what()<<endl;
            }
        }
        if (operation==2) {
            try {
                cout<<"Введите степень кодирования (1/2/4/8) = ";
                cin.ignore();
                cin>>key;
                Steganography thisoperation(key);
                string hiddensize;
                string bmp_path,hidden_path;
                cout<<"Введите путь к bmp-контейнеру с зашифрованными данными : ";
                cin.ignore();
                getline(cin,bmp_path);
                cout<<"Введите путь к файлу, в который будут записаны зашифрованные в контейнере данные : ";
                getline(cin,hidden_path);
                cout<<"Введите длину зашифрованного в контейнере файла (в байтах) : ";
                getline(cin,hiddensize);
                thisoperation.Extraction(bmp_path,hidden_path,hiddensize);
                cout<<"Операция успешно выполнена!"<<endl;
            } catch (StegException&ex) {
                cout<<ex.what()<<endl;
            }
        }
        if (operation==3) {
        cout<<help<<endl;
        }
        if (operation!=0 and operation!=1 and operation!=2 and operation!=3) {
        cout<<"Неизвестная операция, попробуйте еще раз!"<<endl;
        }
    } while (operation!=0);
}
