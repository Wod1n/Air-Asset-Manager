#include "headers/shape.h"

int Shape::getWidth(bool stowed){
  if (stowed) {
    return stowedWidth;
  }
  else{
    return width;
  }
}

int Shape::getHeight(){
  return height;
}

Shape::Shape(int imWidth, int imHeight, int imStowedWidth){
  width = imWidth;
  height = imHeight;
  stowedWidth = imStowedWidth;
}
