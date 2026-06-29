#ifndef LAG_SCREENS_HPP
#define LAG_SCREENS_HPP
namespace lag {

enum class Screens
{
    Greet, Editor
};

inline Screens CurrentScreen{Screens::Greet};

}
#endif
