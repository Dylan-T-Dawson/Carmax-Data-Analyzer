#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 200 //Maximum length for a line
#define ARRAYSIZE 1250 //Size of each array
#define DATASIZE 200000 //Size of the data.

//The following constants are to reference specific variables from the lines read in from the file. They are passed as arguments to the "getIth" function.
#define price 0
#define appraisal_offer 1
#define online_appraisal_flag 2
#define model_year 3
#define mileage 4
#define make 5
#define model 6
#define trim_descrip 7
#define body 8
#define color 9
#define engine 10
#define cylinders 11
#define mpg_city 12
#define mpg_highway 13
#define horsepower 14
#define fuel_capacity 15
#define model_year_appraisal 16
#define mileage_appraisal 17
#define make_appraisal 18
#define model_appraisal 19
#define trim_descrip_appraisal 20
#define body_appraisal 21
#define color_appraisal 22
#define engine_appraisal 23
#define cylinders_appraisal 24
#define mpg_city_appraisal 25
#define mpg_highway_appraisal 26
#define horsepower_appraisal 27
#define fuel_capacity_appraisal 28
#define market 29

//The conditional statement is used to define the number of arrays needed to hold the data given the size of the dataset and the size of the arrays.
//The modulus is used to determine if there needs to be an additional array to hold some number of elements < ARRAYSIZE. 
#if DATASIZE % ARRAYSIZE == 0
    #define numArrays (DATASIZE / ARRAYSIZE)
#elif
    #define numArrays (DATASIZE / ARRASIZE + 1)
#endif

//The following struct is used to allow for the storing of pointers to arrays of strings in the master array, which is described hereafter. 
typedef struct arrOfStrings{
    char arr[ARRAYSIZE][MAXCHAR];
}arrOfStrings;

//Function to get the ith element from a string of elements separated by commas.
void getIth(char* list, char* retVal, int max, int index);

//Gets the price from a string representing a range of prices "$5k to $10k" would return $5k.
int getPrice(char* range);

//Returns the index associated with the maximum element in a given row from the passed in array
int maxInRow(double arr[], int rows, int columns, int rowNum);

int main() {

    arrOfStrings* masterArray[numArrays]; //Array of pointers to arrays of strings. All data will be referenced through this array once it is filled.

    FILE* fp;
    fp = fopen("ShowcaseDataWinter2023.csv", "r"); //ADD ERROR CHECKING

    //Gets and stores the title, which exceeds the normal MAXCHAR set length.
    //The title is not needed, but the file pointer must be moved passed it.
    char title[MAXCHAR*3];
    fgets(title, MAXCHAR*3, fp);

    //Fills each element in the master array with a pointer to an array of strings, which are filled in turn with lines from the data file.
    for(int w = 0; w < numArrays; w++){    
        masterArray[w] = malloc(sizeof(arrOfStrings)); //masterArray[0] is now a pointer to allocated memory that can have its dereference set equal to the value of an arrOfStrings struct.
        arrOfStrings toSetDef; //Struct that the dereference of an element of masterArray will be set to.
        char buf[MAXCHAR];
        int i = 0;
        //Loop fills an instance of an arrOfStrings
        while(i < ARRAYSIZE && fgets(buf, MAXCHAR, fp)){
            strcpy(toSetDef.arr[i], buf);
            i++;
        }
        *masterArray[w] = toSetDef; //Sets an element of the masterArray's dereference (the memory previously allocated) equal to the value of toSetDef.
    }

    /*Appraisal is from 0 to 40k+ and price is from 0 to 70k+ so the array is 9 * 15 = 135 in dimensions. Some values will not be used. 
    This is because the price variable has a range from $0k to $15k that is not broken down into $5k increments as is the rest of the data.
    A more complex method could possibly be used to save space, but unless this improvement is needed, the current implementation is sufficient. */
    const int APROW = 9; //The AP prefix represent Appraisal VS Price.
    const int APCOLUMN = 15;
    double priceArr[9 * 15]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //In the above, the first row holds the price data for an appraisal of 0-5 etc...
    //The columns hold the appraisal data for prices of 0-5, 5-10, etc...
    
    //Vaues to be used in the following loop.
    int val1;
    int val2;
    //The following loop populates the price array based on the value of the appraisal price and price from each line in the data set, which are stored in the master array.
    for(int i = 0; i < numArrays; i++){
        for(int j = 0; j < ARRAYSIZE; j++){
            char buf1[MAXCHAR];
            char buf2[MAXCHAR];
            getIth(masterArray[i]->arr[j], buf1, MAXCHAR, appraisal_offer); //Gets the appraisal from the current line
            getIth(masterArray[i]->arr[j], buf2, MAXCHAR, price); //Gets the price from the current line
            val1 = getPrice(buf1); //Converts appraisal string to int
            val2 = getPrice(buf2); //Converts price string to int.
            //The following throws an error if the value of either the current appraisal offer or price is not within the range specified in the data.
            if(val1 > 70 || val1 < 0 || val2 > 70 || val2 < 0){
                printf("\nError\n The line is %s", masterArray[i]->arr[j]);
                printf("The values are: val1: %d, val2: %d\n", val1, val2);
                printf("The strings are buf1: %s, buf2: %s\n", buf1, buf2);
                printf("The value of getPrice is val1: %d, val2: %d\n", getPrice(buf1), getPrice(buf2));
                exit(1);
            }
            //Adds one to the correct index in the price array.
            priceArr[(val2/5) + (val1/5) * APCOLUMN]++;

        }
    }
    //Upon completion of the above loop, the priceArr is populated and each element reflects the total number of lines matching the respective criteria.
    //For example, index 3 holds the total number of lines with an appraisal of $0k to $5k and a price of $15k to $20k.
    
    //We will now transform the array to hold percentages. Each row should total to 1.
    double rowTot;
    for(int i = 0; i < APROW; i++){
        rowTot = 0;
        for(int j = 0; j < APCOLUMN; j++){
            rowTot += priceArr[i*APCOLUMN + j];
        }
        for(int j = 0; j < APCOLUMN; j++){
            priceArr[i*APCOLUMN + j] = priceArr[i*APCOLUMN+j] / rowTot;
        }
    }
    //Now, each element in the priceArr holds the percentage of a respective price for a given appraisal.
    //For example, if there were 100 appraisals of $15k to $20k and 10 of those had a price of $40k to $45k, then the element in the 4th row, 9th column would be 0.10.

    
    //The following will NOT be used in the final version of this program, however, it is used to see how accurate the price can be predicted
    //using the appraisal price.
    int valO;
    int valT;
    int rowNum;
    int maxI;
    int priceGuess;
    int numCorrect = 0;
    //Loops through each element of the master array and compares the most likely price based on the appraisal price using the priceArr to
    //the actual purchase price of each car.
     for(int i = 0; i < numArrays; i++){
        for(int j = 0; j < ARRAYSIZE; j++){
            char buf1[MAXCHAR];
            char buf2[MAXCHAR];
            getIth(masterArray[i]->arr[j], buf1, MAXCHAR, appraisal_offer);
            getIth(masterArray[i]->arr[j], buf2, MAXCHAR, price);
            valO = getPrice(buf1);
            valT = getPrice(buf2);
            rowNum = valO/5;
            maxI = maxInRow(priceArr, APROW, APCOLUMN, rowNum);
            priceGuess = ((maxI % APCOLUMN)) * 5;
            if(priceGuess == valT){
                numCorrect++;
            }
        }
     }

    printf("Predicted price correctly %f%% of the time.", (double)numCorrect/DATASIZE * 100);
    //The algorithm has achieved 25% accuracy at predicting the price of the vehicle.
   
    //The next step is to continue this for all aspects of the trade in, using each one to predict the price of the purchased vehicle.

    fclose(fp);
    return 0;
}

void getIth(char* list, char* retVal, int max, int index){
    int cur = 0;
    //Passes through index number of commas in the line.
    while(index != 0){
        if(list[cur] == ','){
            index--;
        }
        cur++;
    }
   
   //Stores all characters from the current comma to either the next comma or the end of the string into the return string.
    int i = 0;
    while(list[cur] != ',' && list[cur] != '\0' && cur < max){
        retVal[i] = list[cur];
        i++;
        cur++;
    }
    retVal[i] = '\0';
    return;
}

int getPrice(char* range){
    char priceStr[3];
    //Setting index to 1 skips the dollar sign character.
    int index = 1;
    //The loop and subsequent line store the numbers between the $ and first 'k'in priceStr
    while(index < 2 && range[index] != 'k'){
        priceStr[index-1] = range[index];
        index++;
    }
    priceStr[index-1] = range[index];
    index = atoi(priceStr); //Converts priceStr to an integer value.
    return index;
}

int maxInRow(double arr[], int rows, int columns, int rowNum){
    double max = 0;
    int index;
    //Iterates through each element in a given row from the passed in array and stores the index of the maximum element.
    for(int i = 0; i < columns; i++){
        if(arr[rowNum * columns + i] > max){
            max = arr[rowNum * columns + i];
            index = rowNum*columns + i;
        }
    }
    return index;
}