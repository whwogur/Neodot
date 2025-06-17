#pragma once

namespace Neodot::DS
{
    template<typename T>
    struct DimensionsT
    {
        //===========
        // functions 
        //===========
        T GetArea() const
        {
            return width * height;
        }

        //===========
        // data
        //===========
        T width, height;
    };

    using DimensionsF = DimensionsT<float>;
    using DimensionsI = DimensionsT<int>;
}