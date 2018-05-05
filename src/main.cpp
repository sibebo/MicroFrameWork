#include <iostream>

#include "option_parser.h"
#include "application_base.h"
//#include <sstream>
//#include <string>
//#include <vector>

using namespace std;



void    TestOptionParser(int argc, char **argv)
{
    bool        b{false};
    int         i{0};
    float       f{0.0};
    std::string s;
    bool        x{false};
    bool        alt{false};
    int         coping{false};
    float       doing{false};

    microframework::OptionParser   parser("just a simple test", "0.0.0",
    {
                              {'b', "bool", "a bolean", b},
                              {'i', "int", "an integer", i},
                              {'f', "float", "a floating point", f},
                              {'s', "string", "a string", s},
                              {'a', "alt", "a bolean", alt},
                              {'c', "coping", "an int", coping},
                              {'d', "doing", "a float", doing}
                          }
                          );

//    args.Add({'x', "xxx", "a missing argument", x, true});

    //args.Parse(argc, argv);
    if (parser.Parse(argc, argv))
    {
        cout << "Done!" << endl;
    }
    else
    {
        cout << "help printed" << endl;
    }
}

class MyApp : public microframework::ApplicationBase
{
    bool    b{false};
    int     number{42};
    bool    extreme{false};
    float   speed{42.0};
    std::string name{"klaverkalorius"};

    virtual void    Setup(microframework::OptionParser &options) override
    {
        options.Add({
                        {'b', "bool", "just a boolean", b, true},
                        {'i', "int", "just an integer", number, false},
                        {'x', "extreme", "Make everything extreme", extreme, false},
                        {'s', "speed", "The magic speed", speed, false},
                        {'n', "name", "Who are you?", name, false}
                    });
    }

public:
    MyApp(std::string app_description, std::string app_version)
        : ApplicationBase(app_description, app_version)
    {}

    virtual int     Run() override
    {
        std::cout << b << std::endl;
        std::cout << number << std::endl;
        std::cout << extreme << std::endl;
        std::cout << speed << std::endl;
        std::cout << name << std::endl;

        for (const auto &p : Positionals())
        {
            std::cout << "  " << p << std::endl;
        }

        return 0;
    }
};


class TestApp : public microframework::ApplicationBase
{
    // These are the members filled by the OptionParser according to the user input.
    // Initialize your options to ensure default values:
    bool        b{false};
    int         number{42};
    float       speed{42.0};
    std::string name{"nobody"};

    /// This is an override of the abstract base method.
    /// Add your options here. Note, help option is added automatically.
    virtual void    Setup(microframework::OptionParser &options) override
    {
        options.Add({
                        {'b', "bool", "just a boolean", b, true},
                        {'i', "int", "just an integer kjhriherihtierhieriyerityierytiweytiweyitwyitywitywiryirtyieerytiytriwytiwyituwyirtweitywitywieytiwyerituwyeuitywiueytiwueytirueywuie", number, false},
                        {'s', "speed", "The magic speed", speed, false},
                        {'n', "name", "Who are you?", name, false}
                    });
    }

    /// Validation method. Override the base method to specify your own validation.
    /// In this example, positional arguments are required:
    virtual bool    Validate(microframework::OptionParser &/*options*/) override
    {
        return !Positionals().empty();
    }

public:
    /// The constructor.
    TestApp(std::string app_description, std::string app_version)
        : ApplicationBase(app_description, app_version)
    {}

    /// Alternative constructor with the settings specified directly in the initialization of the base class.
    TestApp()
        : ApplicationBase("My terrific app!", "1.2.3")
    {}

    /// This method overrides the abstract base. It basically runs the application.
    virtual int     Run() override
    {
        std::cout << b << std::endl;
        std::cout << number << std::endl;
        std::cout << speed << std::endl;
        std::cout << name << std::endl;

        for (const auto &p : Positionals())
        {
            std::cout << "  " << p << std::endl;
        }

        return 0;
    }
};


int main(int argc, char **argv)
{
//    TestOptionParser(argc, argv);

    TestApp app;
    if (app.Init(argc, argv))
    {
        return  app.Run();
    }

    return -1;

//    MyApp   my_app("this is my app!", "0.0.0");
//    if (my_app.Init(argc, argv))
//    {
//        my_app.Run();
//    }

//    cout << "Hello World!" << endl;
//    return 0;
}
