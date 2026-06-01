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
                    // skip first character of a line if it is empty space
                    if(text[i] == ' ')
                    {
                        i++;
                        continue;
                    }
                    // break the loop if at the end of the text
                    if(i + wrapLimit >= text.size())
                    {
                        temp += text.substr(i);
                        break;
                    }
                    // skip iteration if a line already has newline character
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
                        int wordBegin{0};
                        int wordEnd{0};
                        // get the fist index of a word
                        for(int c{i + (int)wrapLimit - 1}; c >= 0; --c)
                        {
                            if(c == 0)
                            {
                                wordBegin = c;
                                break;
                            }
                            if(text[c - 1] == ' ' || text[c - 1] == '\n')
                            {
                                wordBegin = c;
                                break;
                            }
                        }
                        // get the last index of a word
                        for(int c{i + (int)wrapLimit}; c < text.size(); ++c)
                        {
                            if(c == text.size() - 1)
                            {
                                wordEnd = c;
                                break;
                            }
                            if(text[c] == ' ' || text[c] == '\n')
                            {
                                wordEnd = c;
                                break;
                            }
                        }
                        // break the line early and move the whole word to
                        // the next line if its size is in bounds of wrap limit
                        if(wordEnd - wordBegin < wrapLimit)
                        {
                            temp += text.substr(i, wordBegin - i);
                            temp += "\n";
                            i = wordBegin;
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

    const std::string& getString() const {return this->string;}
    std::string& getString() {return this->string;}
//    std::string getString() {return string;}
};

}

#endif
