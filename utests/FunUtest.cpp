#include "gtest/gtest.h"
#include "src/Line.hpp"
#include "src/fun.hpp"
#include <QList>
#include <stdexcept>

TEST(Fun, EmptyListInConstructor)
{
    QList<Line> list = {};
    EXPECT_THROW(Fun f(list), std::runtime_error);
}

TEST(Fun, NotEnoughtLinesInConstructor)
{
    QList<Line> list = {{"f", " asd ", 1},
                       {"f", " asd ", 2}};
    EXPECT_THROW(Fun f(list), std::runtime_error);
}

TEST(Fun, NoMarkerInData)
{
    QList<Line> list = {{"f", " asd ", 1},
                        {"f", " asd: ", 2},
                        {"f", " asd ", 3}};
    EXPECT_THROW(Fun f(list), std::runtime_error);
}

TEST(Fun, NoLabelInData)
{
    QList<Line> list = {{"f", " ;##fun(std) ", 1},
                        {"f", " asd ", 2},
                        {"f", " asd ", 3}};
    EXPECT_THROW(Fun f(list), std::runtime_error);
}

TEST(Fun, NoConvention)
{
    QList<Line> list = {{"f", " ;##fun() ", 1},
                        {"f", " asd ", 2},
                        {"f", " asd ", 3}};
    EXPECT_THROW(Fun f(list), std::runtime_error);
}

TEST(Fun, GoodArgumentsInConstructor)
{
    QList<Line> list = {{"f", " ;##fun(std) ", 1},
                        {"f", " asd: ", 2},
                        {"f", " asd ", 3}};
    EXPECT_NO_THROW(Fun f(list));
}
