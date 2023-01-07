# Carmax-Data-Analyzer
Author: Dylan Dawson

The purpose of this project is to use the data from the given file to create an algorithm that can, as accurately as possible, predict aspects of the car that a customer will purchase based upon the car that they had appraised.


The first step in the process is to read in each line of the dataset to a data structure for easy access. The main data structure is an array of pointers to structs. Each struct contains only an array of strings. The dataset is too large to be stored into a single array, so the previously described process is necessary to allow all the data to be accessed through a single data structure.

The next step is to create arrays to store the relations between each variable and each other variable. To give an example, the first relation to be calculated is between appraisal price and purchase price. To determine this, an array is first created to store the number of lines of the dataset that contain each possible combination of appraisal price and purchase price. There are 9 possible ranges for appraisal price and 15 possible ranges for purchase price, so the array will be 9x15=135 elements in size. The array can be visualized as having 9 rows and 15 columns. The first element in the array will store the number of lines in the dataset that have an appraisal price range with a lower bound of zero and a purchase price with a lower bound of zero. The second element in the array will store the number of lines in the dataset that have an appraisal price range with a lower bound of zero and a purchase price with a lower bound of $5k. The final element in the first row (which contains 15 elements) will store the number of lines in the dataset that have an appraisal price range with a lower bound of zero and a purchase price of $70k+ (as this is the highest range in the dataset). After this element, the second row begins with the element storing the total number of lines with an appraisal price with a lower bound of $5k and a purchase price with a lower bound of zero. The array continues in this pattern with the final element storing the number of lines with an appraisal price of $40k+ and a purchase price of $70k+. This array will be filled by iterating through each line from the dataset stored in the data structure described in the first step and incrementing the element in the array that corresponds to the appraisal price and purchase price in each respective line.

Once the array is filled, it can then be converted to a weighted array. The new values for the array will represent the probability that, for a given appraisal aspect, an aspect of the purchased car will hold a specific value. To continue to build upon the previous example, we would first find the sum of each row. Then, for each element in each row, replace its value by its value divided by the sum of the row. This process would be continued for all elements in the array. Upon completion, the first element in the array would hold the probability that, given an appraisal price with a lower bound of $0k, the purchased vehicle also had a lower bound of $0k.

The above-described processes will then be repeated for each possible variable combination. However, seeing as I am currently working on this project alone and there are 224 possible combinations of variables, it is unlikely that I will be able to carry out the processes for every combination. I will attempt to create as much reusable code as possible, which may allow me to succeed in creating all 224 arrays that would theoretically be needed, but because many of the variables are unique in the way that they must be handled, this may prove difficult. For example, some variables are not divided into ranges, such as horsepower, so I will have to divide them into ranges within this program for this method to work. In addition, some variables are qualitative rather than quantitative. For example, when working with the model of the car, I will have to determine every possible model found in the spreadsheet and allocate a column for each of these, which could result in large arrays. I believe that this could be alleviated if, rather than attempting to predict the model, I could instead attempt to predict the vehicle category (truck, sedan, SUV, etc...), but unfortunately, this data was not included in the dataset. The current next step in the project is to complete the steps described in this paragraph.

Following the completion of the above, a prediction algorithm will be created that will attempt to guess all aspects of each customer in the dataset's purchased vehicle based upon all the aspects of their traded in vehicle. The algorithm will work through each purchased vehicle variable taking an average of the predictions for that variable based on all the appraisal variables and finding the maximum, which will be the algorithm's guess for that purchased vehicle variable. For example, if the price was the first variable to be predicted by the algorithm, then it would first calculate the odds for each possible price range based on the appraisal price. Perhaps the appraisal price was $5-$10k and from that there is a 10% chance the purchase price is $15k to $20k, a 40% chance the price is $20k-$25k, and a 50% chance the price is $25k-$30k (all other ranges are zero for simplicity in this example) based on the weighted array for the relation between appraisal price and price. Then, the appraisal mileage may be 100k-110k. From the mileage vs. price weighted array, it might be found that there is a %5 chance the price is $15k-$20k, a 60% chance that the price is $20k-$25k, and a 35% chance the price is $25k-$30k. The algorithm would take this data and find that the average chance for the price being $15k-$20k is 7.5%, the average chance for the price being $20k-$25k is 50%, and the average chance for the price being $25k-$30k is 42.5%. Therefore, the algorithm would guess that the price of the purchased car is $20k-$25k because it had the highest average percentage.

Once the above steps have been completed, the last step in constructing the algorithm is to vary the weight of different relations. For example, to predict the price, it is likely that the appraisal price will a much more consistent relation with the purchase price than would be the relation between the color of the appraisal car and the purchase price. Therefore, when the average of all the percentages is taken, some variables should be weighted more heavily than others. To do this, the algorithm will iterate through all of the data and determine how many times it accurately predicts each variable when all of the relationships between variables are considered evenly weighted. Then, the algorithm will run repeatedly, changing the weights given to the variables. For each variable that needs to be predicted, the algorithm will have the weight of one relationship increased by 10% and the others decreased respectively. The algorithm iterates through all possible combinations of percentages to determine the best possible algorithm for predicting each aspect of the purchased vehicle. On a more powerful machine, the algorithm could be further fine-tuned by changing percentages by smaller amounts than 10% at a time, but that is not possible on my computer at this time.

Upon the completion of the program, the best weights for the algorithm will be outputted to the console and can later be hardcoded.

*NOTES* 

A build task was configured to automatically compile and run the program in VS code using MINGW.

The market variable can be treated as part of the appraisal and used to predict the value of the trade in. The same can be done with whether the appraisal was done online.
In addition, the algorithm can be set to run on only in-person or online appraisals to determine if it is more accurate on one than the other.
