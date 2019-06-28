#include "gtest/gtest.h"
#include "src/Line.hpp"
#include "src/fun.hpp"
#include <QList>
#include <stdexcept>

TEST(Fun, SimpleTest)
{
    QList<Line> list = {
                { "file", " ;##fun(std) ", 1 },
                { "file", " funkcja: ", 2 },
                { "file", " mov.w #666, R12 ", 3 },
                { "file", " ret ", 4 },
                };
    Fun f(list);
    //<TODO> potrzebne namiastrki FunContainer i Convs
}

//<TODO> masa kolejnych testów do napisania
