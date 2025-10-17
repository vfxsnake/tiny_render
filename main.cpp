#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "tgaimage.h"


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
    if (a_x > b_x)
    {
        std::swap(a_x, b_x);
        std::swap(a_y, b_y);
    }
    
    int x_distance = std::abs(a_x - b_x);
    int y_distance = std::abs(a_y - b_y);
    int max_length = std::max(x_distance, y_distance);
    int x_step = (static_cast<float>(x_distance) / static_cast<float>(max_length));
    int y_step = (static_cast<float>(y_distance) / static_cast<float>(max_length));

    for (uint32_t step=0; step < max_length; step++)
    {
        // x_position  = point_a[0] + step * (step_size * step)
        int position_x = a_x + std::roundf(step * x_step);
        int position_y = a_y + std::roundf(step * y_step);
        
        frame_buffer.set(position_x, position_y, color);   
    }
}


int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    std::srand(std::time({}));
    TimeTracker timer = TimeTracker("draw lines method");
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        plot_line(ax, ay, bx, by, framebuffer, {static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255) });
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

