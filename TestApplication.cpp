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
void returnAllCatsToUser();                            //Returns all categories that currently exist as output in list.
void createNewCat(std::string nameNewCat);       //Creates a new category with a given name in std::string; Adds it to allCategories and creates datafile
void makeLowerCase(std::string& userInput);    //Changes std::string input to lower case
void deleteCat(std::string inputDel);            //Deletes a category from allCategories vector, deletes text datafile and changes allCategories datafile
void createCategoriesFile();                     //Creates a 'categories file' if one does not exist already, filling it only with 'notCategorised'.
void loadCategories();                           //Read all categories in 'categories file' and place content into the vector 'allCategories' for further use.
void addDataToCategory(std::string categoryName); //Adds a new line into a category datafile with only product code.
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
        makeLowerCase(userInput);

        if (userInput == "dm") {
            while (true) {
                std::cout << std::string(100, '\n');
                std::cout << "You have chosen to read and or modify the database. Please choose what you'd like to do: \n\n";
                std::cout << "View or modify current existable categories      Type VMD\n";
                std::cout << "Add or delete data in categories                 Type ADC\n\n";
                std::cin >> userInput;
                makeLowerCase(userInput);
                if (isInputReturnBack(userInput)) { break; }

                if (userInput == "vmd") {
                    while (true) {
                        std::cout << std::string(100, '\n');
                        std::cout << "You have chosen to access the existing categories. Please choose what you'd like to do:\n\n";
                        std::cout << "View currently existing categories           Type VC.\n";
                        std::cout << "Modify currently existing categories         Type MC.\n\n";
                        std::cin >> userInput;
                        makeLowerCase(userInput);
                        if (isInputReturnBack(userInput) == true) { break; }

                        if (userInput == "vc") {
                            std::cout << std::string(100, '\n');
                            std::cout << "Currently these categories exist:\n\n";
                            returnAllCatsToUser();
                            std::cout << "\n\n";
                        }

                        else if (userInput == "mc") {
                            std::cout << std::string(100, '\n');
                            std::cout << "You have chosen to modify the current categories.\n These are the currently existing categories:\n";
                            returnAllCatsToUser();
                            std::cout << "\n Please choose whether to delete (d) or add (a) a category.\n";
                            std::cin >> userInput;
                            makeLowerCase(userInput);

                            if (userInput == "d") {
                                while (true)
                                {
                                    std::cout << std::string(100, '\n');
                                    std::cout << "Currently these categories exist:\n\n";
                                    returnAllCatsToUser();
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
                                    std::cout << std::string(100, '\n');
                                    std::cout << "Currently these categories exist:\n\n";
                                    returnAllCatsToUser();
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
                        std::cout << std::string(100, '\n');
                        std::string userChosenCat;        //Which category would you like to modify data in
                        std::cout << "You have chosen to modify data contained in any given category. Please choose which category you'd like to access:\n\n";
                        returnAllCatsToUser();
                        std::cout << "\nPlease type in the name of the category you'd like to make changes to:    ";
                        std::cin >> userChosenCat;
                        makeLowerCase(userChosenCat);

                        if (isInputReturnBack(userChosenCat) == true) { break; }



                        //Return list of items in category (productcode(productname)) in terminal if category n < 10 
                        //Otherwise open textfile to given category database
                        std::cout << std::string(100, '\n');
                        std::cout << "Please decide what you'd like to do with any of the categories:  \n\n";
                        std::cout << "Add new product in this category:                           Type A\n";
                        std::cout << "Delete product in this category:                            Type D\n";
                        std::cout << "Access product in this category to gain further info:       Type AP\n\n";
                        std::cin >> userInput;
                        if (isInputReturnBack(userInput) == true) { break; }
                        makeLowerCase(userInput);

                        if (userInput == "a") {
                            addDataToCategory(userChosenCat);
                        }

                        else if (userInput == "d") {
                            deleteDataFromCategory(userChosenCat);
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

void returnAllCatsToUser() {
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

    for (auto& currCat : allCategories) //Deletes the datafile with name inputDel, and removes category from allCategories vector
    {
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

void makeLowerCase(std::string& userInput) {
    for (auto& c : userInput) {
        c = std::tolower(c);
    }
}

bool isInputReturnBack(std::string userInput) {
    makeLowerCase(userInput);
    for (auto backWord : returnWords) {
        if (backWord == userInput) {
            return true;
        }
    }
    return false;
}

void addDataToCategory(std::string categoryName) {
    std::string userInput;

    while (true)
    {
        std::cout << std::string(100, '\n');
        std::cout << "Please type in the code of the product you'd like to add to this category:   ";
        std::cin >> userInput;

        if (isInputReturnBack(userInput) == true) { break; }
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
}

void deleteDataFromCategory(std::string chosenCat) {
    std::string userInput;
    while (true)
    {
        std::cout << std::string(100, '\n');
        std::cout << "What product would you like to delete? Please fill in either the name or code of the product:  ";
        std::cin >> userInput;
        if (isInputReturnBack(userInput) == true) { break; }

        deleteProductCategory(userInput, chosenCat);
    }

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

        else if (categoryName == "ALL") {

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

    while (true)
    {
        std::cout << std::string(100, '\n');
        std::cout << "You've chosen to access information from a given product.\n";
        std::cout << "Please select a category in which this product is situated:  ";
        std::cin >> userInputCategory;
        if (isInputReturnBack(userInputCategory) == true) { break; }
        std::cout << "\n\nPlease fill in either the code or name of the product:  ";
        std::cin >> userInputProduct;
        if (isInputReturnBack(userInputProduct) == true) { break; }

        dataCont chosenProduct = returnProductCategory(userInputCategory, userInputProduct);
        std::cout << "This is the information from the product you've chosen: \n\n";
        outputGivenProduct(chosenProduct);
        std::cout << "\n\n Please type in one line the changes you'd like to make to each attribute seperated by a comma.\n";
        std::cout << "Two special symbols may be used:\n";
        std::cout << "Fill in a '-' to leave the attribute as it is.\n";
        std::cout << "Fill in a '/' to empty the attribute.\n";
        std::cout << "Please enter your changes:   ";
        std::cin >> userInputChanges;
        if (isInputReturnBack(userInputChanges) == true) { break; }
        userInputChanges.insert(0, ",");
        userInputChanges.append(",");
        dataCont changedNewProduct = chosenProduct;

        changeProductLine(userInputChanges, changedNewProduct);
        //Now 'chosenProduct' is product in database which needs to be changed, 'changedNewProduct' is what it needs to be replaced with

        changeDatabaseProduct(userInputCategory, chosenProduct, changedNewProduct);
    }
}

void checkoutSystem() {
    std::string userInputProduct, userInputCategory;
    int currentShoppingCartVector = 0;
    std::vector <dataCont> shoppingCart;
    std::vector <int> shoppingCartQuantities;

    while (true)
    {
        std::cout << std::string(100, '\n');
        std::cout << "You've chosen to use the checkout-system.\n";
        std::cout << "Here you can add products to a shopping cart which upon user request can be purchased.\n";
        std::cout << "If you'd like to access the shopping cart at any moment please fill in SC.\n";
        std::cout << "If you'd like to add products please type in the name of the category you'd like to add a product from:  ";
        std::cin >> userInputCategory;

        if (isInputReturnBack(userInputCategory) == true) { break; }

        if (userInputCategory == "sc" || userInputCategory == "SC") {
            while (true) {
                std::cout << std::string(100, '\n');
                std::cout << "Your shopping cart at the moment contains the following products: \n";
                currentShoppingCartVector = 0;

                for (auto currProduct : shoppingCart) {
                    std::cout << currProduct.name << " (" << currProduct.productCode << ")    " << " Quantity: " << shoppingCartQuantities[currentShoppingCartVector];
                    currentShoppingCartVector++;
                }

                std::string userInput;
                currentShoppingCartVector = 0;
                std::cout << "\n\nIf you'd like to checkout this shopping cart, please fill in CHOU.\n";
                std::cout << "If you'd like to delete a product from the shopping cart please fill in either the name or code of the product:   ";
                std::cin >> userInput;


                if (userInput == "CHOU" || userInput == "chou") {
                    float totalCheckoutPrice = 0.00;
                    std::string inputChanges;

                    for (auto currProduct : shoppingCart) {
                        dataCont changedNewProduct = currProduct;
                        changedNewProduct.inStorage--;
                        changedNewProduct.sales++;

                        changeDatabaseProduct(currProduct.category, currProduct, changedNewProduct);

                        totalCheckoutPrice = totalCheckoutPrice + (currProduct.price * shoppingCartQuantities[currentShoppingCartVector]);
                        currentShoppingCartVector++;
                    }
                    std::cout << "\n\nAll changes have been made to the database, the total price of these products is :   $" << totalCheckoutPrice;
                }

                else {
                    for (auto currProduct : shoppingCart) {
                        if (userInput == currProduct.name || userInput == currProduct.productCode) {
                            if (shoppingCartQuantities[currentShoppingCartVector] == 1) {
                                shoppingCart.erase(shoppingCart.begin() + currentShoppingCartVector);
                                shoppingCartQuantities.erase(shoppingCartQuantities.begin() + currentShoppingCartVector);
                                break;
                            }
                            else {
                                shoppingCartQuantities[currentShoppingCartVector]--;
                            }
                        }
                        currentShoppingCartVector++;
                    }
                }
            }
        }

        else {
            std::cout << std::string(100, '\n');
            std::cout << "\n\nPlease fill in either the name or code of the product:   ";
            std::cin >> userInputProduct;
            currentShoppingCartVector = 0;
            bool isNewProduct = true;

            dataCont chosenProduct = returnProductCategory(userInputCategory, userInputProduct);

            for (auto currProduct : shoppingCart) {
                if (currProduct.productCode == chosenProduct.productCode) {
                    shoppingCartQuantities[currentShoppingCartVector]++;
                    isNewProduct = false;
                    break;
                }
                currentShoppingCartVector++;
            }

            if (isNewProduct == true) {
                shoppingCart.push_back(chosenProduct);
                shoppingCartQuantities.push_back(1);
            }
        }
    }
}