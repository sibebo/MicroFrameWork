#ifndef OPTION_PARSER_H
#define OPTION_PARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class   Option
{
    char        sn{' '};
    std::string ln;
    std::string h;

    bool        *b{nullptr};
    int         *i{nullptr};
    float       *f{nullptr};
    std::string *s{nullptr};

    bool        is_required{false};
    bool        was_present{false};

    std::string GetRawArguments(const std::string &current)
    {
        auto    pos = current.find_first_not_of('-');
        auto    pos_value_separator = current.find_first_of('=');

        if (pos_value_separator != std::string::npos)
        {
            auto temp = current.substr(pos, pos_value_separator - pos);
            return temp;
        }
        else
        {
            return  current.substr(pos);
        }
    }

    bool    ArgumentIsFront(const std::string &arguments)
    {
        return (arguments.find(sn) == 0);
    }

    bool    ArgumentIsMatching(const std::string &current)
    {
        return (GetRawArguments(current).compare(ln) == 0);
    }

    bool    ArgumentIsLast(const std::string &arguments)
    {
        return (arguments.find(sn) == arguments.length() - 1);
    }

    bool    HasEmbeddedValue(const std::string &current)
    {
        return  (current.find_first_of('=') != std::string::npos);
    }

    std::string GetEmbeddedValue(const std::string &current)
    {
        auto            pos_value_separator = current.find_first_of('=');
        std::string     arguments = current.substr(0, pos_value_separator);

        if (pos_value_separator != std::string::npos)
        {
            ++pos_value_separator;
        }

        return current.substr(pos_value_separator);
    }

    std::string GetArgumentValue(std::string &current, std::vector<std::string> &args)
    {
        std::string value;
        if (HasEmbeddedValue(current))
        {
            value = GetEmbeddedValue(current);
        }
        else if (!args.empty())
        {
            value = args.front();
            args.erase(args.begin()); // Remove recognized option from list.
        }
        else
        {
            throw std::runtime_error(std::string() + "missing value for last argument: --" + ln);
        }

        return value;
    }

    bool    AcceptBool()
    {
        if (b != nullptr)
        {
            *b = true;
            was_present = true;

            return true;
        }

        return false;
    }

    void    AcceptValue(std::string &current, std::vector<std::string> &args, bool is_long)
    {
        std::string value = GetArgumentValue(current, args);

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
        else
        {
            if (is_long)
            {
                throw std::runtime_error(std::string() + "Could not parse value for argument --" + ln + " : " + value);
            }
            else
            {
                throw std::runtime_error(std::string() + "Could not parse value for argument -" + sn + " : " + value);
            }
        }
    }

    void    RemoveShortArgumentFromFront(std::vector<std::string> &args)
    {
        std::string     &current = args.front();

        auto    pos = current.find(sn);
        if (pos != std::string::npos)
        {
            current.erase(pos, 1);
        }

        if (current.compare("-") == 0 || current.empty() || current.find("-=") == 0)
        {
            args.erase(args.begin());
        }
    }

    bool    ParseLong(std::vector<std::string> &args)
    {
        std::string     current = args.front();

        if (ArgumentIsMatching(current))
        {
            args.erase(args.begin()); // Remove recognized option from list.

            if (!AcceptBool())
            {
                AcceptValue(current, args, true);
            }
        }

        return was_present;
    }

    bool    ParseShort(std::vector<std::string> &args)
    {
        std::string     current = args.front();

        auto    arguments = GetRawArguments(current);

        if (ArgumentIsFront(arguments))
        {
            RemoveShortArgumentFromFront(args);

            if (!AcceptBool() && ArgumentIsLast(arguments))
            {
                AcceptValue(current, args, false);
            }
            else
            {
                throw std::runtime_error(std::string() + "Could not find argument for -" + sn);
            }
        }

        return was_present;
    }
    //----------------------------------


public:
    Option() {}
    Option(char short_name, std::string long_name, std::string help, bool &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), b(&value), is_required(is_required)
    {}
    Option(char short_name, std::string long_name, std::string help, int &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), i(&value), is_required(is_required)
    {}
    Option(char short_name, std::string long_name, std::string help, float &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), f(&value), is_required(is_required)
    {}
    Option(char short_name, std::string long_name, std::string help, std::string &value, bool is_required = false)
        : sn(short_name), ln(long_name), h(help), s(&value), is_required(is_required)
    {}

    bool        IsRequired() const {return is_required;}
    bool        IsPresent() const {return was_present;}
    std::string LongName() const {return ln;}

    bool    Parse(std::vector<std::string> &args)
    {
        std::string     &current = args.front();

        if (current.find_first_not_of('-') == 2)
        {
            ParseLong(args);
        }
        else if (current.find_first_not_of('-') == 1)
        {
            ParseShort(args);
        }

        return was_present;
    }
};

class   OptionParser
{
    bool                is_help{false};

    std::string         app_description;
    std::string         app_version;

    std::vector<std::string>    positionals;
    std::vector<std::string>    unknown;

    std::map<std::string, Option> options;

    void    PrintHelp(std::string app_name)
    {
        std::string name = app_name;

        auto    pos = app_name.find_last_of(":/\\");
        if (pos != std::string::npos)
        {
            app_name = app_name.substr(pos + 1);
        }

        std::cout << app_name << ", version: " << app_version << std::endl;
        std::cout << app_description << std::endl;
        std::cout << std::endl;
    }

    std::vector<std::string>    GetArgs(int argc, char **argv)
    {
        std::vector<std::string>   args(argv, argv + argc);

        return args;
    }

    bool    IsOption(const std::string &s)
    {
        return (s.find('-') == 0);
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

    void    Add(const std::vector<Option> &options)
    {
        for (const Option &option : options)
        {
            Add(option);
        }
    }

    void    Add(const Option &option)
    {
        //options["hej"] = option;
        //options[option.LongName()] = option;
        //options.insert(std::make_pair(option.LongName(), option));
        options.emplace(option.LongName(), option);
    }

    bool    IsHelp() const {return is_help;}

    bool    Parse(int argc, char **argv)
    {
        Add({'h', "help", "Printing this help", is_help});

        auto    args = GetArgs(argc, argv);
        args.erase(args.begin());

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
                    unknown.push_back(args.front());
                    args.erase(args.begin());
                }
            }
            else
            {
                positionals.push_back(args.front());
                args.erase(args.begin());
            }
        }

        if (IsHelp())
        {
            PrintHelp(argv[0]);
            return false;
        }

        return true;
    }

    const std::vector<std::string>& Positionals() const {return positionals;}
    const Option&                   operator[](const std::string &long_name) {return options[long_name];}
};


#endif  // OPTION_PARSER_H
