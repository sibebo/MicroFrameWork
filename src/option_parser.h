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
    char        sn{' '};
    std::string ln;
    std::string h;
    OPTION_TYPE option_type{BOOL};

    bool        *b{nullptr};
    int         *i{nullptr};
    float       *f{nullptr};
    std::string *s{nullptr};

    bool        is_required{false};
    bool        was_present{false};
    bool        requires_value{false};

//    std::string GetRawArguments(const std::string &current)
//    /// Returns the argument part, i.e. the one between '-' and optional '='.
//    /// @param the entire argument, e.g. "-abc=42" or "--option=42".
//    /// @return     the argument part, e.g. "abc" or "option".
//    {
//        auto    pos = current.find_first_not_of('-');
//        auto    pos_value_separator = current.find_first_of('=');

//        if (pos_value_separator != std::string::npos)
//        {
//            auto temp = current.substr(pos, pos_value_separator - pos);
//            return temp;
//        }
//        else
//        {
//            return  current.substr(pos);
//        }
//    }

//    bool    ArgumentIsFront(const std::string &arguments)
//    /// Returns true if the short name of the option matches the first char
//    /// in the argument. Otherwise false.
//    /// @param  arguments   a string containing the argument block, i.e. a single or a sequence of short options, e.g. "-abc".
//    /// @return             true if short name matches first char in arguments. Otherwise, false.
//    {
//        return (arguments.find(sn) == 0);
//    }

//    bool    ArgumentIsMatching(const std::string &current)
//    /// Returns true if the long name of the option matches the current argument.
//    /// @param  current     current argument.
//    /// @return             true if current matches the options long name. Otherwise, false.
//    {
//        return (GetRawArguments(current).compare(ln) == 0);
//    }

//    bool    ArgumentIsLast(const std::string &arguments)
//    /// Returns true if the short name of the option is the last char in the argument block.
//    /// @param  arguments   a string containing the argument block, i.e. a single or a sequence of short options, e.g. "-abc".
//    /// @return             true if short name matches last char in arguments. Otherwise, false.
//    {
//        return (arguments.find(sn) == arguments.length() - 1);
//    }

//    bool    HasEmbeddedValue(const std::string &current)
//    {
//        return  !current.empty();
//        //return  (current.find_first_of('=') != std::string::npos);
//    }

//    std::string GetEmbeddedValue(const std::string &current)
//    {
//        if (current.front() == '=')
//        {
//            return current.substr(1);
//        }
//        else
//        {
//            return current;
//        }
//    }

//    std::string GetArgumentValue(const std::string &current, std::vector<std::string> &args)
//    {
//        std::string value;
//        if (HasEmbeddedValue(current))
//        {
//            value = GetEmbeddedValue(current);
//            args.erase(args.begin()); // Remove recognized option from list.
//        }
//        else if (!args.empty())
//        {
//            value = args.front();
//            args.erase(args.begin()); // Remove recognized option from list.
//        }
//        else
//        {
//            throw std::runtime_error(std::string() + "missing value for last argument: --" + ln);
//        }

//        return value;
//    }

//    bool    AcceptBool()
//    /// If option is registered as a bool, the value is set to true and true is returned. Otherwise, false is returned.
//    /// @return     true if the option is registered as a bool. Otherwise, false.
//    {
//        if (b != nullptr)
//        {
//            *b = true;
//            was_present = true;

//            return true;
//        }

//        return false;
//    }

//    bool    AcceptValue(const std::string &current, std::vector<std::string> &args, bool is_long)
//    ///
//    {
//        std::string value = GetArgumentValue(current, args);

//        if (value.empty())
//        {
//            if (is_long)
//            {
//                throw std::runtime_error(std::string() + "Could not parse value for argument --" + ln);
//            }
//            else
//            {
//                throw std::runtime_error(std::string() + "Could not parse value for argument -" + sn);
//            }
//        }

//        if (i != nullptr)
//        {
//            *i = std::stoi(value);
//            was_present = true;
//        }
//        else if (f != nullptr)
//        {
//            *f = std::stof(value);
//            was_present = true;
//        }
//        else if (s != nullptr)
//        {
//            *s = value;
//            was_present = true;
//        }
//        else
//        {
//            if (is_long)
//            {
//                throw std::runtime_error(std::string() + "Could not parse value for argument --" + ln + " : " + value);
//            }
//            else
//            {
//                throw std::runtime_error(std::string() + "Could not parse value for argument -" + sn + " : " + value);
//            }
//        }

//        return was_present;
//    }

//    void    RemoveLongArgumentFromFront(std::vector<std::string> &args)
//    {
//        std::string     &current = args.front();

//        auto    pos = current.find(ln);
//        if (pos != std::string::npos)
//        {
//            current.erase(pos, ln.length());
//        }

//        if (current.compare("-") == 0 || current.empty() || current.find("-=") == 0)
//        {
//            args.erase(args.begin());
//        }
//    }

//    void    RemoveShortArgumentFromFront(std::vector<std::string> &args)
//    {
//        std::string     &current = args.front();

//        auto    pos = current.find(sn);
//        if (pos != std::string::npos)
//        {
//            current.erase(pos, 1);
//        }

//        if (current.compare("-") == 0 || current.empty() || current.find("-=") == 0)
//        {
//            args.erase(args.begin());
//        }
//    }

//    bool    ParseLong(std::vector<std::string> &args)
//    {
//        std::string     current = args.front();

//        if (ArgumentIsMatching(current))
//        {
//            args.erase(args.begin()); // Remove recognized option from list.

//            if (!AcceptBool())
//            {
//                AcceptValue(current.substr(2 + ln.length()), args, true);
//            }
//        }

//        return was_present;
//    }

//    bool    ParseShort(std::vector<std::string> &args)
//    {
//        std::string     current = args.front();

//        auto    arguments = GetRawArguments(current);

//        if (ArgumentIsFront(arguments))
//        {
//            RemoveShortArgumentFromFront(args);

//            if (!AcceptBool()) // && ArgumentIsLast(arguments))
//            {
//                AcceptValue(current.substr(1 + 1), args, false);
//            }
////            else
////            {
////                throw std::runtime_error(std::string() + "Could not find argument for -" + sn);
////            }
//        }

//        return was_present;
//    }
//    //----------------------------------


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

    bool    Parse(std::vector<std::string> &args)
    {
        std::string     &current = args.front();

        if (current.find_first_not_of('-') == 2)
        {
            ParseLongAlt(args);
        }
        else if (current.find_first_not_of('-') == 1)
        {
            ParseShortAlt(args);
        }

        return was_present;
    }

private:
    bool    ParseShortAlt(std::vector<std::string> &args)
    {
        std::string &current = args.front();

        // Is the option too short to parse?
        if (current.length() <= 1) return false;

        // Does the option match the current short name (with optional value embedded)?
        if (current[1] == sn)       // option is first char after leading '-'.
        {
            current.erase(1, 1);    // Remove option char.
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
                        stream << "Failed parsing value for option '" << sn << "' : '" << current << "'" << std::endl;
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
                        stream << "Failed parsing value for option '" << sn << "' : '" << args.front() << "'" << std::endl;
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
        }

        return was_present;
    }

    bool    ParseLongAlt(std::vector<std::string> &args)
    {
        std::string &current = args.front();

        // Is the option too short to parse?
        if (current.length() <= 2) return false;

        // Does the option match the current long name (with optional value embedded)?
        if (current.find(ln) == 2)      // option is first char after leading "--".
        {
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
                        stream << "Failed parsing value for option '" << ln << "' : '" << current << "'" << std::endl;
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
                        stream << "Failed parsing value for option '" << ln << "' : '" << args.front() << "'" << std::endl;
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
        }

        return was_present;
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

    std::string         app_name;
    std::string         app_description;
    std::string         app_version;

    std::vector<std::string>    positionals;
    std::vector<std::string>    unknown;

    std::map<std::string, Option> options;

    std::vector<std::string>    args;

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

    bool    IsHelp() const {return is_help;}

    /**
     * @brief Parse parses the user specified options as defined by argc and agrv.
     * @param argc      number of arguments. From main.
     * @param argv      argument values. From main.
     * @return          true if everything is fine. Otherwise false.
     * @throws          std::runtime_error in case of a parsing error.
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
                    if (!option.IsPresent())
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
                    throw(std::runtime_error(std::string("Unknown option: ") + args.front()));
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

        for (const auto &m : options)
        {
            const Option &option = m.second;

            if (option.IsRequired() && !option.IsPresent())
            {
                throw (std::runtime_error(std::string("Missing required option: --") + option.LongName()));
            }
        }

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

//----------------------------------------------------------------------------
#endif  // OPTION_PARSER_H
