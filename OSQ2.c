#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "string.h"
#define MAXLIMIT 30
#define INPUTLIMIT 1000

char str[INPUTLIMIT];//User input
int refPages[MAXLIMIT];
int frameSize;//Size of frame
int steps = 0;//Track number of steps
int counter = 0;//Track which index was pushed in first for FIFO
int numberOfEmptyslots = 0;//Track number of empty slots
int pageFaults = 0;//Track number of page faults
bool change = false;//To check if there is a change needed in Frame
int choice = 0;


void initArrays();//inititalize the arrays
void FIFO(int frame[]);//FIFO algorithim
void LRU(int frame[]);//LRU algorithim
void OPT(int frame[]); //OPS algorithim

int main()
{

    printf("Please select an algorithim\n");
    printf("1.First In First Out (FIFO)\n");
    printf("2.Optimal page replacement algorithm\n");
    printf("3)Least recently used (LRU) page replacement algorithm\n");
    scanf("%d",&choice);

    fflush(stdin);
   

    //Get user input for referenced string
    printf("Enter referenced string(range of 20-30 frames): ");
    fgets(str,INPUTLIMIT,stdin);


     //Get user input for frame size
    printf("\nEnter frame size(range of 3-6 frames): ");
    scanf("%d",&frameSize);
    numberOfEmptyslots = frameSize;

    int frame[frameSize];



   
    initArrays(frame);
    //FIFO(frame);
     switch (choice)
    {
    case 1:
        FIFO(frame);
        break;
    case 2:
        OPT(frame);
        break;

    case 3:
        LRU(frame);
        break;
    
    default:
        break;
    }

  
    return 0;
}

//Initialize the arrays after inputs
void initArrays(int frame[])
{
    //set default values for refPages
     for(int i =0; i < MAXLIMIT; ++i)
    {
        refPages[i] = -1;
    }
    //set default values for frame
    for(int i =0; i < frameSize; ++i)
     {
         frame[i] = -1;
     }

    //Seperate commas from char array and place to int array
    for(int i =0; i< INPUTLIMIT; ++ i)
    {
        if(str[i] == '\0' || str[i] == '\n')
            break;
        else
        {
            //check for commas or spaces
            if(str[i] == ',' || str[i] == ' ' )
                continue;
            else
            {
                //converting of char to int
                refPages[counter] =  str[i] -'0' ;
                counter++;
            }
        }
    }
}

void FIFO(int frame[])
{
    counter = 0; // to keep track of index for FIFO
    int pageSize = sizeof(refPages)/sizeof(refPages[0]);//To get size of array
    for(int i = 0; i < pageSize; ++ i)
    {
        change = true;
        //Break if -1 is detected
        if(refPages[i] == -1)
            break;
        else
        {
            //Check if value is already in frame.
            for(int y = 0; y < frameSize; ++y)
            {
                if(frame[y] == refPages[i])
                {
                    //No changes needed if value is found
                    change = false;
                    break;
                }
                else
                    //Change needed
                    change = true; 
            }
        }
        //If change is needed
        if(change)
        {
            for(int j = 0; j < frameSize; ++j)
            {
                //Check for empty slots and fill the empty slots first.
                if(numberOfEmptyslots > 0)
                {                
                    if(frame[j] == -1)
                    {
                        frame[j] = refPages[i];
                        numberOfEmptyslots--;
                        steps++;
                        change = true;
                        break;
                    }
                    continue;
                }
                //By using counter to keep track of which index was queued first for FIFO Algorithim
                else
                {
                    frame[counter] = refPages[i];
                    counter++;
                    if(counter >= frameSize)
                        counter = 0;
                }  
                change = true;
                steps++;
                break;
            }  
        }
        //If there is a change in frame, print out the current step
        if(change == true)
        {
            printf("Step %d: ",steps);
            for(int n = 0; n < frameSize; ++n)
                printf("%d\t",frame[n]);
            printf("\n");
        }    
    }
     printf("There are %d page faults in this page replacement process",steps);
}


void LRU(int frame[])
{
    int pageSize = sizeof(refPages)/sizeof(refPages[0]);//To get size of array
    int indexCount[frameSize];//To store time for each frame
    int lru = 0;//index which will contain the LRU page

    //Check which was the least recently used page
    for(int i = 0; i < pageSize; ++ i)
    {
        change = true;
        //Break if -1 is detected
        if(refPages[i] == -1)
            break;
         else
        {
            //Check if value is already in frame.
            for(int y = 0; y < frameSize; ++y)
            {
                if(frame[y] == refPages[i])
                {
                    //No changes needed if value is found
                    change = false;
                    indexCount[y] = 0;
                    //Loop to add time to frames not affected
                    for(int f = 0; f < frameSize; ++f)
                    {
                        if(f!=y)
                            indexCount[f]+=1;
                    }
                    break;
                }
                else
                    //Change needed
                    change = true; 
            }
        }
        if(change)
        {
            for(int j = 0; j < frameSize; ++j)
            {
                //Check for empty frames and fill first
                if(numberOfEmptyslots > 0)
                {                
                    if(frame[j] == -1)
                    {
                        frame[j] = refPages[i];
                        numberOfEmptyslots--;
                        steps++;
                        change = true;
                        //increment Time for the affected frames.
                        for(int k = 0; k < frameSize; ++k)
                        {
                            if(frame[k] != -1)
                                indexCount[k]+=1;
                        }
                        break;
                    }
                    continue;
                }
                else
                {       
                    int max = indexCount[0];
                    lru = 0;
                    //Find the index with the highest LRU time
                    for(int x = 0; x<frameSize; ++x)
                    {
                        if(max < indexCount[x])
                        {
                            max = indexCount[x];
                            lru = x;
                        }
                    }
                    //Assign page to index
                    frame[lru] = refPages[i];
                    indexCount[lru] = 0;

                    //Increment time for frames not affected
                    for(int y = 0; y < frameSize; ++y)
                    {
                        if(y!= lru)
                            indexCount[y]+=1;
                    }
                    change = true;
                    steps++;
                    break;
                }
            }
        }
            //If there is a change in frame, print out the current step
            if(change == true)
            {
                printf("Step %d: ",steps);
                for(int n = 0; n < frameSize; ++n)
                    printf("%d\t",frame[n]);
                printf("\n");
            }    
    }

    printf("There are %d page faults in this page replacement process",steps);
}



/*Optimal page replacement Algorithm:
    Replace pages that will not be used for the longest period of time
*/
void OPT(int frame[])
{

    int firstOccuranceArr[6]={0}; //to store the firstOccurance 
    int pageSize = sizeof(refPages)/sizeof(refPages[0]);//To get size of array

    int strLen=0; //to get len of str
    while(refPages[strLen]!=-1){
                    strLen++;
                }
    strLen--; //arr start from 0

    for(int i = 0; i < pageSize; ++ i)
    {
        change = true;
        //Break if -1 is detected
        if(refPages[i] == -1)
            break;
        else
        {
            //Check if value is already in frame.
            for(int y = 0; y < frameSize; ++y)
            {
                if(frame[y] == refPages[i])
                {
                    //No changes needed if value is found
                    change = false;
                    break;
                }
                else
                    //Change needed
                    change = true; 
            }
        }
        //If change is needed
        if(change)
        {
            for(int j = 0; j < frameSize; ++j)
            {
                //Check for empty slots and fill the empty slots first.
                if(numberOfEmptyslots > 0)
                {                
                    if(frame[j] == -1)
                    {
                        frame[j] = refPages[i];
                        numberOfEmptyslots--;
                        steps++;
                        change = true;
                        break;
                    }
                    continue;
                }
                
                else
                {         
                    /* iterate through the memory frame
                        find the first occurance(Index) of each page in the memory frame on the ref string
                        The larger the first occurance value mean it will not be used for the longest*/
                    int k=0;
                    while(k<frameSize){
                        int firstOccuranceI=i; //use firstOccuranceI so to keep iterate through remaining pages on ref string
                        while(firstOccuranceI<=strLen){ 
                            if(frame[k]==refPages[firstOccuranceI]){    
                                firstOccuranceArr[k]=firstOccuranceI; //store first occurance of memory frame's page in firstOccuranceArr[]
                                k++;
                                firstOccuranceI=i; //set back to i as have to get first occurance for all pages in memory frame
                                break;
                            }
                            if(firstOccuranceI==strLen){  //if remaining ref string doesn't contain the memory page's frame
                                firstOccuranceArr[k]=MAXLIMIT+1; //Maxlimit+1 indicate's that the index is out of bound
                                k++;                          
                                firstOccuranceI=i;
                                break;
                                }           
                            else{
                                firstOccuranceI++;
                            }
                        }     
                    }
                    /*find Max algo: 
                        -To determine Victim frame
                        - by retrieving the page in memory frame which has the largest firstOccurance value
                    */
                    int currentMax=firstOccuranceArr[0]; //let the first value in firstOccuranceArr[0] to be max
                    int currentMaxIdx=0; //to keep track of largest first occurance index
                    for(int l=1;l<frameSize;l++){ 
                        if (firstOccuranceArr[l]>currentMax){  //if first occurance index is larger then currentMax value.
                            currentMax=firstOccuranceArr[l];   // it will be the new currentMax
                            currentMaxIdx=l;    //update currentMaxIdx with new largest first occurance index
                        }
                    }
                    //swap the victim frame with the current ref string page.
                    frame[currentMaxIdx] = refPages[i];
                }  
                change = true;
                steps++;
                break;
            }  
        }
        //If there is a change in frame, print out the current step
        if(change == true)
        {
            printf("Step %d: ",steps);
            for(int n = 0; n < frameSize; ++n)
                printf("%d\t",frame[n]);
            printf("\n");
        }    
    }
     printf("There are %d page faults in this page replacement process",steps);
}