/*
 * This file gives full implementation of a few functions and gives headers for the rest of the functions that you
 * are required to implement.
 * You may also find all of these functions as part of the TODO list
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include "household.h"
// sorting function required for ranking of data
void sortSelection(LocationCountPair arr[], int arrSize){
    int min = 0, temp = 0;
    char tempStr[20];

    for (int i = 0; i < arrSize ; i++)
    {
        min = i;  // record the position of the smallest
        for (int j = i + 1; j < arrSize; j++)
        {
            // update min when finding a smaller element
            if (arr[j].count > arr[min].count)
                min = j;
        }
        // put the smallest element at position i
        temp = arr[i].count;
        arr[i].count = arr[min].count;
        arr[min].count = temp;
        strcpy(tempStr, arr[i].town);
        strcpy(arr[i].town, arr[min].town);
        strcpy(arr[min].town, tempStr);
    }
}
/**
 * This is a generic validation function that takes the upper bound of valid options up to upperbound and returns exitOption
 * if the user opts to go back to the previous menu instead of providing valid data. Therefore exitOption should not be a
 * valid choice!!
 */
 /**
  *
  * @param upperbound is the largest integer value that is a valid choice
  * @param exitOption is the value user enters to exit the menu
  * @return
  */
int dataValidation(int upperbound, int exitOption) {
        int n = 0, num;
        char temp[40];
        while (1)
        {
            fgets(temp, sizeof(temp), stdin);
            n = sscanf(temp, "%d", &num);
            if (num == exitOption) return exitOption;
            else if(num >= 0 && num <= upperbound && n == 1) return num;
            else
            {
                printf("Invalid data, Enter an integer 1 through %d or enter %d to exit the program."
                       " Try again \n",upperbound, exitOption );
                continue;
            }
        }
    }

// full implementation of regionMapping function is given as a sample to help write other matching functions, if required.
/**
 *
 * @param x Takes an integer representing a region
 * @return is the corresponding region's name
 */
char* regionMapping(int x)
{
   return regionsArr[x];
}// ends regionMapping function
/**
 * Full implementation of the menu function is provided that implements entire main user interface of the application.
 * Students are required to implement various functions called in this menu.
 * A list of ToDos is also given for easier development
 * @param top of the list to be provided by reference.
 */
void menu(NodePtr* top)
{
    int optionTopLevel = 0;
    while (1)
    {
        // Display a menu to the user
        char temp[120];
        char option[120];
        int valid = 0;
        puts("Menu:");
        printf("Enter your choice to\n");
        printf("1. display households of a race\n");
        printf("2. display households of a region\n");
        printf("3. display households of a town\n");
        printf("4. display households of a region with a given minimum number of people tested positive for Covid-19\n");
        printf("5. display the regions town-wise ranking of number of people fully vaccinated positive for Covid-19\n");
        printf("6. add a record\n");
        puts("7. delete all records of a region, town and race triplet");
        puts("8. display updated data");
        puts("9. store data to a file");
        puts("10. display data from file");
        puts("0. to exit the program");
        scanf("%d", &optionTopLevel);
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { } // input stream flushing

        if (optionTopLevel == 0)
        {
            printf("\nThank you");
            return;
        }
        else if (optionTopLevel > 10)
        {
            printf("Invalid selection, enter an integer 0 through 10, try again\n");
            continue;
        }
        int regionOption = 0, townOption = 0, raceOption = 0, numberTested, numberTestedPos;
        char filename[120] = "..//data//";
        char strTemp[120];
        switch( optionTopLevel) {
            case 1:
                puts("Enter an integer for race: CAUCASIAN (0), INDIGENOUS(1), AFRO AMERICAN(2), ASIAN(3), and OTHER(4)");
                if((raceOption= dataValidation(4, 9)) == 9 ) break;
                displayRecordsOfOneRace(*top, raceMapping(raceOption));
                break;
            case 2:
                puts("Enter an integer for region: Durham (0), Peel(1), and York(2):");
                if((regionOption= dataValidation(2, 9)) == 9 ) break;
                displayRecordsOfOneRegion(*top, regionMapping(regionOption));
                break;
            case 3:
                puts("Enter an integer for town: OSHAWA(0), WHITBY(1), BRAMPTON(2), MISSISSAUGA(3), MAPLE(4) and VAUGHAN(5)");
                if((townOption= dataValidation(5, 9)) == 9 ) break;
                displayRecordsOfOneTown(*top, townMapping(townOption));
                break;
            case 4:
                puts("Enter an integer for region: Durham (0), Peel(1), and York(2):");
                if((regionOption= dataValidation(2, 9)) == 9 ) break;
                while(valid != 1) {
                    puts("Enter lowerbound of number of Covid-19 positive cases per household in the region\n");
                    fgets(temp, sizeof(temp), stdin);
                    sscanf(temp, "%d", &numberTestedPos);
                    if (numberTestedPos <= 0) {
                        puts("Invalid data, enter a positive integer, try again");
                        continue;
                    }
                    else valid = 1; // setting valid flag
                }
                valid = 0; // resetting valid flag for next iteration
                displayRecordsOfRegionWithPositiveCases(*top, regionMapping(regionOption), numberTestedPos);
                break;
            case 5:
                regionsTownWiseRankingVaccinated(*top);
                break;
            case 6:
                addNode(top, userInteractionForRecordAddition());
                break;
            case 7:
                puts("Enter region: 0 for Durham, 1 for Peel, 2 for York");
                fgets(temp, sizeof(temp), stdin);
                sscanf(temp, "%d", &regionOption);
                if(regionOption == 0){
                    puts("Enter town: 0 for Oshawa, 1 for Whitby");
                    fgets(temp, sizeof(temp), stdin);
                    sscanf(temp, "%d", &townOption);
                }
                else if(regionOption == 1){
                    puts("Enter town: 0 for Brampton, 1 for Mississauga");
                    fgets(temp, sizeof(temp), stdin);
                    sscanf(temp, "%d", &townOption);
                }
                else {
                    puts("Enter town: 0 for Maple, 1 for Vaughan");
                    fgets(temp, sizeof(temp), stdin);
                    sscanf(temp, "%d", &townOption);
                }
                puts("Enter race");
                puts("Enter 0 for Caucasian, 1 for indigenous, 2 for African_American, 3 for Asian, 4 for Other");
                fgets(temp, sizeof(temp), stdin);
                sscanf(temp, "%d", &raceOption);
                deleteNodesGivenCriteria(top, regionMapping(regionOption), townMappingRegionBased(regionOption, townOption),
                                         raceMapping(raceOption));
                break;
            case 8:
                printList(*top);
                break;
            case 9:
                puts("Enter file name with extension, for example clients.txt");
                fgets(temp, sizeof(temp), stdin);
                sscanf(temp, "%s", strTemp);
                writeListToFile(*top, strcat(filename, strTemp));
                break;
            case 10:
                displayRecordsFromFile();
                break;
            default:
                puts("Invalid option");
        } // switch block ends here

    } // while loop ends

}// menus function ends
/**
 * This is a simple mapping function, just like regionMapping function
 * @param x is an integer corresponding to the townArr index
 * @return char array representing name of the town
 */
char* townMapping(int x){
    //TODO 11 implement townMapping function
}// ends townMapping function
/**
 *
 * @param x
 * @return
 */
char* raceMapping(int x)
{
    //TODO 12 implement raceMapping function

}// ends raceMapping function

/**
 * This function takes region integer and town integer, town integer actually represents its town number within that region
 * So if there are three towns in a region, town number 0 corresponds to the first town in that region.
 * Read the header file and carefully go through the ordering of elements of regionArr and townArr. regionArr's elements
 * are in alphabetical order, but try to figure out what is the order of townArr elements.
 * @param region an integer value representing a region
 * @param x representing index value from townsArr array (refer to the header file)
 * @return
 */
char* townMappingRegionBased(int region, int x){
    //TODO 10 implement townMappingRegionBased function. Hint! Take lead from regionMapping function


}// ends townMappingRegionBased function
/**
 * It populates the linked list with valid random data. The top of the list is passed as a reference i.e. address of the pointer!
 * @param top top is passed by reference i.e. address of the pointer top is passed in the call!
 */
void initializeData(NodePtr* top) {
    // This function populates the linked list with random data.
    // TODO 01: Implement initializeData function.
} //initializeData ends
/**
 *
 * @param top is the top of the list
 * @param region is the region that all the displayed records should belong to
 */
void displayRecordsOfOneRegion(NodePtr top, char region[]) {
    // TODO 02: implement displayRecordsOfOneRegion function.

} //ends displayRecordsOfOneRegion
/**
 *
 * @param top
 * @param town
 */
void displayRecordsOfOneTown(NodePtr top, char town[]) {
    // TODO 03: implement displayRecordsOfOneTown function

} //ends displayRecordsOfOneTown
/**
 *
 * @param top
 * @param race
 */
void displayRecordsOfOneRace(NodePtr top, char race[]) {
    // TODO 04: implement displayRecordsOfOneRace function

} //ends displayRecordsOfOneTown
/**
 *
 * @param top
 * @param region
 * @param numOfPositiveCases
 */
void displayRecordsOfRegionWithPositiveCases(NodePtr top, char region[], int numOfPositiveCases){
    // TODO 05: implement displayRecordsOfRegionWithPositiveCases function

}
/**
 * This function ranks all the towns in descending order of total number of people vaccinated
 * @param top
 */
void regionsTownWiseRankingVaccinated(NodePtr top){
    // TODO 06: implement regionsTownWiseRankingVaccinated function

} // function regionsTownWiseRankingVaccinated ends
/**
 * This function gets validated data from the user for adding a record and then invokes makeHousehold function as part
 * of the return statement
 * @return  it returns a household instance that this function creates based on validated user input
 */
Household userInteractionForRecordAddition(){
    // TODO 07: implement userInteractionForRecordAddition function
}
/**
 *
 * @param top top of the list to be passed by reference
 * @param region passed as a character array
 * @param town passed as a character array
 * @param race passed as a character array
 */
void deleteNodesGivenCriteria(NodePtr* top, char region[], char town[], char race[]) {
    //TODO 08: implement deleteNodesGivenCriteria function
}// deleteNodeCriteria function ends
/**
 * This function prints the entire list of data. It invokes printRecord function
 * @param ptr is the top of the list
 */
void printList(NodePtr ptr) {
    // TODO 09: implement printList function
    //Hint! Use FORMAT_HEADER format specifier string constant declared in the header file

    //function printRecord is invoked

}
/**
 * It prints a single record starting with a serial number to keep a count of number of records printed
 * @param ctr serial number of the record
 * @param ptr top of the list
 */
void printRecord(int ctr, NodePtr ptr){
    // TODO 13: implement printRecord function
    // Hint! use FORMAT_DATA format specifier string constant declared in the header file

}
/**
 * This function takes data items of a Household record and sets members of a locally declared Household instance and returns it
 * @param race
 * @param region
 * @param town
 * @param familySize
 * @param totPosCovid
 * @param fullyVac
 * @return a Household instance
 */
Household makeHousehold(char race[], char region[], char town[], int familySize, int totPosCovid, int fullyVac ){
    // TODO 14: implement makeHousehold function

}
/**
 * makeNode function allocates dynamic memory to create a node, populates with the data based on its argument of type Household
 * and returns the populated node
 * @param num
 * @return
 */
NodePtr makeNode(Household num){
    // TODO 15: implement makeNode function

}
/**
 * Add node takes a Household instance, creates a node from it and then adds it to the front of the list that it takes as
 * its other argument
 * @param tom
 * @param num
 */
void addNode(NodePtr* top, Household num){
    // TODO 16: implement addNode function
}
/**
 * THis function deletes a node from the list
 * @param tom takes top of the list as a reference
 */
void deleteNode(NodePtr* tom){
    // TODO 17: implement deleteNode function
}
/**
 * This function deletes all nodes (records) of the list
 * @param tom
 */
void deleteAllNodes(NodePtr* tom){
    // TODO 18: implement deleteAllNodes function
}
/**
 * It write all the records to a file. As a sample, clients.txt file is saved in the data folder as part of the project folder
 * @param top
 * @param fileName
 */
void writeListToFile(NodePtr top, char fileName[]){
    // TODO 19: implement writeListToFile function
}
void displayRecordsFromFile() {
    DIR *d;
    struct dirent *dir;
    char path[1000]="..//data";
    d = opendir(path);
    char full_path[1000];
    int fileCtr = 1;
    if (d) while ((dir = readdir(d)) != NULL) fileCtr++;
    char fileNames[fileCtr][80];
    int fileCount = 0;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            full_path[0]='\0';
            strcat(full_path,dir->d_name);
            if(full_path[0] == '.') continue; // skipping dislpay of . and .. in a directory list
            else {
                strcpy(fileNames[fileCount], full_path);
                printf("(%d) %s\n",fileCount++, full_path); // displaying the contents of Data folder
            }

        }
    }
    closedir(d);
    puts("---------------Contents of the Data folder--------------------");
    printf("Enter 0 to %d to access the file or enter %d to go back to the previous menu\n", fileCount - 1, fileCount);
    int fileNum = 0;
    fileNum = dataValidation(fileCount - 1, fileCount);
    if ( fileNum == fileCount) return; // Going back to the main menu
    char filename[120] = "..//data//";
    strcat(filename, fileNames[fileNum]);
    filename[strcspn(filename, "\n")] = '\0'; // this replaces the next-line character in the name of the file by string termination character
    FILE *cfPtr = fopen( filename, "r" );
    // TODO 20: finish displayRecordsFromFile function by adding code to read data from the file selected above
}