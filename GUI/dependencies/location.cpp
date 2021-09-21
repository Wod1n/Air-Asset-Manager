#include <iostream>

#include "headers/location.h"

void Location::setRegion(int newRegion){
  region = newRegion;
}

void Location::setLocation(int newfly, std::string newspot){
  fly = newfly;
  spot = newspot;
}

void Location::setLocation(int newRegion, int newLeft, int newTop){
  region = newRegion;
  left = newLeft;
  top = newTop;
}

void Location::setRotation(int newRotation){
  rotation = newRotation;
}

int Location::getLeft(){
  return left;
}

int Location::getTop(){
  return top;
}

int Location::getRotation(){
  return rotation;
}

std::string Location::getLocation(){

  std::string locationString{};

  switch (region) {
    case 2:{
      switch (fly) {
        case 0:{
          locationString += "Knuckle: ";
          break;
        }

        case 1:{
          locationString += "Fly 1: ";
          break;
        }

        case 2:{
          locationString += "Fly 2: ";
          break;
        }

        case 3:{
          locationString += "Fly 3: ";
          break;
        }

        case 4:{
          locationString += "Fly 4: ";
          break;
        }

        case 5:{
          locationString += "Graveyard: ";
          break;
        }

        case 6:{
          locationString += "Bridge Park: ";
          break;
        }

        case 7:{
          locationString += "FlyCo Park: ";
          break;
        }

        case 8:{
          locationString += "Fwd Lift: ";
          break;
        }

        case 9:{
          locationString += "Aft Lift: ";
          break;
        }
      }
      locationString += "Spot " + spot;
    }
  }

  return locationString;
}


std::string Location::getSpot(){
  return "Spot";
}

std::string Location::getRegion(){
  switch (region){

    case 0:{
      return "Flight Deck";
    }

    case 1:{
      return "Hanger";
    }

    case 2:{
      return "On Mission";
    }

    case 3:{
      return "In Transit";
    }

    default:{
      return "Unknown";
    }
  }
}

int Location::getRegionCode(){
  return region;
}

std::string Location::saveData(){
  std::string outputString{};
  outputString += std::to_string(region);
  outputString += " " + std::to_string(left) + " " + std::to_string(top) + " " + std::to_string(rotation);
  return outputString;
}

Location::Location(){
  std::cout << "Input Region: ";
  std::cin >> region;
  std::cout << std::endl;

  switch (region) {
    case 2:{
      std::cout << "Input Fly: ";
      std::cin >> fly;

      std::cout << "Input Spot: ";
      std::cin >> spot;
      std::cout << std::endl;
      break;
    }

    default:{
      fly = -1;
      spot = 'Z';
      break;
    }
  }
}

Location::Location(int imRegion, int imFly, char imSpot){
  region = imRegion;
  fly = imFly;
  spot = imSpot;
}

Location::Location(int imRegion, int imLeft, int imTop, int imRotation){
  region = imRegion;
  left = imLeft;
  top = imTop;
  rotation = imRotation;
}
