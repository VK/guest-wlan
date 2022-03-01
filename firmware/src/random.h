#ifndef __RANDOM_H
#define __RANDOM_H
#include <Arduino.h>

String random_letters = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char random_Char()
{
    return random_letters[random(0, random_letters.length() - 1)];
}

String random_String(int len)
{
    String output = "";
    for (int i = 0; i < len; i++)
    {
        output += random_Char();
    }

    return output;
}

#endif