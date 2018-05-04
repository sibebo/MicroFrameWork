# MicroFrameWork
A simple micro framework for CLI applications


## Example:
``` cpp
#include "application_base.h"

class MyApp : public microframework::ApplicationBase
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
                        {'i', "int", "just an integer", number, false},
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
    MyApp()
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
    MyApp app;
    if (app.Init(argc, argv))
    {
        return  app.Run();
    }

    return -1;
}

```
