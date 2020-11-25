#include "mys.hpp"

namespace brainfuck::main

{

class Tape;
class Op;

List<std::shared_ptr<Op>>
parse(const std::shared_ptr<StringIO>& source);

void run(const List<std::shared_ptr<Op>>& ops, const std::shared_ptr<Tape>& tape);

int main();

String SOURCE_B(">++[<+++++++++++++>-]<[[>+>+<<-]>[<+>-]++++++++\n[>++++++++<-]>.[-]<<>++++++++++[>++++++++++[>++\n++++++++[>++++++++++[>++++++++++[>++++++++++[>+\n+++++++++[-]<-]<-]<-]<-]<-]<-]<-]++++++++++.");

class Tape : public Object {

public:
    List<i32> tape;
    i32 pos;

    Tape()
    {
        this->tape = List<i32>({0});
        this->pos = 0;
    }

    virtual ~Tape()
    {
    }

    i32 get()
    {
        return this->tape[this->pos];
    }

    void inc(i32 value)
    {
        this->tape[this->pos] += value;
    }

    void move(i32 value)
    {
        this->pos += value;
        while (this->pos >= len(this->tape)) {
            this->tape.append(0);
        }
    }

    String __str__() const
    {
        std::stringstream ss;
        ss << "Tape(";
        ss << "tape=" << this->tape << ", ";
        ss << "pos=" << this->pos;
        ss << ")";
        return String(ss.str().c_str());
    }

};

class Op : public Object {

public:

    virtual ~Op()
    {
    }

    virtual void execute(const std::shared_ptr<Tape>& tape) = 0;

};

class Inc : public Op {

public:
    i32 val;

    Inc(i32 val)
    {
        this->val = val;
    }

    virtual ~Inc()
    {
    }

    void execute(const std::shared_ptr<Tape>& tape)
    {
        tape->inc(this->val);
    }

    String __str__() const
    {
        std::stringstream ss;
        ss << "Inc(";
        ss << "val=" << this->val;
        ss << ")";
        return String(ss.str().c_str());
    }

};

class Move : public Op {

public:
    i32 val;

    Move(i32 val)
    {
        this->val = val;
    }

    virtual ~Move()
    {
    }

    void execute(const std::shared_ptr<Tape>& tape)
    {
        tape->move(this->val);
    }

    String __str__() const
    {
        std::stringstream ss;
        ss << "Move(";
        ss << "val=" << this->val;
        ss << ")";
        return String(ss.str().c_str());
    }

};

class Print : public Op {

public:

    virtual ~Print()
    {
    }

    void execute(const std::shared_ptr<Tape>& tape)
    {
        std::cout << chr(tape->get()) << std::flush;
    }

    String __str__() const
    {
        std::stringstream ss;
        ss << "Print(";
        ss << ")";
        return String(ss.str().c_str());
    }

};

class Loop : public Op {

public:
    List<std::shared_ptr<Op>> ops;

    Loop(const List<std::shared_ptr<Op>>& ops)
    {
        this->ops = ops;
    }

    virtual ~Loop()
    {
    }

    void execute(const std::shared_ptr<Tape>& tape)
    {
        while (tape->get() > 0) {
            run(this->ops, tape);
        }
    }

    String __str__() const
    {
        std::stringstream ss;
        ss << "Loop(";
        ss << "ops=" << this->ops;
        ss << ")";
        return String(ss.str().c_str());
    }

};

List<std::shared_ptr<Op>>
parse(const std::shared_ptr<StringIO>& source)
{
    auto ops = List<std::shared_ptr<Op>>({});
    while (true) {
        String c = source->read(1);
        if (c == "+") {
            ops.append(std::make_shared<Inc>(1));
        } else {
            if (c == "-") {
                ops.append(std::make_shared<Inc>(-1));
            } else {
                if (c == ">") {
                    ops.append(std::make_shared<Move>(1));
                } else {
                    if (c == "<") {
                        ops.append(std::make_shared<Move>(-1));
                    } else {
                        if (c == ".") {
                            ops.append(std::make_shared<Print>());
                        } else {
                            if (c == "[") {
                                auto ops2 = parse(source);
                                ops.append(std::make_shared<Loop>(ops2));
                            } else {
                                if (contains(c, List<String>({"]", ""}))) {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ops;
}

void run(const List<std::shared_ptr<Op>>& ops, const std::shared_ptr<Tape>& tape)
{
    // The program is a lot faster with references to operations.
    for (auto& op: ops) {
        op->execute(tape);
    }
}

int main(int __argc, const char *__argv[])
{
    (void)__argc;
    (void)__argv;

    auto string = std::make_shared<StringIO>(SOURCE_B);
    auto ops = parse(string);
    auto tape = std::make_shared<Tape>();
    run(ops, tape);

    return 0;
}

}

int package_main(int argc, const char *argv[])
{
    return brainfuck::main::main(argc, argv);
}
