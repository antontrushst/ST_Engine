#ifndef ST_STRING_HPP
#define ST_STRING_HPP

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

namespace st{

class String
{
    std::string  string;
    unsigned int lineWidth;
public:
    String(const std::string &text = "", const unsigned int &wrapLimit = 0)
        : string{[&]() -> std::string
            {
                if(text.size() <= wrapLimit || wrapLimit == 0)
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

                    // if wrap occurs in the middle of a word then try to
                    // move the whole word to the next line
                    if(sView.back() != ' ' && text[i + wrapLimit] != ' '
                        && text[i + wrapLimit] != '\n')
                    {
                        std::string word;
                        int wordStart{0};
                        int wordEnd{0};
                        for(int c{i + (int)wrapLimit - 1}; c >= 0; --c)
                        {
                            if(text[c] == ' ')
                            {
                                wordStart = c + 1;
                                break;
                            }
                            word += text[c];
                        }
                        std::reverse(word.begin(), word.end());
                        for(int c{i + (int)wrapLimit - 1}; c < text.size(); ++c)
                        {
                            if(text[c] == ' ')
                            {
                                wordEnd = c - 1;
                                break;
                            }
                            word += text[c];
                        }
                        if(wordEnd - wordStart < wrapLimit)
                        {
                            temp += text.substr(i, wordStart - i) + "\n";
                            i = wordStart;
                            continue;
                        }
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
