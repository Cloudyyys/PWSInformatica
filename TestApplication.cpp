#include "Database.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <ShellApi.h>
#include <tchar.h>
#include <sstream>

std::vector <catTemplate> allCategories;
std::string returnWords[3] = { "b", "back", "return" };     //All the words which are checked to see if to go back to previous page
int const maxCategoryName = 1024;                                 //Max category name
std::string const dirCategoriesFile = "C:\\InformaticaDatabases\\";


void startupSequence();                          //Is called at the very beginning of the program, doing anything necessary.
bool isInputReturnBack(std::string userInput);   //Checks whether inputted std::string is one of the 'returnWords'
void returnProductsCat(catTemplate chosenCat);   //Returns all products in a given category
void returnAllCats();                            //Returns all categories that currently exist
void createNewCat(std::string nameNewCat);       //Creates a new category with a given name in std::string; Adds it to allCategories and creates datafile
void returnLowerCase(std::string& userInput);    //Changes std::string input to lower case
void deleteCat(std::string inputDel);            //Deletes a category from allCategories and associated datafile
void openCategories();
void createCategoriesFile();                     //Creates a 'categories file' if one does not exist already, filling it only with 'notCategorised'.
void loadCategories();                           //Read all categories in 'categories file' and place content into the vector 'allCategories' for further use.
void addDataToCategory(std::string categoryName);
void deleteDataFromCategory(std::string chosenCat);
void deleteProductCategory(std::string userGivenProduct, std::string userChosenCategory);
void accessGivenProduct();
void changeProductLine(std::string inputChanges, dataCont& replaceThisProduct);
void outputGivenProduct(dataCont givenProduct);
void checkoutSystem();
void changeDatabaseProduct(std::string userChosenCategory, dataCont productToReplace, dataCont newProduct);
std::string returnDatabaseLineProduct(dataCont givenProduct);
dataCont returnProduct(std::string productLine);
dataCont returnProductCategory(std::string categoryName, std::string productName);

int main()
{
    std::string userInput;
    startupSequence();


    while (true)
    {
        std::cout << "Welcome to our prototype!\n\n Right now you're in the main menu. Please choose what you'd like to access: \nNote: Input is not case sensitive\n";
        std::cout << "Database Management        Type DM\n";
        std::cout << "Checkout-System            Type CS\n";
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
                        std::string userChosenCat;        //Which category would you like to modify data in
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

                        std::cout << "Please decide what you'd like to do with any of the categories:  \n\n";
                        std::cout << "Add new product in this category:                           Type A\n";
                        std::cout << "Delete product in this category:                            Type D\n";
                        std::cout << "Access product in this category to gain further info:       Type AP\n\n";
                        std::cin >> userInput;
                        returnLowerCase(userInput);

                        if (userInput == "a") {
                            addDataToCategory(userChosenCat);
                        }

                        else if (userInput == "d") {
                            std::cout << "Would you like to delete a product from this category? Please type y or n\n\n";
                            std::string userInputCat;
                            std::cin >> userInput;

                            if (userInput == "y") {
                                deleteDataFromCategory(userChosenCat);
                            }
                        }

                        else if (userInput == "ap") {
                            accessGivenProduct();
                        }
                    }
                }
            }
        }
        else if (userInput == "cs") {

        checkoutSystem();
}   

    }
}

void startupSequence() {
    createCategoriesFile();
    loadCategories();
}

void createCategoriesFile() {
    std::ifstream openSettingsFile(dirCategoriesFile + "allCategories.txt");
    if (!(openSettingsFile.is_open())) {
        std::ofstream settingsFile(dirCategoriesFile + "allCategories.txt", std::ios::app);
        if (settingsFile.is_open()) {
            settingsFile << "notCategorised\n";
        }
        settingsFile.close();
    }
    openSettingsFile.close();
}

void loadCategories() {
    std::fstream openSettingsFile(dirCategoriesFile + "allCategories.txt");
    std::string lineCatFile;

    while (std::getline(openSettingsFile, lineCatFile)) {
        catTemplate addedCategory(lineCatFile);
        allCategories.push_back(addedCategory);
    }
    openSettingsFile.close();
}

void returnAllCats() {
    for (auto currCat : allCategories) {
        std::cout << currCat.nameCategory << "\n";
    }
}

void createNewCat(std::string newNameCat) {
    catTemplate newCategory(newNameCat);
    allCategories.push_back(newCategory);
    std::fstream categoryFile(dirCategoriesFile + "allCategories.txt", std::ios::app);

    if (categoryFile.is_open()) 
    {
        categoryFile << newCategory.nameCategory << "\n";
        categoryFile.close();
    }

    std::ofstream newDatabaseFile(dirCategoriesFile + newCategory.nameDataFileCat, std::ios::app);
    if (newDatabaseFile.is_open()) {
        newDatabaseFile.close();
    }
}

void deleteCat(std::string inputDel) {
    int currCatNum = 0;
    char stringToCharArray[maxCategoryName];

    for (auto& currCat : allCategories) {
        if (currCat.nameCategory == inputDel) {
            std::string deleteThisCatFile = dirCategoriesFile + currCat.nameDataFileCat;
            strcpy_s(stringToCharArray, deleteThisCatFile.c_str());
            remove(stringToCharArray);
            allCategories.erase(allCategories.begin() + currCatNum);
        }
        currCatNum++;
    }

    std::ifstream categoryFile(dirCategoriesFile + "allCategories.txt", std::ios::app);
    std::string currLine, newFile;
    while (std::getline(categoryFile, currLine)) {
        if ((!(currLine.empty() || currLine.find_first_not_of(' ') == std::string::npos)) && !(currLine == inputDel)) {
            newFile += currLine + "\n";
        }
    }
    categoryFile.close();

    std::ofstream categoryFileIn(dirCategoriesFile + "allCategories.txt");
    categoryFileIn << newFile;
    categoryFileIn.close();
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

void addDataToCategory(std::string categoryName) {
    std::string userInput;

    std::cout << "Please type in the code of the product you'd like to add to this category:   ";
    std::cin >> userInput;
    dataCont newInputDatabase(userInput);

    for (auto& currCat : allCategories) {
        if (currCat.nameCategory == categoryName) {
            currCat.category.push_back(newInputDatabase);

            std::ofstream currDataFile(dirCategoriesFile + currCat.nameDataFileCat, std::ios::app);
            if (currDataFile.is_open()) {
                currDataFile << "&" << newInputDatabase.productCode << "&&&&&&&\n";
                currDataFile.close();
            }
        }
    }
}

void deleteDataFromCategory(std::string chosenCat) {
    std::string userInput;
    std::cout << "What product would you like to delete? Please fill in either the name or code of the product:  ";
    std::cin >> userInput;
    std::cout << "You have chosen to delete a product. From which category would you like to delete something?:\n\n";
    returnAllCats();
    std::cout << "\n From which category would you like to delete a product? If you'd like to delete something from all categories";
    std::cout << "\n please type in 'all'.   :    ";
    std::cin >> chosenCat;

    deleteProductCategory(userInput, chosenCat);

}

void deleteProductCategory(std::string userGivenProduct, std::string userChosenCategory) {
    for (auto currCat : allCategories) {
        if (currCat.nameCategory == userChosenCategory) {
            std::fstream streamCategory(dirCategoriesFile + currCat.nameDataFileCat);
            std::string currLine, newFile;

            while (std::getline(streamCategory, currLine)) {
                dataCont currentProduct = returnProduct(currLine);
                if (!(currentProduct.productCode == userGivenProduct) && !(currentProduct.name == userGivenProduct)) {
                    newFile += currLine + "\n";
                }
            }

            streamCategory.close();
            std::ofstream databaseFileIn(dirCategoriesFile + currCat.nameDataFileCat);
            databaseFileIn << newFile;
            databaseFileIn.close();

            int currProductInCat = 0;
            for (auto& currProduct : currCat.category) {
                if (currProduct.productCode == userGivenProduct || currProduct.name == userGivenProduct) {
                    currProduct.category.erase(currProduct.category.size() + currProductInCat);
                }
                currProductInCat++;
            }
        }
    }
}

dataCont returnProduct(std::string productLine) {
    int firstDelimiterPos, secondDelimiterPos, currCharLine = 0, currAttributeNumber = 0;
    std::size_t lengthString = productLine.size();
    std::string currAttribute;
    bool firstDelimiterFound = false;
    std::string allAtributesCurrProduct[7];

    currAttribute.resize(lengthString);
    while (currAttributeNumber < 7)
    {
        for (auto currChar : productLine) {

            if (currChar == '&' && firstDelimiterFound == false) {
                firstDelimiterPos = currCharLine;
                firstDelimiterFound = true;
            }
            else if (currChar == '&' && firstDelimiterFound == true) {
                secondDelimiterPos = currCharLine;
                currAttribute = productLine.substr((firstDelimiterPos + 1), (secondDelimiterPos - 1));
                allAtributesCurrProduct[currAttributeNumber] = currAttribute;
                currAttributeNumber++;
                break;
            }
            currCharLine++;
        }
        productLine = productLine.substr(secondDelimiterPos, productLine.size() - secondDelimiterPos);
        firstDelimiterFound = false;
        currCharLine = 0;
    }

    dataCont returnProduct(allAtributesCurrProduct[0]);
    returnProduct.name = allAtributesCurrProduct[1];
    returnProduct.category = allAtributesCurrProduct[2];
    returnProduct.price = std::stoi(allAtributesCurrProduct[3]);
    returnProduct.inStorage = std::stoi(allAtributesCurrProduct[4]);
    returnProduct.sales = std::stoi(allAtributesCurrProduct[5]);
    returnProduct.remarks = allAtributesCurrProduct[6];

    return returnProduct;
}

dataCont returnProductCategory(std::string categoryName, std::string productName) {
    for (auto currCat : allCategories)
    {
        if (currCat.nameCategory == categoryName)
        {
            std::fstream streamCategory(dirCategoriesFile + currCat.nameDataFileCat);
            std::string currLine, newFile;
            while (std::getline(streamCategory, currLine)) {
                dataCont currentProduct = returnProduct(currLine);
                if (currentProduct.productCode == productName || currentProduct.name == productName) {
                    return currentProduct;
                }
            }
        }
    }
}

void outputGivenProduct(dataCont givenProduct) {
    std::ostringstream outputFloat, outputIntStorage, outputIntSales;
    outputFloat << givenProduct.price;
    outputIntStorage << givenProduct.inStorage;
    outputIntSales << givenProduct.sales;

    std::string productOutputLine = givenProduct.name + " (" + givenProduct.productCode + ")" +
        "\nCategory:     " + givenProduct.category +
        "\nPrice:        " + outputFloat.str() +
        "\nIn storage:   " + outputIntStorage.str() +
        "\nTotal Sales:  " + outputIntSales.str() +
        "\nRemarks:      " + givenProduct.remarks;
    std::cout << productOutputLine;
}

void changeProductLine(std::string inputChanges, dataCont& replaceThisProduct) {

    int firstDelimiterPos, secondDelimiterPos, currCharLine = 0, currAttributeNumber = 0;
    std::size_t lengthString = inputChanges.size();
    std::string currAttribute;
    bool firstDelimiterFound = false;
    std::string allAtributesCurrProduct[7];

    currAttribute.resize(lengthString);
    while (currAttributeNumber < 7)
    {
        for (auto currChar : inputChanges) {

            if (currChar == ',' && firstDelimiterFound == false) {
                firstDelimiterPos = currCharLine;
                firstDelimiterFound = true;
            }
            else if (currChar == ',' && firstDelimiterFound == true) {
                secondDelimiterPos = currCharLine;
                currAttribute = inputChanges.substr((firstDelimiterPos + 1), (secondDelimiterPos - 1));
                allAtributesCurrProduct[currAttributeNumber] = currAttribute;
                currAttributeNumber++;
                break;
            }
            currCharLine++;
        }
        inputChanges = inputChanges.substr(secondDelimiterPos, inputChanges.size() - secondDelimiterPos);
        firstDelimiterFound = false;
        currCharLine = 0;
    }

    replaceThisProduct.productCode = allAtributesCurrProduct[0];
    replaceThisProduct.name = allAtributesCurrProduct[1];
    replaceThisProduct.category = allAtributesCurrProduct[2];
    replaceThisProduct.price = std::stoi(allAtributesCurrProduct[3]);
    replaceThisProduct.inStorage = std::stoi(allAtributesCurrProduct[4]);
    replaceThisProduct.sales = std::stoi(allAtributesCurrProduct[5]);
    replaceThisProduct.remarks = allAtributesCurrProduct[6];
}


void changeDatabaseProduct(std::string userChosenCategory, dataCont productToReplace, dataCont newProduct) {
    for (auto currCat : allCategories) {
        if (currCat.nameCategory == userChosenCategory) {
            std::fstream streamCategory(dirCategoriesFile + currCat.nameDataFileCat);
            std::string currLine, newFile;

            while (std::getline(streamCategory, currLine)) {
                dataCont currentProduct = returnProduct(currLine);
                if (!(currentProduct.productCode == productToReplace.productCode) && !(currentProduct.name == productToReplace.name)) {
                    newFile += currLine + "\n";
                }
                else if ((currentProduct.productCode == productToReplace.productCode) && (currentProduct.name == productToReplace.name)) {
                    newFile += newProduct.returnDatabaseLine() + "\n";
                }
            }

            streamCategory.close();
            std::ofstream databaseFileIn(dirCategoriesFile + currCat.nameDataFileCat);
            databaseFileIn << newFile;
            databaseFileIn.close();

        }
    }
}
void accessGivenProduct() {
    std::string userInputCategory, userInputProduct, userInputChanges;

    std::cout << "You've chosen to access information from a given product.\n";
    std::cout << "Please select a category in which this product is situated:  ";
    std::cin >> userInputCategory;
    std::cout << "\n\nPlease fill in either the code or name of the product:  ";
    std::cin >> userInputProduct;


    dataCont chosenProduct = returnProductCategory(userInputCategory, userInputProduct);
    std::cout << "This is the information from the product you've chosen: \n\n";
    outputGivenProduct(chosenProduct);
    std::cout << "\n\n Please type in one line the changes you'd like to make to each attribute seperated by a comma.\n";
    std::cout << "Two special symbols may be used:\n";
    std::cout << "Fill in a '-' to leave the attribute as it is.\n";
    std::cout << "Fill in a '/' to empty the attribute.\n";
    std::cout << "Please enter your changes:   ";
    std::cin >> userInputChanges;
    userInputChanges.insert(0, ",");
    userInputChanges.append(",");
    dataCont changedNewProduct = chosenProduct;

    changeProductLine(userInputChanges, changedNewProduct);
    //Now 'chosenProduct' is product in database which needs to be changed, 'changedNewProduct' is what it needs to be replaced with

    changeDatabaseProduct(userInputCategory, chosenProduct, changedNewProduct);
}

void checkoutSystem() {
    std::string userInputProduct, userInputCategory;
    std::vector <dataCont> shoppingCart;

    std::cout << "You've chosen to use the checkout-system.\n";
    std::cout << "Here you can add products to a shopping cart which upon user request can be purchased.\n";
    std::cout << "Please fill in the category which the product is located it. If it is unknown, fill in UNK. \nn";
    std::cout << "If you'd like to access the shopping cart please fill in SC:   ";
    std::cin >> userInputCategory;
    std::cout << "\n\nPlease fill in either the name or code of the product:   ";
    std::cin >> userInputProduct;

    dataCont chosenProduct = returnProductCategory(userInputCategory, userInputProduct);
}