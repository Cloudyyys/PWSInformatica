#include "Database.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

std::vector <catTemplate> allCategories;
std::string returnWords[3] = { "b", "back", "return" };     //All the words which are checked to see if to go back to previous page
int const maxCategoryName = 1024;                                 //Max category name
std::string const nameCategoriesFile = "categoriesData.txt";

bool isInputReturnBack(std::string userInput);   //Checks whether inputted std::string is one of the 'returnWords'
void returnProductsCat(catTemplate chosenCat);   //Returns all products in a given category
void returnAllCats();                            //Returns all categories that currently exist
void createNewCat(std::string nameNewCat);       //Creates a new category with a given name in std::string; Adds it to allCategories and creates datafile
void returnLowerCase(std::string& userInput);    //Changes std::string input to lower case
void deleteCat(std::string inputDel);            //Deletes a category from allCategories and associated datafile
void openCategories();

int main()
{
    std::string userInput;
    catTemplate notCategorized(std::string("notCategorized"));
    allCategories.push_back(notCategorized);


    while (true)
    {
        std::cout << "Welcome to our prototype!\n\n Right now you're in the main menu. Please choose what you'd like to access: \nNote: Input is not case sensitive\n";
        std::cout << "Database Management        Type DM\n";
        std::cin >> userInput;
        returnLowerCase(userInput);

        if (userInput == "dm") {
            while (true) {
                std::cout << "You have chosen to read and or modify the database. Please choose what you'd like to do: \n\n";
                std::cout << "View or modify current existable categories      Type VMD\n";
                std::cout << "Add or delete data in categories                 Type ADC\n\n";
                std::cin >> userInput;
                returnLowerCase(userInput);
                if (isInputReturnBack(userInput)) {
                    break;
                }

                if (userInput == "vmd") {
                    while (true) {
                        std::cout << "You have chosen to access the existing categories. Please choose what you'd like to do:\n\n";
                        std::cout << "View currently existing categories           Type VC\n";
                        std::cout << "Modify currently existing categories         Type MC\n\n";
                        std::cin >> userInput;
                        returnLowerCase(userInput);
                        if (isInputReturnBack(userInput)) {
                            break;
                        }

                        if (userInput == "vc") {
                            std::cout << "Currently these categories exist:\n\n";
                            returnAllCats();
                            std::cout << "\n\n";
                        }

                        else if (userInput == "mc") {
                            std::cout << "You have chosen to modify the current categories.\n These are the currently existing categories:\n";
                            returnAllCats();
                            std::cout << "\n Please choose whether to delete (d) or add (a) a category.\n";
                            std::cin >> userInput;
                            returnLowerCase(userInput);

                            if (userInput == "d") {
                                while (true)
                                {
                                    std::cout << "Currently these categories exist:\n\n";
                                    returnAllCats();
                                    std::cout << "\nPlease type in a category name for it to be deleted or return to previous page:    ";
                                    std::cin >> userInput;
                                    if (isInputReturnBack(userInput)) {
                                        break;
                                    }
                                    deleteCat(userInput);
                                }

                            }
                            else if (userInput == "a") {
                                while (true)
                                {
                                    std::cout << "Currently these categories exist:\n\n";
                                    returnAllCats();
                                    std::cout << "\nPlease type in a name for a new category or return to previous page:   ";
                                    std::cin >> userInput;
                                    if (isInputReturnBack(userInput)) {
                                        break;
                                    }
                                    createNewCat(userInput);
                                }

                            }

                        }
                    }

                }

                else if (userInput == "adc")
                {

                    while (true)
                    {
                        std::string userChosenCat;
                        std::cout << "You have chosen to modify data contained in any given category. Please choose which category you'd like to access:\n\n";
                        returnAllCats();
                        std::cout << "\nPlease type in the name of the category you'd like to make changes to:    ";
                        std::cin >> userChosenCat;
                        returnLowerCase(userChosenCat);

                        if (isInputReturnBack(userChosenCat)) {
                            break;
                        }

                        //Return list of items in category (productcode(productname)) in terminal if category n < 10 
                        //Otherwise open textfile to given category database

                        std::cout << "Please decide what you'd like to do with any of the products:  \n\n";
                        std::cout << "Add new product in this category:                           Type A\n";
                        std::cout << "Delete product in this category:                            Type D\n";
                        std::cout << "Access product in this category to gain further info:       Type AP\n\n";
                        std::cin >> userInput;
                        returnLowerCase(userInput);

                        if (userInput == "a") {
                            std::cout << "Please type in the code of the product you'd like to add to this category:   ";
                            std::cin >> userInput;
                            dataCont newInputDatabase(userInput);

                            for (auto& currCat : allCategories) {
                                if (currCat.nameCategory == userChosenCat) {
                                    currCat.category.push_back(newInputDatabase);
                                    
                                    std::ofstream currDataFile;
                                    currDataFile.open(currCat.nameDataFileCat);
                                    if (currDataFile.is_open()) {
                                        currDataFile << newInputDatabase.productCode << "+-&" << std::endl;
                                        currDataFile.close();
                                    }
                                }
                                returnProductsCat(currCat);
                            }
                        }

                        else if (userInput == "mdc")
                        {
                            while (true) {
                                std::cout << "You have chosen to modify data contained in any given category. Please choose what'd you like to do:\n\n";
                                std::cout << "Add new ";
                                std::cout << "Insert new or delete data in the database.              Type NDD";
                                std::cin >> userInput;
                                returnLowerCase(userInput);



                                std::cout << "Please fill in productcode. Further info can be added later.\n";
                                std::cin >> userInput;
                                dataCont newInputDatabase(userInput);
                                std::cout << "Which category would you like to add it in? Currently these are the categories:\n\n";
                                returnAllCats();
                                std::cout << "Please type in one of these categories:   ";
                                std::cin >> userInput;

                                for (auto& currCat : allCategories) {
                                    if (currCat.nameCategory == userInput) {
                                        currCat.category.push_back(newInputDatabase);
                                    }
                                    returnProductsCat(currCat);
                                }

                                std::cout << "\n These are the current products in the category:  \n";
                                std::cout << "Would you like to add more products? Please type y or n.\n";
                                std::cin >> userInput;
                            }
                        }
                    }


                    if (userInput == "delete") {
                        std::cout << "Would you like to delete a product from this category? Please type y or n\n\n";
                        std::string userInputCat;
                        std::cin >> userInput;

                        if (userInput == "y") {
                            std::cout << "What product would you like to delete? Please fill in either the name or code of the product:  ";
                            std::cin >> userInput;
                            std::cout << "You have chosen to delete a product. From which category would you like to delete something?:\n\n";
                            returnAllCats();
                            std::cout << "\n From which category would you like to delete a product? If you'd like to delete something from all categories";
                            std::cout << "\n please type in 'all'.   :    ";
                            std::cin >> userInputCat;

                            if (userInputCat == "all")
                                for (auto currCat : allCategories) {
                                    int currProductInCat = 0;
                                    for (auto& currProduct : currCat.category) {
                                        if (currProduct.productCode == userInput || currProduct.name == userInput) {
                                            currProduct.category.erase(currProduct.category.size() + currProductInCat);
                                        }
                                        currProductInCat++;
                                    }
                                }
                            else if (!(userInputCat == "all")) {
                                for (auto currCat : allCategories) {
                                    if (currCat.nameCategory == userInputCat) {
                                        int currProductInCat = 0;
                                        for (auto& currProduct : currCat.category) {
                                            if (currProduct.productCode == userInput || currProduct.name == userInput) {
                                                currProduct.category.erase(currProduct.category.size() + currProductInCat);
                                            }
                                            currProductInCat++;
                                        }
                                    }
                                }
                            }



                        }
                    }

                }
            }
        }

    }


}



void returnProductsCat(catTemplate chosenCat)
{
    for (auto currProduct : chosenCat.category) {
        std::cout << currProduct.productCode << "(" << currProduct.name << ")\n";
    }
}

void returnAllCats() {
    for (auto currCat : allCategories) {
        std::cout << currCat.nameCategory << "\n";
    }
}

void createNewCat(std::string newNameCat) {
    allCategories.push_back(catTemplate(newNameCat));
    catTemplate Category(newNameCat);
    std::fstream dataFile(nameCategoriesFile, std::ios::in || std::ios::out);

    if (dataFile.is_open()) 
    {
        std::string line;
        while (std::getline(dataFile, line)) {
            dataFile << Category.nameCategory;
        }
        dataFile.close();
    }

}

void deleteCat(std::string inputDel) {
    int currCatNum = 0;
    char stringToCharArray[maxCategoryName];

    for (auto& currCat : allCategories) {
        if (currCat.nameCategory == inputDel) {
            strcpy_s(stringToCharArray, currCat.nameDataFileCat.c_str());
            remove(stringToCharArray);
            allCategories.erase(allCategories.begin() + currCatNum);
        }
        currCatNum++;
    }

}

void returnLowerCase(std::string& userInput) {
    for (auto& c : userInput) {
        c = std::tolower(c);
    }
}

bool isInputReturnBack(std::string userInput) {
    returnLowerCase(userInput);
    for (auto backWord : returnWords) {
        if (backWord == userInput) {
            return true;
        }
    }
    return false;
}

void openCategories() {
    std::fstream catDataFile;
    catDataFile.open("categoriesData.txt");
}