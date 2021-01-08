#pragma once

namespace OpenGL_Learn
{
    template <typename T1, typename T2>
    struct Tuple2
    {
    public:
        T1 Item1;
        T2 Item2;

        Tuple2() :Item1(), Item2() {}
        Tuple2(const T1& _1, const T2& _2) :Item1(_1), Item2(_2) {}
        Tuple2(T1&& _1, T2&& _2) :Item1(_1), Item2(_2) {}
    };

    template <typename T1, typename T2, typename T3>
    struct Tuple3
    {
    public:
        T1 Item1;
        T2 Item2;
        T3 Item3;

        Tuple3() :Item1(), Item2(), Item3() {}
        Tuple3(const T1& _1, const T2& _2, const T3& _3) :Item1(_1), Item2(_2), Item3(_3) {}
        Tuple3(T1&& _1, T2&& _2, T3&& _3) :Item1(_1), Item2(_2), Item3(_3) {}
    };

    template <typename T1, typename T2, typename T3, typename T4>
    struct Tuple4
    {
    public:
        T1 Item1;
        T2 Item2;
        T3 Item3;
        T4 Item4;

        Tuple4() :Item1(), Item2(), Item3(), Item4() {}
        Tuple4(const T1& _1, const T2& _2, const T3& _3, const T4& _4) :Item1(_1), Item2(_2), Item3(_3), Item4(_4) {}
        Tuple4(T1&& _1, T2&& _2, T3&& _3, const T4& _4) :Item1(_1), Item2(_2), Item3(_3), Item4(_4) {}
    };
}