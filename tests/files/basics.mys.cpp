// This file was generated by mys. DO NOT EDIT!!!

#include "basics.mys.hpp"

/* mys-embedded-c++-before-namespace start */

#include <before>

/* mys-embedded-c++-before-namespace stop */

namespace mys::basics

{

Tuple<i32, String> func_1(i32 a);

i32 func_2(i32 a, i32 b);

std::shared_ptr<Dict<i32, std::shared_ptr<List<f32>>>> func_3(i32 a);

void func_4(void);

std::shared_ptr<List<i64>> func_5(void);

int main(int __argc, const char *__argv[]);

;

/* mys-embedded-c++ start */

#include <not-before>

/* mys-embedded-c++ stop */;

Tuple<i32, String> func_1(i32 a)
{
    return Tuple<todo>({(2 * a), "Bar"});
}

i32 func_2(i32 a, i32 b)
{
    for (auto i: range(b)) {
        a += (i * b);
    }
    return a;
}

std::shared_ptr<Dict<i32, std::shared_ptr<List<f32>>>> func_3(i32 a)
{
    return std::make_shared<Dict<todo>>({});
}

void func_4(void)
{
    try {
        throw GeneralError();
    } catch (std::exception& e) {
        std::cout << "func_4():      An exception occurred." << std::endl;
    }
}

std::shared_ptr<List<i64>> func_5(void)
{
    auto small = std::make_shared<List<i64>>(std::initializer_list<i64>{});
    for (auto v: List<todo>({3, 1, 5, 7, 2})) {
        if ((v < 5)) {
            small->append(v);
        }
    }
    small->sort();
    small->reverse();
    return small;
}

class Calc : public Object {

public:

    i32 value;

    void triple(void)
    {
        this->value *= 3;
    }

    Calc(i32 value)
    {
        this->value = value;
    }

    String __str__() const
    {
        std::stringstream ss;
        ss << "Calc(";
        ss << "value=" << this->value;
        ss << ")";
        return String(ss.str().c_str());
    }

};

int main(int __argc, const char *__argv[])
{
    auto argv = create_args(__argc, __argv);
    i32 value = i32(argv->get(1));
    std::cout << "func_1(value):" << " " << func_1(value) << std::endl;
    std::cout << "func_2(value):" << " " << func_2(value) << std::endl;
    std::cout << "func_3(value):" << " " << func_3(value) << std::endl;
    func_4();
    std::cout << "func_5():     " << " " << func_5() << std::endl;
    auto calc = std::make_shared<Calc>(value);
    calc->triple();
    std::cout << "calc:         " << " " << calc << std::endl;

    return 0;
}

}



int package_main(int argc, const char *argv[])
{
    return mys::basics::main(argc, argv);
}
