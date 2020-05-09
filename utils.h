#ifndef UTILS
#define UTILS

#include <fstream>
#include <time.h>
#include <iostream>

using namespace std;

void rand_plain_text()
{
    ofstream out_file("plain_text.txt");
    srand((unsigned)time(NULL));
    for (int i = 0; i < 256 * 64; i++)
    {
        char temp = (rand() % 26) + 65;
        out_file << temp;
    }
    out_file.close();
    return;
}

#endif