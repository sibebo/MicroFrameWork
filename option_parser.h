#ifndef OPTION_PARSER_H
#define OPTION_PARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class   Option
{
    char        sn;
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
            return  current.substr(pos, pos_value_separator - 1);
        }
        else
        {
            return  current.substr(pos);
        }
    }

    bool    ArgumentIsPresent(const std::string &arguments)
    {
        return (arguments.find(sn) != std::string::npos);
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

    void    ParseShort(int argc, char **argv, int &index)
    {
        std::string     current(argv[index++]);

        auto    arguments = GetRawArguments(current);

        if (ArgumentIsPresent(arguments))
        {
            if (b != nullptr)
            {
                *b = true;
                was_present = true;
            }
            else if (ArgumentIsLast(arguments))
            {
                std::string value;
                if (HasEmbeddedValue(current))
                {
                    value = GetEmbeddedValue(current);
                }
                else if (index < argc)
                {
                    value = argv[index++];
                }
                else
                {
                    throw std::runtime_error(std::string() + "missing value for last argument: --" + ln);
                }

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
            else
            {
                throw std::runtime_error(std::string() + "Could not parse value for argument -" + sn);
            }
        }
    }

    void    ParseLong(int argc, char **argv, int &index)
    {
        std::string     current(argv[index++]);

        if (ArgumentIsMatching(current))
        {
            if (b != nullptr)
            {
                *b = true;
                was_present = true;
            }
            else
            {
                std::string value;
                if (HasEmbeddedValue(current))
                {
                    value = GetEmbeddedValue(current);
                }
                else if (index < argc)
                {
                    value = argv[index++];
                }
                else
                {
                    throw std::runtime_error(std::string() + "missing value for last argument: --" + ln);
                }

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
                    throw std::runtime_error(std::string() + "Could not parse value for argument -" + sn);
                }
            }
        }
    }

    bool    Parse(int argc, char **argv, int &index)
    {
        std::string     current(argv[index]);

        if (current.find_first_not_of('-') == 2)
        {
            ParseLong(argc, argv, index);
        }
        else if (current.find_first_not_of('-') == 1)
        {
            ParseShort(argc, argv, index);
        }
        else
        {
            ++index;
        }

        if (IsRequired() && !IsPresent())
        {
            throw std::runtime_error(std::string() + "Missing argument: " + LongName());
        }

        return was_present;
    }


    //----------------------------------
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


    bool    ParseLong(std::vector<std::string> &args)
    {
        std::string     current = args.front();

        if (ArgumentIsMatching(current))
        {
            args.erase(args.begin()); // Remove recognized option from list.

            if (b != nullptr)
            {
                *b = true;
                was_present = true;
            }
            else
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
                    throw std::runtime_error(std::string() + "Could not parse value for argument --" + ln + " : " + value);
                }
            }
        }

        return was_present;
    }


    void    RemoveShortArgumentFromFront(std::vector<std::string> &args)
    {
        std::string     &current = args.front();

        auto    pos = current.find(sn);
        if (pos != std::string::npos)
        {
            current.erase(pos, 1);
        }

        if (current.compare("-") == 0 || current.empty())
        {
            args.erase(args.begin());
        }
    }


    bool    ParseShort(std::vector<std::string> &args)
    {
        std::string     current = args.front();

        auto    arguments = GetRawArguments(current);

        if (ArgumentIsPresent(arguments))
        {
            RemoveShortArgumentFromFront(args);

            if (b != nullptr)
            {
                *b = true;
                was_present = true;
            }
            else if (ArgumentIsLast(arguments))
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
                    throw std::runtime_error(std::string() + "Could not parse value for argument -" + sn + " : " + value);
                }
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

    bool    Parse(int argc, char **argv)
    {
        int index = 1;

        bool    found{false};
        while (index < argc && !found)
        {
            found = Parse(argc, argv, index);
        }

        return found;
    }

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
    Option              help{'h', "help", "Printing this help", is_help};

    std::string         app_description;
    std::string         app_version;

    std::vector<std::string>    positionals;

    std::vector<Option> options;

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

    std::vector<std::string>    ParseArgs(int argc, char **argv)
    {
        std::vector<std::string>   args(argv, argv + argc);

        return args;
    }

public:
    OptionParser(std::string app_description, std::string app_version)
        : app_description(app_description), app_version(app_version)
    {}

    OptionParser(std::string app_description, std::string app_version, const Option &options)
        : app_description(app_description), app_version(app_version)
    {this->options.push_back(options);}

    OptionParser(std::string app_description, std::string app_version, const std::vector<Option> &options)
        : app_description(app_description), app_version(app_version), options(options)
    {}

    void    Add(const Option &option)
    {
        options.push_back(option);
    }

    bool    IsHelp() const {return is_help;}

    bool    Parse(int argc, char **argv)
    {
        auto    args = ParseArgs(argc, argv);

        if (help.Parse(argc, argv))
        {
            PrintHelp(argv[0]);
            return false;
        }

        for (Option &option : options)
        {
            option.Parse(argc, argv);
        }

        return true;
    }

    bool    ParseOneAtATime(int argc, char **argv)
    {
        Add(help);

        auto    args = ParseArgs(argc, argv);
        args.erase(args.begin());

        while (!args.empty())
        {
            bool    found = false;
            for (Option &option : options)
            {
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
};


#endif  // OPTION_PARSER_H
