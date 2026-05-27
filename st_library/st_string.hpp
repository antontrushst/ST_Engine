#ifndef ST_STRING_HPP
#define ST_STRING_HPP

#include <iostream>
#include <string>
#include <string_view>

namespace st{

class String
{
    std::string  string;
    unsigned int lineWidth;
public:
    String(const std::string &text = "", const unsigned int &wrapLimit = 0)
        : string{[&]() -> std::string
            {
                if(text.size() <= wrapLimit)
                    return text;

                std::string temp;
                for(int i{0}; i < text.size();)
                {
                    if(i + wrapLimit >= text.size())
                    {
                        temp += text.substr(i);
                        break;
                    }

                    std::string_view sView{text.c_str() + i, wrapLimit};
                    if(sView.find('\n') != std::string::npos)
                    {
                        temp += text.substr(i, sView.find('\n') + 1);
                        i += sView.find('\n') + 1;
                        continue;
                    }

                    temp += text.substr(i, wrapLimit) + "\n";
                    i += wrapLimit;
                }
                return temp;
            }()}
        , lineWidth{wrapLimit}
    {}

    friend std::ostream& operator <<(std::ostream &os, const String &string)
    {
        os << string.string;
        return os;
    }
};

}

#endif
