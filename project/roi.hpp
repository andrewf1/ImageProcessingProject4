
#ifndef ROI_HPP
#define ROI_HPP

class roi 
{
public:
    roi() = default;
    roi(int x_i, int y_i, int sx_i, int sy_i) {
            x = x_i;
            y = y_i;
            sx = sx_i;
            sy = sy_i;
        };
    ~roi() = default;
    
    int x;
    int y;
    int sx;
    int sy;
    int a, b; // histogram stretching
};

#endif