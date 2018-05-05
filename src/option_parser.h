#ifndef OPTION_PARSER_H
#define OPTION_PARSER_H
//----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

//----------------------------------------------------------------------------
namespace microframework
{
class   Option
{
public:
    enum    OPTION_TYPE
    {
        BOOL,
        INT,
        FLOAT,
        STRING
    };

private:
    char        sn{' '};            ///< option short name.
    std::string ln;                 ///< option long name.
    std::string h;                  ///< option help text.
    OPTION_TYPE option_type{BOOL};  ///< option type.

    bool        *b{nullptr};        ///< pointer to value of type bool, OPTION_TYPE::BOOL.
    int         *i{nullptr};        ///< pointer to value of type int, OPTION_TYPE::INT.
    float       *f{nullptr};        ///< pointer to value of type float, OPTION_TYPE::FLOAT.
    std::string *s{nullptr};        ///< pointer to value of type string, OPTION_TYPE::STRING.

    bool        is_required{false};     ///< true if option is required, mandatory.
    bool        was_present{false};     ///< true if option was found.
    bool        requires_value{false};  ///< true if option is followed by a value (OPTION_TYPE::INT, OPTION_TYPE::FLOAT, OPTION_TYPE::STRING).

public:
    Option() {}
    Option(char short_name, std::string long_name, std::string help, bool &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), option_type(BOOL), b(&value), is_required(is_required)
    {}
    Option(char short_name, std::string long_name, std::string help, int &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), option_type(INT), i(&value), is_required(is_required), requires_value(true)
    {}
    Option(char short_name, std::string long_name, std::string help, float &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), option_type(FLOAT), f(&value), is_required(is_required), requires_value(true)
    {}
    Option(char short_name, std::string long_name, std::string help, std::string &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), option_type(STRING), s(&value), is_required(is_required), requires_value(true)
    {}

    bool        IsRequired() const {return is_required;}
    bool        IsPresent() const {return was_present;}
    std::string LongName() const {return ln;}
    char        ShortName() const {return sn;}
    OPTION_TYPE OptionType() const {return option_type;}
    std::string HelpText() const {return h;}

    std::string ValueAsString() const
    /// Returns the referenced value as string.
    /// @return     string representing the referenced value.
    {
        switch (OptionType())
        {
        case Option::OPTION_TYPE::BOOL:
            return *b ? "true" : "false";
        break;
        case Option::OPTION_TYPE::INT:
            return std::to_string(*i);
        break;
        case Option::OPTION_TYPE::FLOAT:
            return std::to_string(*f);
        break;
        case Option::OPTION_TYPE::STRING:
            return *s;
        break;
        }

        return std::string();
    }

    /**
     * @brief Parse checks if the first argument in the list matched this option. If it does, it is parsed and true is returned. Otherwise false is returned.
     * @param args  Vector of options left to be parsed.
     * @return      True if the first argument matches this option. Otherwise false.
     */
    bool    Parse(std::vector<std::string> &args)
    {
        std::string     &current = args.front();

        if (current.find_first_not_of('-') == 2)
        {
            return ParseLong(args);
        }
        else if (current.find_first_not_of('-') == 1)
        {
            return ParseShort(args);
        }

        return false;
    }

private:
    /**
     * @brief ParseShort parses a long option, i.e. an option prefixed by '-'.
     * @param args      vector of arguements left to be parsed.
     * @return          true if option matches this option. Otherwise false.
     * @throws          std::runtime_error in case of a missing value for the option.
     */
    bool    ParseShort(std::vector<std::string> &args)
    {
        std::string &current = args.front();

        // Is the option too short to parse?
        if (current.length() <= 1) return false;

        // Does the option match the current short name (with optional value embedded)?
        if (current[1] != sn)   return false;

        // option is first char after leading '-'.
        current.erase(1, 1);    // Remove option char.

        if (was_present)
        {
            std::stringstream stream;
            stream << "Option already present: '-" << sn << "'" << std::endl;
            throw(std::runtime_error(stream.str()));
        }

        was_present = true;

        if (requires_value)
        {
            current.erase(0, 1);    // Remove the leading '-'.
            if (!current.empty())
            {
                if (current.front() == '=' || current.front() == ':')
                {
                    current.erase(0, 1);    // remove the separator, '=' or ':'.
                }

                try
                {
                    SetValue(current);
                }
                catch(...)
                {
                    std::stringstream stream;
                    stream << "Failed parsing value for option '-" << sn << "' : '" << current << "'" << std::endl;
                    throw(std::runtime_error(stream.str()));
                }

                args.erase(args.begin());   // remove option and embedded value.
            }
            else
            {
                args.erase(args.begin());   // remove option.

                try
                {
                    SetValue(args.front());
                }
                catch(...)
                {
                    std::stringstream stream;
                    stream << "Failed parsing value for option '-" << sn << "' : '" << args.front() << "'" << std::endl;
                    throw(std::runtime_error(stream.str()));
                }
                args.erase(args.begin());   // remove value.
            }
        }
        else
        {
            SetBool();
            if (current.length() == 1)
            {
                args.erase(args.begin());   // remove option artifact, '-'.
            }
        }

        return true;
    }

    /**
     * @brief ParseLong parses a long option, i.e. an option prefixed by "--".
     * @param args      vector of arguements left to be parsed.
     * @return          true if option matches this option. Otherwise false.
     * @throws          std::runtime_error in case of a missing value for the option.
     */
    bool    ParseLong(std::vector<std::string> &args)
    {
        std::string &current = args.front();

        // Is the option too short to parse?
        if (current.length() <= 2) return false;

        // Does the option match the current long name (with optional value embedded)?
        if (current.find(ln) != 2)  return false;

        // option is first char after leading "--".
        if (was_present)
        {
            std::stringstream stream;
            stream << "Option already present: '--" << ln << "'" << std::endl;
            throw(std::runtime_error(stream.str()));
        }

        was_present = true;

        if (requires_value)
        {
            current.erase(0, 2 + ln.length());    // Remove the leading "--" and option long name.
            if (!current.empty())
            {
                if (current.front() == '=' || current.front() == ':')
                {
                    current.erase(0, 1);    // remove the separator, '=' or ':'.
                }

                try
                {
                    SetValue(current);
                }
                catch(...)
                {
                    std::stringstream stream;
                    stream << "Failed parsing value for option '--" << ln << "' : '" << current << "'" << std::endl;
                    throw(std::runtime_error(stream.str()));
                }
                args.erase(args.begin());   // remove option and embedded value.
            }
            else
            {
                args.erase(args.begin());   // remove option.

                try
                {
                    SetValue(args.front());
                }
                catch(...)
                {
                    std::stringstream stream;
                    stream << "Failed parsing value for option '--" << ln << "' : '" << args.front() << "'" << std::endl;
                    throw(std::runtime_error(stream.str()));
                }
                args.erase(args.begin());   // remove value.
            }
        }
        else
        {
            SetBool();
            args.erase(args.begin());   // remove option.
        }

        return true;
    }

    void    SetBool()
    {
        if (b != nullptr)
        {
            *b = true;
            was_present = true;
        }
    }

    void    SetValue(std::string value)
    {
        if (i != nullptr)
        {
            *i = std::stoi(value);
            was_present = true;
        }
        else if (f != nullptr)
        {
            *f = std::stof(value);
            was_present = true;
        }
        else if (s != nullptr)
        {
            *s = value;
            was_present = true;
        }
    }
};


//----------------------------------------------------------------------------
class   OptionParser
{
    bool                is_help{false};

    std::string         app_name;               ///< application name extracted from the first argument, i.e. argv[0].
    std::string         app_description;        ///< user specified via constructor.
    std::string         app_version;            ///< user specified via constructor.

    std::vector<std::string>    positionals;    /// Vector of positional arguments (i.e. those without leading '-' og "--").
    //std::vector<std::string>    unknown;        /// Unknown options.

    std::map<std::string, Option> options;      ///< user specified via constructor or Add. @see Add.

    std::vector<std::string>    args;           ///< the raw arguments, derrived from argc and argv.

    /**
     * @brief PrintHelp prints the help text to console.
     */
    void    PrintHelp()
    {
        std::string name = app_name;

        auto    pos = app_name.find_last_of(":/\\");
        if (pos != std::string::npos)
        {
            app_name = app_name.substr(pos + 1);
        }

        std::cout << app_name << ", version: " << app_version << std::endl;
        std::cout << std::string(79, '-') << std::endl;
        std::cout << app_description << std::endl;
        std::cout << std::endl;

        std::size_t     max_length = 0;
        for (auto &m : options)
        {
            Option  &option = m.second;
            max_length = std::max(max_length, option.LongName().length());
        }

        for (auto &m : options)
        {
            Option  &option = m.second;

            std::stringstream   stream;

            stream << "--" << std::setw(max_length) << std::left << option.LongName()
                   << " -" << std::setw(1) << std::left << option.ShortName();

            switch (option.OptionType())
            {
            case Option::OPTION_TYPE::BOOL:
                stream << std::setw(10) << " ";
            break;
            case Option::OPTION_TYPE::INT:
                stream << std::setw(10) << " <integer>";
            break;
            case Option::OPTION_TYPE::FLOAT:
                stream << std::setw(10) << " <float>";
            break;
            case Option::OPTION_TYPE::STRING:
                stream << std::setw(10) << " <string>";
            break;
            }

            if (option.HelpText().length() < 80 - (max_length + 17))
            {
                stream << "  " << option.HelpText() << std::endl;
            }
            else
            {
                stream << std::endl;
                stream << std::string(max_length + 17, ' ') << option.HelpText() << std::endl;
            }

            if (option.IsRequired())
            {
                stream << std::string(max_length + 17, ' ') << "Required."
                       << std::endl;
            }

            if (!option.IsRequired() && option.OptionType() != Option::OPTION_TYPE::BOOL)
            {
                stream << std::string(max_length + 17, ' ') << "Default: "
                       << option.ValueAsString()
                       << std::endl;
            }

            std::cout << stream.str();
        }

        std::cout << std::string(79, '-') << std::endl;
    }

    /**
     * @brief GetArgs converts the args to a vector of arguments.
     * @param argc  argument count. From main.
     * @param argv  argument values. From main.
     * @return      a std::vector of arguments.
     */
    std::vector<std::string>    GetArgs(int argc, char **argv)
    {
        std::vector<std::string>   args(argv, argv + argc);

        return args;
    }

    /**
     * @brief IsOption returns true if input is an option, i.e. starting with one or two '-'.
     * @param s     token to test.
     * @return      returns true if s is an option. I.e. the first char of s is '-'.
     */
    bool    IsOption(const std::string &s)
    {
        return (s.find('-') == 0);
    }

    /**
     * @brief Prepare takes care of initial initialization including the addition of the help option (-h and --help)
     * @param argc  argument count. From main.
     * @param argv  argument values. From main.
     * @return      returns true if everything is fine. If no arguments are present help is displayed and false is returned.
     */
    bool    Prepare(int argc, char **argv)
    {
        Add({'h', "help", "Printing this help", is_help});

        args = GetArgs(argc, argv);
        app_name = args.front();
        args.erase(args.begin());

        if (args.empty())
        {
            PrintHelp();
            return false;
        }

        return true;
    }

    /**
     * @brief AssertRequiredOptionsArePresent  Asserts that required options are present. If not, astd::runtime_error exception is thrown.
     * @throws          std::runtime_error in case of a missing required option.
     */
    void    AssertRequiredOptionsArePresent()
    {
        for (const auto &m : options)
        {
            const Option &option = m.second;

            if (option.IsRequired() && !option.IsPresent())
            {
                throw (std::runtime_error(std::string("Missing required option: --") + option.LongName()));
            }
        }
    }

public:
    OptionParser(std::string app_description, std::string app_version)
        : app_description(app_description), app_version(app_version)
    {}

    OptionParser(std::string app_description, std::string app_version, const Option &option)
        : app_description(app_description), app_version(app_version)
    {Add(option);}

    OptionParser(std::string app_description, std::string app_version, const std::vector<Option> &options)
        : app_description(app_description), app_version(app_version) //, options(options)
    {Add(options);}

    /**
     * @brief Add adds a list of options to the option list.
     * @param options   options to add to option list.
     */
    void    Add(const std::vector<Option> &options)
    {
        for (const Option &option : options)
        {
            Add(option);
        }
    }

    /**
     * @brief Add adds a single option to the option list.
     * @param option    option to add to option list.
     */
    void    Add(const Option &option)
    {
        //options["hej"] = option;
        //options[option.LongName()] = option;
        //options.insert(std::make_pair(option.LongName(), option));
        options.emplace(option.LongName(), option);
    }

    /**
     * @brief IsHelp returns true if the help option was present, i.e. -h or --help.
     * @return  true if help option was present.
     */
    bool    IsHelp() const {return is_help;}

    /**
     * @brief Parse parses the user specified options as defined by argc and agrv.
     * @param argc      number of arguments. From main.
     * @param argv      argument values. From main.
     * @return          true if everything is fine. Otherwise false.
     * @throws          std::runtime_error in case of a parsing error.
     * @throws          std::runtime_error in case of a missing required option.
     */
    bool    Parse(int argc, char **argv)
    {
        if (!Prepare(argc, argv)) return false;

        while (!args.empty())
        {
            if (IsOption(args.front()))
            {
                bool    found = false;
                for (auto &m : options)     //Option &option : options)
                {
                    Option  &option = m.second;
                    if (true || !option.IsPresent())
                    {
                        if (option.Parse(args))
                        {
                            found = true;
                            break;
                        }
                    }
                }

                if (!found)
                {
                    std::string     arg = args.front();
                    if (arg.find_first_not_of('-') == 1)
                    {
                        arg = arg.substr(0, 2);
                    }
                    throw(std::runtime_error(std::string("Unknown option: ") + arg));
//                    unknown.push_back(args.front());
//                    args.erase(args.begin());
                }
            }
            else
            {
                positionals.push_back(args.front());
                args.erase(args.begin());
            }
        }

        AssertRequiredOptionsArePresent();  // May throw!

        if (IsHelp())
        {
            PrintHelp();
            return false;
        }

        return true;
    }

    /**
     * @brief Help writes the help text to the console.
     */
    void    Help()
    {
        PrintHelp();
    }

    bool                            HasPositionals() const {return !positionals.empty();}
    const std::vector<std::string>& Positionals() const {return positionals;}
    const Option&                   operator[](const std::string &long_name) {return options[long_name];}
};

}   // namespace microframework
//----------------------------------------------------------------------------
#endif  // OPTION_PARSER_H
