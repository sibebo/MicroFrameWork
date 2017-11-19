#include <iostream>

#include "option_parser.h"
//#include <sstream>
//#include <string>
//#include <vector>

using namespace std;



void    Test(int argc, char **argv)
{
    bool        b{false};
    int         i{0};
    float       f{0.0};
    std::string s;
    bool        x{false};

    std::vector<Option>  options
    {
        {'b', "bool", "a bolean", b},
        {'i', "int", "an integer", i},
        {'f', "float", "a floating point", f},
        {'x', "xxx", "a missing argument", x, true},
        {'s', "string", "a string", s}
    };

    try
    {
        for (Option &option : options)
        {
            if (!option.Parse(argc, argv) && option.IsRequired())
            {
                cout << "missing argument: " << option.LongName() << " !!!" << endl;
            }
        }
    }
    catch(std::exception &e)
    {
        cout << e.what() << endl;
    }
}


void    TestArgParser(int argc, char **argv)
{
    bool        b{false};
    int         i{0};
    float       f{0.0};
    std::string s;
    bool        x{false};
    bool        alt{false};
    int         coping{false};
    float       doing{false};

    OptionParser   args("just a simple test", "0.0.0",
    {
                              {'b', "bool", "a bolean", b},
                              {'i', "int", "an integer", i},
                              {'f', "float", "a floating point", f},
                              {'s', "string", "a string", s},
                              {'a', "alt", "a bolean", alt},
                              {'c', "coping", "an int", coping},
                              {'d', "doing", "a float", doing},
                          });

//    args.Add({'x', "xxx", "a missing argument", x, true});

    //args.Parse(argc, argv);
    if (args.ParseOneAtATime(argc, argv))
    {
        cout << "Done!" << endl;
    }
    else
    {
        cout << "help printed" << endl;
    }

}

int main(int argc, char **argv)
{
    TestArgParser(argc, argv);

    cout << "Hello World!" << endl;
    return 0;
}
