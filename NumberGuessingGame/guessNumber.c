#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
//function definitions
void endGame(char* username, int guessCount, int allowedGuess, bool result);
int generateRandomNumner(int min, int max);
int getGuess();
void helpGuess(char *username, int userguess, int number, int guessCount, int allowedGuess);
void writeReportFile(char* username, int guessCount, int allowedGuess, bool result);
main()
{
//ask for guess
int allowedGuess = 7;
int guessCount,userguess;

char* username = getenv("USER");
if(username == NULL){
printf("Enter username: ");
scanf("%49s",username); //reads up to 49 characters 
}
printf("Hello %s, I have a number between -100 to 100. Can you guess it in %d guesses? Enter your guess:\n",username, allowedGuess);

//have computer choose a random number
int number = generateRandomNumber(-100,100);

while(userguess!=number && allowedGuess>guessCount){
userguess = getGuess();
guessCount++;
helpGuess(username, userguess,number, guessCount, allowedGuess);
}
if(userguess!=number)
endGame(username,guessCount, allowedGuess, 0);

}//end main

//functions
int generateRandomNumber(int min, int max){
srand(time(0));
int number =(rand() % (max-min+1)) +min;
//printf("Number between %d and %d: %d\n", min,max,number);
return number;
}

void endGame(char* username, int guessCount,int allowedGuess, bool result){
//printf("Guess count = %d, Allowed Guess = %d\n", guessCount, allowedGuess);
if(guessCount<allowedGuess || result){
printf("Correct! You guessed with %d/%d guesses.\n", guessCount, allowedGuess);
writeReportFile(username, guessCount, allowedGuess, 1);
}
else if (guessCount>allowedGuess || (!result)){
printf("Bummer. You couldn't guess in %d allowed attempts.\n", allowedGuess);
writeReportFile(username,guessCount,allowedGuess,0);
}
}

int getGuess(){
int guess;
scanf("%d",&guess);
return guess;
}

void helpGuess(char* username,int userguess, int number,int guessCount, int allowedGuess){
if(userguess<number)
printf("Too low. Guess again\n");
if(userguess>number)
printf("Too high. Guess again\n");
if(userguess==number){
endGame(username,guessCount, allowedGuess, 1);
}
}

void writeReportFile(char* username, int guessCount, int allowedGuess, bool result){
int fd = open("game_report.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
if(fd <0)
printf("Error opening file for writing\n");
char buffer[256];
if(result)
snprintf(buffer, sizeof(buffer), "User: %s, Guessed correctly in %d/%d guesses\n", username, guessCount, allowedGuess);
else
snprintf(buffer, sizeof(buffer), "User: %s, Failed to guess in %d attempts\n", username, allowedGuess);
write(fd, buffer, strlen(buffer));
close(fd);
}
