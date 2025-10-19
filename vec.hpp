#pragma once

#include <vector>
#include <array>
#include <cassert>
/*
    base template structure for multi-dimensional vectors, mainly for graphic use
    it can represent a 2d, 3d, 4d vectors and so on. 
*/
template <typename T, int n>
struct Vec
{
    static_assert(n > 0, "vector dimension must be positive");
    std::array<T, n> components;

    // default constructor:
    Vec()
    {
        std::fill(components.begin(), components.end(), 0);
    }

    Vec(T value)
    {
        std::fill(components.begin(), components.end(), value);
    }

    T& operator[](const int index)
    {
        assert(index >=0 && index < n);
        return components[index];
    }

    const T& operator[](const int index) const
    {
        assert(index >=0 && index < n);
        return components[index];
    }

};

template <typename T>
struct Vec<T,2>
{
    // union overlays the memory of the 2 structures in this case array[2] and struct[x,y]
    union
    {
        std::array<T,2> components;
        struct {T x, y;};
    };
    
    Vec()
    {
        std::fill(components.begin(), components.end(), 0);
    }

    Vec(T value)
    {
        std::fill(components.begin(), components.end(), value);
    }

    Vec(T _x, T _y) : x(_x), y(_y) { }


    T& operator[](const int index)
    {
        assert(index >=0 && index < 2);
        return components[index];
    }

    const T& operator[](const int index) const
    {
        assert(index >=0 && index < 2);
        return components[index];
    }
};


template <typename T>
struct Vec<T,3>
{
    // union overlays the memory of the 2 structures in this case array[2] and struct[x,y]
    union
    {
        std::array<T,3> components;
        struct {T x, y, z;};
    };
    
    Vec()
    {
        std::fill(components.begin(), components.end(), 0);
    }

    Vec(T value)
    {
        std::fill(components.begin(), components.end(), value);
    }

    Vec(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }


    T& operator[](int index)
    {
        assert(index >=0 && index < 3, "index out of range");
        return components[index];
    }

    const T& operator[](const int index) const
    {
        assert(index >=0 && index < 3);
        return components[index];
    }
};

template <typename T>
struct Vec<T,4>
{
    // union overlays the memory of the 2 structures in this case array[2] and struct[x,y]
    union
    {
        std::array<T,4> components;
        struct {T x, y, z, w;};
    };
    
    Vec()
    {
        std::fill(components.begin(), components.end(), 0);
    }

    Vec(T value)
    {
        std::fill(components.begin(), components.end(), value);
    }

    Vec(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }


    T& operator[](const int index)
    {
        assert(index >=0 && index < 4);
        return components[index];
    }

    const T& operator[](const int index) const
    {
        assert(index >=0 && index < 4);
        return components[index];
    }
};

/*
    TODO: add operation overloads
*/
template <typename T, int n>
Vec<T, n> operator +(const Vec<T,n>& a, const Vec<T,n>& b)
{
    Vec<T, n> result;
    for (int i = 0; i < n; i++)
    {
        result[i] =  a[i] + b[i];
    }
    return result;
}

template <typename T, int n>
Vec<T, n> operator -(const Vec<T,n>& a, const Vec<T,n>& b)
{
    Vec<T, n> result;
    for (int i = 0; i < n; i++)
    {
        result[i] =  a[i] - b[i];
    }
    return result;
}

template <typename T>
using Vec2 = Vec<T, 2>;

template <typename T>
using Vec3 = Vec<T, 3>;

template <typename T>
using Vec4 = Vec<T, 4>;

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;

using Vec2i = Vec<int, 2>;
