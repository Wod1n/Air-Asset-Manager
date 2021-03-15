#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "dependencies/headers/inventory.h"
#include "dependencies/headers/aircraft.h"
#include "dependencies/headers/location.h"

bool mainMenu(Inventory *embarked){
  int selection{};

  std::cout << "Main Menu\n" << "=========================" << std::endl;

  std::cout << "1. View Embarked Aircraft\n";
  std::cout << "2. View Aircraft in:\n";
  std::cout << "3. Add New Aircraft\n";
  std::cout << "4. Transfer Aircraft\n";
  std::cout << "5. Remove Aircraft\n";
  std::cout << "6. Edit Aircraft\n";
  std::cout << "7. Add Squadron\n";
  std::cout << "8. View Embarked Squadrons\n";
  std::cout << "0. Save and Quit" << std::endl;
  std::cout << "=========================" << std::endl;

  std::cout << "Select Option: ";

  std::cin >> selection;

  switch(selection){
    case 1:{
      std::cout << "-------------------------" << std::endl;
      std::cout << embarked->outputData(0) << std::endl;
      return 1;
    }

    case 2:{
      int newSelection{};
      std::cout << "1. Hanger \n2. Flight Deck \n3. On Mission\n4. In Transit" << std::endl;
      std::cout << "-------------------------" << std::endl;
      std::cout << "Select Option: ";
      std::cin >> newSelection;

      std::cout << embarked->outputData(newSelection) << std::endl;
      return 1;
    }

    case 3:{
      embarked->addAircraft();
      return 1;
    }

    case 4:{
      embarked->transferAircraft();
      return 1;
    }

    case 7:{
      embarked->addSquadron();
      return 1;
    }

    case 8:{
      std::cout << embarked->viewSquadrons() << std::endl;
      return 1;
    }

    case 0:{
      return 0;
    }

    default:{
      std::cout << "Invalid Selection" << std::endl;
      return 1;
    }
  }
}

int main(){

  Inventory *embarked = new Inventory;

  while(mainMenu(embarked));

  delete embarked;

  return 1;
}
