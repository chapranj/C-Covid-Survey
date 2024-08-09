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
            printList(*top);
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
    //TODO 10 implement townMapping function
    return townsArr[x];
}// ends townMapping function
/**
 *
 * @param x
 * @return
 */
char* raceMapping(int x){
    //TODO 11 implement raceMapping function
    return racesArr[x];
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
    //TODO 12 implement townMappingRegionBased function. Hint! Take lead from regionMapping function
    switch (region) {
        case 0:
            return townsArr[x];
        case 1:
            return townsArr[x+2];
        case 2:
            return townsArr[x+4];
        default:
            break;
    }
    return 0;
}// ends townMappingRegionBased function
/**
 * It populates the linked list with valid random data. The top of the list is passed as a reference i.e. address of the pointer!
 * @param top top is passed by reference i.e. address of the pointer top is passed in the call!
 */
void initializeData(NodePtr* top) {

    // This function populates the linked list with random data.
    // TODO 01: Implement initializeData function.

    *top =  malloc(sizeof (**top));
    (*top)->next = NULL;

    strcpy((*top)->data.race, racesArr[rand()%5]);
    int randomRegionSelected = rand()%3;
    strcpy((*top)->data.region, regionsArr[randomRegionSelected]);
    int townBaseRandomForm = rand()%2;
    strcpy((*top)->data.town, townMappingRegionBased(randomRegionSelected,townBaseRandomForm));
    int randFamilySize = (rand()%MAX_FAMILYSIZE)+1;
    (*top)->data.familySize = randFamilySize;
    (*top)->data.fullyVaccinated = (rand() % randFamilySize + 1);
    (*top)->data.testedPositive = (rand() % randFamilySize + 1);

    for (int i = 0; i < SIZE-1; ++i) {
        Household houseHold;
        strcpy(houseHold.race, racesArr[rand() % 5]);
        int randomRegionSelected = rand() % 3;
        strcpy(houseHold.region, regionsArr[randomRegionSelected]);
        int townBaseRandomForm = rand() % 2;
        strcpy(houseHold.town, townMappingRegionBased(randomRegionSelected,townBaseRandomForm));
        int randFamilySize = (rand()%MAX_FAMILYSIZE)+1;
        houseHold.familySize = randFamilySize;
        houseHold.fullyVaccinated =(rand() % randFamilySize+1);
        houseHold.testedPositive = (rand() % randFamilySize+1);
        addNode(top, houseHold);
    }
} //initializeData ends
/**
 *
 * @param top is the top of the list
 * @param region is the region that all the displayed records should belong to
 */
void displayRecordsOfOneRegion(NodePtr top, char region[]) {
    // TODO 02: implement displayRecordsOfOneRegion function.
    int snoCounter = 1;
    printf(FORMAT_HEADER, "S.No", "Size", "Vaccinated", "Positive", "Race", "Region", "Town");
    while(top!=NULL){
        if (!strcmp(top->data.region, region)) {
            printf(FORMAT_DATA, snoCounter++, top->data.familySize, top->data.fullyVaccinated, top->data.testedPositive,
                   top->data.race, top->data.region, top->data.town);
        }
        top = top->next;
    }
    puts("");
} //ends displayRecordsOfOneRegion
/**
 *
 * @param top
 * @param town
 */
void displayRecordsOfOneTown(NodePtr top, char town[]) {
    // TODO 03: implement displayRecordsOfOneTown function
    int snoCounter = 1;
    printf(FORMAT_HEADER, "S.No", "Size", "Vaccinated", "Positive", "Race", "Region", "Town");
    while(top!=NULL){
        if (!strcmp(top->data.town, town)) {
            printf(FORMAT_DATA, snoCounter++, top->data.familySize, top->data.fullyVaccinated, top->data.testedPositive,
                   top->data.race, top->data.region, top->data.town);
        }
        top = top->next;
    }
    puts("");
} //ends displayRecordsOfOneTown
/**
 *
 * @param top
 * @param race
 */
void displayRecordsOfOneRace(NodePtr top, char race[]) {
    // TODO 04: implement displayRecordsOfOneRace function
    int snoCounter = 1;
    printf(FORMAT_HEADER, "S.No", "Size", "Vaccinated", "Positive", "Race", "Region", "Town");
    while(top!=NULL){
        if (!strcmp(top->data.race, race)) {
            printf(FORMAT_DATA, snoCounter++, top->data.familySize, top->data.fullyVaccinated, top->data.testedPositive,
                   top->data.race, top->data.region, top->data.town);
        }
        top = top->next;
    }
    puts("");
} //ends displayRecordsOfOneTown
/**
 *
 * @param top
 * @param region
 * @param numOfPositiveCases
 */
void displayRecordsOfRegionWithPositiveCases(NodePtr top, char region[], int numOfPositiveCases){
    // TODO 05: implement displayRecordsOfRegionWithPositiveCases function
    printf(FORMAT_HEADER, "S.No", "Size", "Vaccinated", "Positive", "Race", "Region", "Town");
    int ctr = 1;
    while(top!=NULL){
        if((top->data.testedPositive >= numOfPositiveCases) && (!strcmp(top->data.region, region))){
            printRecord(ctr++, top);
        }
        top=top->next;
    }
    puts("");
}
/**
 * This function ranks all the towns in descending order of total number of people vaccinated
 * @param top
 */
void regionsTownWiseRankingVaccinated(NodePtr top){
    // TODO 06: implement regionsTownWiseRankingVaccinated function
    LocationCountPair lC[6];
    LocationCountPair lCRegion[6];
    for (int i = 0; i < ARR_TOWN_LEN; ++i) {
        strcpy(lC[i].town, townsArr[i]);
        lC[i].count = 0;
    }
    for (int i = 0; i < ARR_REGION_LEN; ++i) {
        strcpy(lCRegion[i].town, regionsArr[i]);
        lCRegion[i].count = 0;
    }

    while(top!=NULL){
        for (int i = 0; i < ARR_TOWN_LEN; ++i) {
            if ((!strcmp(top->data.town, lC[i].town))){
                lC[i].count+=top->data.fullyVaccinated;
                break;
            }
        }
        for (int i = 0; i < ARR_REGION_LEN; ++i) {
            if ((!strcmp(top->data.region, lCRegion[i].town))){ //.town for lCRegion is basically .region here
                lCRegion[i].count+=top->data.fullyVaccinated;
                break;
            }
        }
        top = top->next;
    }

    sortSelection(lC, ARR_TOWN_LEN);
    sortSelection(lCRegion, ARR_REGION_LEN);

    puts("Total Vaccinated: \nRegion-wise ranking:\n");
    for (int i = 0; i < ARR_REGION_LEN; ++i) {
        printf("\t\t%s : %d\n", lCRegion[i].town, lCRegion[i].count);
    }
    puts("Town-wise ranking:\n");
    for (int i = 0; i < ARR_TOWN_LEN; ++i) {
        printf("\t\t%s : %d\n", lC[i].town, lC[i].count);
    }
} // function regionsTownWiseRankingVaccinated ends

/**
 * This function gets validated data from the user for adding a record and then invokes makeHousehold function as part
 * of the return statement
 * @return  it returns a household instance that this function creates based on validated user input
 */
Household userInteractionForRecordAddition(){
    // TODO 07: implement userInteractionForRecordAddition function
    char temp[25];
    int regionOption, townOption, raceOption, totalPeople, fullyVac, covPos;
    while(1) {
        puts("Enter region: 0 for Durham, 1 for Peel, 2 for York");
        fgets(temp, sizeof(temp), stdin);
        int n = sscanf(temp, "%d", &regionOption);
        if (n != 1 || regionOption > 2 || regionOption < 0) {
            puts("Please enter valid option. Try again!");
            continue;
        }
        break;
    }
    while(1) {
        printf("Enter town : 0 for %s, 1 for %s:\n", townMappingRegionBased(regionOption, 0), townMappingRegionBased
                (regionOption, 1));
        fgets(temp, sizeof(temp), stdin);
        int n = sscanf(temp, "%d", &townOption);
        if (n != 1 || townOption > 1 || townOption < 0) {
            puts("Please enter valid option. Try again!");
            continue;
        }
        break;
    }
    while(1) {
        puts("Enter race");
        puts("Enter 0 for Caucasian, 1 for indigenous, 2 for African_American, 3 for Asian, 4 for Other");
        fgets(temp, sizeof(temp), stdin);
        int n = sscanf(temp, "%d", &raceOption);
        if (n != 1 || raceOption > 4 || raceOption < 0) {
            puts("Please enter valid option. Try again!");
            continue;
        }
        break;
    }
    puts("Please enter\n1-total people in the household,\n2-people fully vaccinated,\n3-people tested Covid "
         "positive\n separated by space/tab and make sure all of these are valid integers\n");
    while(1){
        fgets(temp, sizeof (temp), stdin);
        int n = sscanf(temp, "%d%d%d", &totalPeople, &fullyVac, &covPos);
        if((n!=3) || ((totalPeople<covPos)||(totalPeople <fullyVac))){
            puts("Total people in the household can't be less than total vaccinated or total Covid positive cases. "
                 "Please re-enter values:");
            continue;
        }
        break;
    }
    return makeHousehold(raceMapping(raceOption), regionMapping(regionOption), townMappingRegionBased(regionOption,
                                                                                                townOption),
                  totalPeople, covPos, fullyVac);
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
    int counter = 0;
    while((*top)!=NULL &&
    (strcmp((*top)->data.region, region)==0 &&
    (strcmp((*top)->data.race, race)==0 &&
    (strcmp((*top)->data.town, town)==0)))){
        NodePtr temp = *top;
        *top = (*top)->next;
        free(temp);
        counter++;
    }

    NodePtr curr = *top;
    NodePtr prev = curr;

    while(curr!=NULL){
        while(curr!=NULL &&
        ((strcmp((curr)->data.region,region)!=0) ||
        (strcmp((curr)->data.town, town)!=0 ||
        (strcmp((curr)->data.race,race)!=0)))){
            prev = curr;
            curr = curr->next;
        }

        if(curr==NULL){
            break;
        }
        prev->next = curr->next;
        free(curr);
        counter++;
        curr=prev->next;
    }

    printf("%d matching records deleted..\n", counter);

}// deleteNodeCriteria function ends
/**
 * This function prints the entire list of data. It invokes printRecord function
 * @param ptr is the top of the list
*/
void printList(NodePtr ptr) {
    // TODO 09: implement printList function
    //Hint! Use FORMAT_HEADER format specifier string constant declared in the header file
    int snoCounter = 1;
    printf(FORMAT_HEADER, "S.No", "Size", "Vaccinated", "Positive", "Race", "Region", "Town");
    while(ptr!=NULL){
        printRecord(snoCounter++, ptr);
        ptr = ptr->next;
    }
    puts("");
    //function printRecord is invoked
}
/**
 * It prints a single record starting with a serial number to keep a count of number of records printed
 * @param ctr serial number of the record
 * @param ptr top of the list
 */
void printRecord(int ctr, NodePtr ptr) {
    // TODO 13: implement printRecord function
    // Hint! use FORMAT_DATA format specifier string constant declared in the header file
    printf(FORMAT_DATA, ctr, ptr->data.familySize, ptr->data.fullyVaccinated, ptr->data.testedPositive, ptr->data
            .race, ptr->data.region, ptr->data.town);
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
    Household newHouse;
    strcpy(newHouse.race, race);
    strcpy(newHouse.region, region);
    strcpy(newHouse.town, town);
    newHouse.familySize = familySize;
    newHouse.testedPositive = totPosCovid;
    newHouse.fullyVaccinated = fullyVac;
    return newHouse;

}

/**
 * makeNode function allocates dynamic memory to create a node, populates with the data based on its argument of type Household
 * and returns the populated node
 * @param num
 * @return
 */
NodePtr makeNode(Household num){
    // TODO 15: implement makeNode function
    Node* n = malloc(sizeof (Node));
    n->data = num;
    n->next = NULL;
    return n;
}

/**
 * Add node takes a Household instance, creates a node from it and then adds it to the front of the list that it takes as
 * its other argument
 * @param tom
 * @param num
 */
void addNode(NodePtr* top, Household num){
    // TODO 16: implement addNode function
    Node* newNode = makeNode(num);
    newNode->data = num;
    newNode->next = *top;
    (*top) = newNode;
}
/**
 * THis function deletes a node from the list
 * @param tom takes top of the list as a reference
 */
void deleteNode(NodePtr* tom){
    // TODO 17: implement deleteNode function
    NodePtr temp = *tom;
    *tom = (*tom)->next;
    free(temp);
}
/**
 * This function deletes all nodes (records) of the list
 * @param tom
 */
void deleteAllNodes(NodePtr* tom){
    // TODO 18: implement deleteAllNodes function
    while(*tom!=NULL){
        deleteNode(tom);
    }
}
/**
 * It write all the records to a file. As a sample, clients.txt file is saved in the data folder as part of the project folder
 * @param top
 * @param fileName
 */
void writeListToFile(NodePtr top, char fileName[]){
    // TODO 19: implement writeListToFile function
    FILE *cfPtr;
    if ((cfPtr = fopen(fileName, "w")) == NULL){
        printf("File could not be opened\n");
    }
    else{
        while(top!=NULL){
            fprintf(cfPtr ,"%s %s %s %d %d %d\n",  top->data.race, top->data.region, top->data.town, top->data
            .familySize, top->data.fullyVaccinated, top->data.testedPositive);
            top=top->next;
        }
        fprintf(cfPtr, "%s %s %s %d %d %d", "Last", "Record", "None", 0,0,0 );
        fclose(cfPtr);
    }
    printf("%d records written successfully as the text file: %s\n", SIZE, fileName);
    displayRecordsFromFile();
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

    char race[RACE_NAME_LEN];
    char region[REGION_NAME_LEN];
    char town[TOWN_NAME_LEN];
    int familySize;
    int fullyVaccinated;
    int testedPositive;
    int ctr = 1;
//    Household h;
    if (fopen(filename, "r") == NULL) {
        printf("File could not be opened!\n");
        exit(1);
    }
    else{
        while(!feof(cfPtr)){
            Household h;
            fscanf(cfPtr,  "%s %s %s %d %d %d", race, region, town, &familySize, &fullyVaccinated, &testedPositive );
            if(!strcmp(race, "Last")){
                break;
            }
            Household houseRead = makeHousehold(race, region, town, familySize, fullyVaccinated, testedPositive);
            NodePtr houseReadPtr = makeNode(houseRead);
            printRecord(ctr++, houseReadPtr );
            free(houseReadPtr);
        }
        fclose(cfPtr);
    }
    // TODO 20: finish displayRecordsFromFile function by adding code to read data from the file selected above
}