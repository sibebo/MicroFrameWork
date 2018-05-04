#ifndef APPLICATION_BASE_H
#define APPLICATION_BASE_H

#include <string>
#include "option_parser.h"
//-----------------------------------------------------------------------------
/**
USAGE:

#include "application_base.h"

class MyApp : public ApplicationBase
{
    // These are the members filled by the OptionParser according to the user input.
    // Initialize your options to ensure default values:
    bool        b{false};
    int         number{42};     // Whole numbers must be int.
    float       speed{42.0};    // floating points must be float.
    std::string name{"nobody"}; // A string is a std::string.

    /// This is an override of the abstract base method.
    /// Add your options here. Note, help option is added automatically.
    virtual void    Setup(OptionParser &options) override
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
    virtual bool    Validate(OptionParser &options) override
    {
        return !Positionals().empty();
    }

public:
    /// The constructor.
    MyApp(std::string app_description, std::string app_version)
        : ApplicationBase(app_description, app_version)
    {}

    /// Alternative constructor with the settings specified directly in the initialization of the base class.
    MyApp()
        : ApplicationBase("My terrific app!", "1.2.3")
    {}

    /// This method overrides the abstract base. It basically runs the application.
    /// In this example, simply writing the user specified options and arguments to the console.
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
    MyApp   app;
    if (app.Init(argc, argv))
    {
        return  app.Run();
    }

    return -1;
}
*/



//-----------------------------------------------------------------------------
/**
 * @brief The ApplicationBase class is the application base. Inherit it and fill in your options and mechanics to make a nice command line application.
 */
class ApplicationBase
{
    bool            is_ok{false};   ///< state of the application initialization.
    OptionParser    options;        ///< options.

protected:
    /**
     * @brief HasPositionals returns true if user specified positional arguments.
     * @return  true if any positional arguments were specified. Otherwise false.
     */
    bool    HasPositionals() const
    {
        return options.HasPositionals();
    }

    /**
     * @brief Positionals returns a const reference to the vector of positional arguments (if any).
     * @return      a const reference to a vector of positional arguments (if any).
     */
    const std::vector<std::string>&    Positionals() const
    {
        return options.Positionals();
    }

    /**
     * @brief Setup does the setup. I.e. adding option specifications to the OptionParser. Method is abstract, so override it and annd your own options.
     * @param options   OptionParser to be filled with options.
     */
    virtual void    Setup(OptionParser &options) = 0;

    /**
     * @brief Validate validates the options. As default, it always returns true. So, override it to do your own validation.
     * @param options
     * @return          true if user input (as caught by the OptionParser) is ok. Otherwise false.
     */
    virtual bool    Validate(OptionParser &/*options*/)
    {
        return true;
    }

public:
    /**
     * @brief ApplicationBase, the contructor.
     * @param app_description   an application description to be displayed when showing help info.
     * @param app_version       some version info string of your choise.
     */
    ApplicationBase(std::string app_description, std::string app_version)
        : options(app_description, app_version)
    {}

    /**
     * @brief Init is the basic initialization of the application object. Place your own initialization of your overrided Setup method.
     * @param argc      argument count from main.
     * @param argv      argument values from main.
     * @return          true if everything is fine. Otherwise false.
     */
    virtual bool     Init(int argc, char **argv)
    {
        Setup(options);

        if (options.Parse(argc, argv))
        {
            is_ok = Validate(options);
            if (!is_ok)
            {
                options.Help();
            }
        }
        else
        {
            is_ok = false;
        }

        return is_ok;
    }

    /**
     * @brief IsOk
     * @return      true if everything is OK. Otherwise false.
     */
    bool    IsOk() const {return is_ok;}

    /**
     * @brief Run is where things happen. After calling Init, call this method to start the thing. It is abstract, so override it and fill in your own stuff.
     * @return      value to be returned by main method.
     */
    virtual int     Run() = 0;
};

//-----------------------------------------------------------------------------
#endif  // APPLICATION_BASE_H
