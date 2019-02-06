#pragma once

#include <string>

namespace trading {

    template<class T, class X>
    struct Asset {
        T val;
        X id;
    };

    template<class T>
    struct Asset<T, std::string> {
        T val;
        std::string id;
    };

}
