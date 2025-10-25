#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <tuple>
#include <algorithm>
#include "tgaimage.h"
#include "model.h"


constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

class TimeTracker
{
public:
    TimeTracker(const char* function_name) : _function_name(function_name)
    {
        _start_time = std::chrono::steady_clock::now();
    }
    
    ~TimeTracker()
    {
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        auto lapse = end_time - _start_time;
        using _seconds = std::chrono::seconds;
        auto time_in_seconds = std::chrono::duration_cast<_seconds>(lapse);
        std::cout << "function " << _function_name << " took: " <<  time_in_seconds;
    }

private:
    std::chrono::steady_clock::time_point _start_time;
    const char* _function_name;
};

void plot_line(int a_x, int a_y, int b_x, int b_y, TGAImage& frame_buffer, TGAColor color)
{
    bool steep = std::abs(a_x-b_x) < std::abs(a_y-b_y);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(a_x, a_y);
        std::swap(b_x, b_y);
    }
    if (a_x>b_x) { // make it left−to−right
        std::swap(a_x, b_x);
        std::swap(a_y, b_y);
    }
    int y = a_y;
    int ierror = 0;
    for (int x=a_x; x<=b_x; x++) 
    {
        if (steep) // if transposed, de−transpose
            frame_buffer.set(y, x, color);
        else
            frame_buffer.set(x, y, color);
        ierror += 2 * std::abs(b_y-a_y);
        if (ierror > b_x - a_x) {
            y += b_y > a_y ? 1 : -1;
            ierror -= 2 * (b_x-a_x);
        }
    }
}


std::tuple<int, int> project(Vec3f vec3, float offset_x, float offset_y, int width, int height)
{
    return {
        round((vec3.x + offset_x) * width * 0.5),
        round((vec3.y + offset_y) * height * 0.5)
    };
}


/*
    will return true if there is an intersection point of the segment and the y_val
    * will return false if there is not intersection, 
    * will return false if the segments y's values are the same of y_val. as it is the same segment. 
*/
bool intersect_segment(int y_val, int ax, int ay, int bx, int by, int& x_out)
{
    if (y_val < std::min({ay, by}) || y_val > std::max(ay, by))
    {
        x_out = ax;
        return false;
    }
    int delta_x = bx - ax;
    // if line a constant along y (parallel to y axis)
    if (delta_x == 0)
    {
        x_out = ax;  // intersects at ax, y_val
        return true; 
    }

    int delta_y = by - ay;
    if (delta_y == 0)
    {
        int& x_out = ax;
        return false;
    }

    // get the curve function elements y = m(x) + B;
    float m = delta_y / float(delta_x);

    int B = round(float(ay) - (m * float(ax)));

    float x = (y_val - B) / m;
    
    x_out = round(x);
    int min_x = std::min(ax, bx);
    int max_x = std::max(ax, bx);
    if (x_out >= min_x && x_out <= max_x)
    {
        return true;
    }

    return false;
}


void rasterize_triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& frame_buffer, TGAColor color)
{
    int min_x = std::min({ax, bx, cx});
    int max_x = std::max({ax, bx, cx});
    int min_y = std::min({ay, by, cy});
    int max_y = std::max({ay, by, cy});


    for (int y = min_y; y <= max_y ; y++)
    {
        // stores the x cordinates of the intersected points
        std::vector<int> x_cords;
        int x_cord = 0;
        if (intersect_segment(y, ax, ay, bx, by, x_cord))
        {
            x_cords.emplace_back(x_cord);
        }
        
        if (intersect_segment(y, bx, by, cx, cy, x_cord))
        {
            x_cords.emplace_back(x_cord);
        }
        
        if (intersect_segment(y, cx, cy, ax, ay, x_cord))
        {
            x_cords.emplace_back(x_cord);
        }

        if (x_cords.size() <= 1)
        {
            continue;
        }

        auto [_min, _max] = std::minmax_element(x_cords.begin(), x_cords.end());
        for (int x = *_min; x <= *_max; x++)
        {
            frame_buffer.set(x, y, color);
        }

    }
}




void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) 
{
    rasterize_triangle(ax, ay, bx, by, cx, cy, framebuffer, color);
    // plot_line(ax, ay, bx, by, framebuffer, white);
    // plot_line(bx, by, cx, cy, framebuffer, white);
    // plot_line(cx, cy, ax, ay, framebuffer, white);
}



int width = 128;
int height = 128;

int main(int argc, char** argv)
{
    TGAImage framebuffer(width, height, TGAImage::RGB);
    triangle(  7, 45, 35, 100, 45,  60, framebuffer, red);
    triangle(120, 35, 90,   5, 45, 110, framebuffer, blue);
    triangle(115, 83, 80,  90, 85, 120, framebuffer, green);
    framebuffer.write_tga_file("../framebuffer.tga");
    return 0;
}

