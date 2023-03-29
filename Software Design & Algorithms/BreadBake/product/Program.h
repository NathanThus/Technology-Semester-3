#ifndef PROGRAM_H
#define PROGRAM_H

struct Program
{
    int waiting;
    int kneading;
    int rising;
    int baking;
    bool addYeast;
    bool addExtras;
};

//The programs

Program PlainBread = { 60, 20, 160, 50, true, false };
Program BreadPlus = { 60, 20, 160, 50, true, true };
Program Rapid = { 0, 15, 60, 40, true, false};
Program Dough = { 40, 20, 80, 0, true, false};
Program Bake = {0,0,0, 69, false, false};


//Get the program based on the ID. See the PDF for the ID's
Program GetProgram(int id)
{
    switch(id)
    {
        case 1:
            return PlainBread;
        case 2:
            return BreadPlus;
        case 3:
            return Rapid;
        case 4:
            return Dough;
        case 5:
            return Bake;
    }
    return PlainBread;
}

#endif
