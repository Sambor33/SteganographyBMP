/**
* @file Exception.h
* @author Самборский И.С.
* @version 1.0
* @brief Описание класса StegException
* @date 30.05.2021
* @copyright ИБСТ ПГУ
*/
#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <cmath>
#include <locale>
using namespace std;
///@brief Собственный класс исключений
class StegException: public exception
{
private:
    ///@brief поле ошибки, в котором указывается ее вид.
    string error;
    ///@brief поле ошибки, в котором указывается ее описание.
    string description;
public:
    /** 
    * @brief Конструктор инициализации ошибки строками.
    * @param err - вид ошибки.
    * @param desc - описание ошибки.
    * @details err присваивается приватному полю error,а desc присваивавется приватному полю description.
    **/
    StegException (const string&err,const string&desc);
    ///@brief Метод, возвращающий информацию об ошибке.
    string what () noexcept;
    /**
     * @brief Метод проверки степени кодирования.
     * @param key - степень кодирования, введенная пользователем для выполнения операций сокрытия/извлечения.
     * @details Бросает исключение в случае если: строка с вводом степени пустая, степень содержит символы, не 
     * являющиеся цифрами, степень не является одним из перечисленных чисел: 1/2/4/8
     **/
    static bool DegreeCheck (const string&key);
    /**
     * @brief Метод проверки файла на существование/корректный ввод пути.
     * @param filepath - путь к файлу.
     * @details Бросает исключение в случае если: файл по заданному пути не открывается.
     **/
    static bool FileCheck (const string&filepath);
    /**
     * @brief Метод проверки файла-контейнера на формат BMP.
     * @param bmp_path - путь к файлу-контейнеру.
     * @details Функция проверят первые 2 байта файла на то,составляют ли они комбинацию BM. В случае, если нет, бросается исключение.
     **/
    static bool BMPCheck (const string&bmp_path);
    /**
     * @brief Метод проверки на возможность скрыть данные из файла в контейнере при данной степени кодировки.
     * @param bmp_path - путь к файлу-контейнеру.
     * @param hidden_path - путь к файлу, данные из которого будут скрываться.
     * @param key - степень кодирования.
     * @details Метод вычисляет объем файла-контейнера и файла, данные из которого будут скрываться и проверяет, может ли в контейнер
     * быть помещен объем файла со скрываемой информацией при заданной степени кодировки. Если не может, выбрасывается исключение.
     **/
    static bool ConFileSizeCheck(const string&bmp_path,const string&hidden_path,const int&key);
    /**
     * @brief Метод проверки на возможность извлечь из контейнера определенное количество байтов при данной степени кодировки.
     * @param bmp_path - путь к файлу-контейнеру.
     * @param hidden_path - путь к файлу, данные из которого будут скрываться.
     * @param key - степень кодирования.
     * @param hiddensize - количество байтов для извлечения (вводится пользователем).
     * @details Первоначально метод проверяет строку с введеным количеством байтов для извлечения на корректность ввода: если строка
     * пустая или содержит символы не являющиеся цифрами выбрасывается исключение. Далее метод вычисляет объем файла-контейнера и 
     * максимальное количество байт,доступное для извлечения при данной степени кодировки. Если заданное пользователем число байт на
     * извлечение больше чем вычисленное максимально допустимое число байт для извлечения, то бросается исключение.
     **/
    static bool ExFileSizeCheck(const string&bmp_path,const string&hidden_path,const int&key,const string&hiddensize);
    /**
     * @brief Метод проверки нового файла для записи в него извлеченных данных из контейнера на существование/корректный ввод пути.
     * @param hidden_path - путь к новому файлу.
     * @details Бросает исключение в случае если: файл по заданному пути не открывается (не создается).
     **/
    static bool NewFileCheck(const string&hidden_path);
    /**
     * @brief Метод для тестирования корректности работы функций сокрытия/извлечения.
     * @param hidden_path - путь к файлу, данные из которого будут скрываться в контейнере.
     * @param newfile_path - путь к новому файлу, куда записываются извлеченные данные из контейнера.
     * @details В тестовом сценарии, где используется данная функция выполняется пара операций: сокрытие и извлечение. Далее 
     * содержимое файла, данные из которого скрывались, и содержимое файла, в который были записаны скрываемые данные из контейнера 
     * сравниваются на полное совпадение. В случае если если файлы не равны выбрасывается исключение, не дающее завершить тест успешно.
     **/
    static bool FileComparsion (const string&hidden_path,const string&newfile_path);
};