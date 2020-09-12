// This file was generated by mys. DO NOT EDIT!!!

#include "mys.hpp"
#include "std/thread.hpp"
#include "std/timer.hpp"
#include "std/random.hpp"
#include "message_passing/student.mys.types.hpp"
#include "message_passing/calculator.mys.types.hpp"

namespace message_passing::student
{

class Timeout : public Object {

public:
    Timeout();

    virtual ~Timeout();
};

class CalcTimer : public mys::timer::Timer {

public:
    std::shared_ptr<Student> student;

    CalcTimer(std::shared_ptr<Student> student);

    virtual ~CalcTimer();
    
    virtual void on_timeout();
};
    
class Student : public mys::thread::Thread {

public:
    std::optional<std::shared_ptr<message_passing::calculator::Calculator>> calculator;
    std::shared_ptr<CalcTimer> timer;

    Student();

    virtual ~Student();

    virtual void start_timer();

    virtual void start();

    virtual void send_timeout(std::shared_ptr<Timeout> message);

    virtual void handle_timeout(std::shared_ptr<Timeout> message);

    virtual void send_result(std::shared_ptr<message_passing::calculator::Result> message);

    virtual void handle_result(std::shared_ptr<message_passing::calculator::Result> message);
};

}