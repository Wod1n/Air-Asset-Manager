#ifndef SHAPE_H
#define SHAPE_H

class Shape{
  private:
    int width;
    int height;
    int stowedWidth;
  public:
    int getWidth(bool);
    int getHeight();

    Shape(int, int, int);
};

#endif
