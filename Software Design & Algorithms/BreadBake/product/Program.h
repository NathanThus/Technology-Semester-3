#ifndef PROGRAM_H
#define PROGRAM_H

struct Program
{
    int resting;
    int kneading;
    int rising;
    int baking;
    bool addYeast;
    bool addExtras;
};

//The programs

const Program PlainBread = { 60, 20, 160, 50, true, false };
const Program BreadPlus = { 60, 20, 160, 50, true, true };
const Program Rapid = { 0, 15, 60, 40, true, false};
const Program Dough = { 40, 20, 80, 0, true, false};
const Program Bake = {0,0,0, 69, false, false};


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
        default:
        return PlainBread;
    }
}

#endif
