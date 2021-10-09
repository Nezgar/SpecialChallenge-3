#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <Windows.h>



struct BMPheader {
    WORD    bfType;
    LONG    biWidth;
    LONG    biHeight;
};

// Выводим черно-белую картнку BMP формата в виде цифр.
void getBMP(const char* fname) {
    /*  Вводим файл BMP в поток
    *   Считываем ширину и длинну в пикселях
    *   Создаем двумерный массив который равен ширине и длинне картинки 
    *   Назначаем цвета в цифрах черный=0, белый=1
    *   Считываем данные картинки. Записываем их в двумерный массив. Заполняем массив "белым" и "черным" цветами.
    *   Выводим полцчившийся массив(картинку в консоль)
    *   Удаляем двумерный массив
    */

    
    FILE* screen = fopen(fname, "rb");
    if (screen == NULL) perror("Не удалось открыть файл"); //смещение 54

    BMPheader bh;
    size_t res;

    res = fread(&bh, 1, sizeof(bh), screen);
    if (res != sizeof(BMPheader)) { 
        fclose(screen); 
        std::cout << "Не BMP файл"; 
    }

    fseek(screen, 18, SEEK_SET);
    fread(&bh.biWidth, sizeof(LONG), 1, screen);
   
    fseek(screen, 22, SEEK_SET);
    fread(&bh.biHeight, sizeof(LONG), 1, screen);
   
    LONG** photo = new LONG * [bh.biHeight];
    for (LONG i = 0; i < bh.biHeight; i++) {
        photo[i] = new LONG[bh.biWidth];
    }

    for (LONG i = 0; i <= bh.biHeight - 1; i++) {
        for (LONG j = 0; j <= bh.biWidth - 1; j++) {
            photo[i][j] = 1;
        }
        
    }


    int black = 0;
    int white = 1;

    int buff{ 0 };

    

    LONG x{ 0 }, y{ bh.biHeight - 1 };
    int jump = 54;

    //Положение пиксельных данных от начла структуры 54 байта
    fseek(screen, 54, SEEK_SET);
    while (!feof(screen)) {
        fseek(screen, jump, SEEK_SET);
        buff = fgetc(screen);
        if (buff == 255 && x < (bh.biWidth) && y > -1) {
            photo[y][x] = white;
            
            x++;
            jump += 3;

        }
        else if (buff == 0 && x < (bh.biWidth) && y > -1) {
            photo[y][x] = black;
           
            x++;
            jump += 3;

        }
        else if (x == bh.biWidth && y > -1) {
            y--;
            x = 0;
            // Если строка заканчивается на байтах не кратных 4
            if (((jump - 54) + 1 % 4) == 0) {
                jump += 1;
            }
            else if ((((jump - 54) + 2) % 4) == 0) {
                jump += 2;
            }
            else if ((((jump - 54) + 3) % 4) == 0) {
                jump += 3;
            }

           
        }
    }

    std::cout << "\n";


    for (LONG i = 0; i < bh.biHeight; i++) {
        for (LONG j = 0; j < bh.biWidth; j++) {
            std::cout << photo[i][j];
        }
        std::cout << "\n";
    }

    fclose(screen);

    for (int i = 0; i < bh.biHeight; ++i) {
        delete[] photo[i];
        delete[] photo;
    }
       
}

int main()
{
   
    const char* filepath= "D:\infinity.bmp" ;
   
    getBMP(filepath);
}

