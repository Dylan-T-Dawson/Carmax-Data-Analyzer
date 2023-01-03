//The purpose of this project is to create an algorithm that can accurately predict certain aspects of the car
//a customer will purchase based on the aspects of the car that they have traded in.
//The first step in this process is to determine the consistency of each variable. ("How often do customers purchase a car of the same model?")
//This will give us an idea of the areas on which to focus (No need to try to predict a variable that is not consistent in any way.
//We will compare each variable in the traded in car to each variable in the purchased car to see if there is any relation. (Price to model, price to cylinder, price to etc... )
//We will then use the best found relations for each variable in the purchased car to attempt to predict the purchased car's attributes.
//Next, we will attempt to use varying numbers of the most correlated variables to predict the purchased car's attribtutes ("Do price and model of the trade in correlate to the price of the bought more than just price?")
//Finally, we will create an AI that varies the weight placed on each of the variables and uses the previous information as a starting point. (If there is no correlation between model of trade in and cylinders of purchased, then the weight of model in predicting cylinders would be zero).
//The AI will vary these weights in as many ways as feasible on my laptop in a reasonable amount of time until it determines the best possible combination of weights for each variable in prediciting each variables to produce the highest percentage of correct results based on the given dataset.
//These weights will then be hardcoded into an algorithm capable of creating the most likely purchased car based on the traded in car.
//This algorithm will be the deliverable of the project. From here we can advertise to customers based primarily on their traded in car and give them the best possible suggestion. This will help in securing purchases after a trade in. 
//One problem may be ensuring that the algorithm does not create impossible cars ("Lambo with 4 cylinders and a $2 price). This should not happen actually because customers won't be buying impossible cars. Even if this were to happen, real cars with the most matching aspects could be promoted.
//gcc CarmaxParser.c -o CarmaxParser
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 200 //Maximum length for a line
#define ARRAYSIZE 1250 //Size of each array
#define DATASIZE 200000 //Size of the data.

//The following constants are to reference specific variables from the lines read in from the file.
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
    //The title is not needed, but the file pointer must be moved passe it.
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



    //Now, the goal is to use each variable to predict the value of all the other variables. To do this for non-numeric or range variables, we will hold the value of the 
    //variable in question constant at each possible value and determine the number of vehicles with this value of the variable in question with each possible variation of the
    //response variable. For example, we will hold the appraise price constant at each interval respectively while iterating through the entire list. We will determine the number of vehicles
    //matching the current appraised price whose model is x, y, z, etc... and whose cylinders are x, y, z, and whose ... Then, we will generate from this a model which will allow us to use the price
    //to predict each of the other variables. For example, if there are 10 cars with a price appraisal between 5 and 10k, and of those 10, 3 lead to a x body, 6 and y body, and 1 a z body, the price 
    //appraisal will generate a prediction that it is 30% x, 60% y, and 10% z. Then say that from the body we get an estimate that is 20% x, 60% z, and 20% y, we find the average of these values and 
    //which ever variable has the highest percentage is our prediction. Once we have this model generated for all variables, we can then vary the weight on each variable and determine which variables
    //are the best predictors of what. For example, maybe the trade-in trim is useless at predicting the number of cylinders of the purchased car, so it gets a weight of zero when calculating that variables,
    //but perhaps it is an excellent predictor of the purchased car's trim, then it might have a weight of .25. For now, we will ignore the market and the online appraisal field.

    //We will first test using just the appraisal price to predict the purchase price.
    //Step 1 is to stratify all of the data into arrays.

    /*Appraisal is from 0 to 40k+ and price is from 0 to 70k plus so the array is 9 * 15 = 135 in dimensions. Some values will not be used. 
    This is because the price variable has a range from $0k to $15k that is not broken down into $5k increments as is the rest of the data.
    A more complex method could possibly be used to save space, but unless this improvement is needed, the current implementation is sufficient. */
    const int APROW = 9; //AP stands for Appraisal VS Price
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
    //The columns the appraisal data for prices of 0-5, 5-10, etc...
    
    //Vaues to be used in the following loop.
    int val1;
    int val2;
    //The following loop populates the price array based on the value of a the appraisal price and price from each line in the data set, which are stored in the master array.
    for(int i = 0; i < numArrays; i++){
        for(int j = 0; j < ARRAYSIZE; j++){
            char buf1[MAXCHAR];
            char buf2[MAXCHAR];
            getIth(masterArray[i]->arr[j], buf1, MAXCHAR, appraisal_offer);
            getIth(masterArray[i]->arr[j], buf2, MAXCHAR, price);
            val1 = getPrice(buf1);
            val2 = getPrice(buf2);
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

    //Now lets write a practice predictor
    int valO;
    int valT;
    int rowNum;
    int maxI;
    int priceGuess;
    int numCorrect = 0;
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
            priceGuess = ((maxI % APCOLUMN)) * 5; //wrong
            //printf("(%d, %d) ", priceGuess, maxI);
            if(priceGuess == valT){
                numCorrect++;
            }
            //printf("NumCorrect: %d, PriceGuess: %d, Val2: %d,\n", numCorrect, priceGuess, valT);
        }
     }

     printf("Predicted price correctly %f%% of the time.", (double)numCorrect/DATASIZE * 100);
     //We have achieved 25% accuracy at predicting the price of the vehicle.
   
     //The next step is to continue this for all parts of the trade in, using each one to predict the price of the purchased vehicle.
     //We can act as if market is part of the appraisal and use it to predict the value of the trade in. The same can be done with whether or not the appraisal was done online.
     //In addition, we can set our algorithm to online run on in person or online appraisal and see which is more accurate.

    //Next step is to clean this up and write a readme and publish to GitHub.
    fclose(fp);
    return 0;
}

void getIth(char* list, char* retVal, int max, int index){
    int cur = 0;
    while(index != 0){
        if(list[cur] == ','){
            index--;
        }
        cur++;
    }
   
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
    int index = 1;
    while(index < 2 && range[index] != 'k'){
        priceStr[index-1] = range[index];
        index++;
    }
    priceStr[index-1] = range[index];
    index = atoi(priceStr);
    return index;
}

int maxInRow(double arr[], int rows, int columns, int rowNum){
    double max = 0;
    int index;
    for(int i = 0; i < columns; i++){
        if(arr[rowNum * columns + i] > max){
            max = arr[rowNum * columns + i];
            index = rowNum*columns + i;
        }
    }
    return index;
}