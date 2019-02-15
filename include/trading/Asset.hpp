#pragma once

#include <string>
#include <memory>

namespace trading {

    template<class T, class X>
    struct Asset {
        T val;
        T amt;
        T fee;
        X id;
    };

    template<class T>
    struct Asset<T, std::string> {
        T val;
        T amt;
        T fee;
        std::string id;
    };

    typedef std::shared_ptr<Asset<double, std::string> > AssetPtr;

}
