#ifndef APPLICATION_BASE_H
#define APPLICATION_BASE_H

#include <string>
#include "option_parser.h"

//-----------------------------------------------------------------------------
class ApplicationBase
{
    bool            is_ok{false};
    OptionParser    options;

protected:
    virtual void    Setup(OptionParser &options) = 0;
    virtual bool    Validate(OptionParser &options)
    {
        return true;
    }

public:
    ApplicationBase(std::string app_description, std::string app_version)
        : options(app_description, app_version)
    {}

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

    bool    IsOk() const {return is_ok;}

    virtual int     Run() = 0;
};

//-----------------------------------------------------------------------------
#endif  // APPLICATION_BASE_H
