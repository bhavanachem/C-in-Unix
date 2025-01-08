# Number Guessing Game

## Overview

This is a **C program** for a number guessing game where the user attempts to guess a randomly generated number between -100 and 100. The game provides feedback on whether the user's guess is too high or too low and tracks the number of attempts taken by the user to guess the correct number. A game report is maintained to record the username and the number of attempts for each game.

## Learning Process

While working on this project, I got to code several key concepts I learned, such as writing functions with parameters in C to reuse code. I also learned how to generate random numbers within a specific range using rand(). I also learned writing and creating files using file descriptors (fd), which was essential for storing the game report. Finally, I learned how to implement while and if loops which allowed me to compare the guessed number to the random number, guiding the player with hints like "too high" or "too low."
One challenge I encountered was using Booleans in C. I solved this by including the <stdbool.h> header and understanding how to use true and false to represent logical values. Another challenge I faced was when I got an error when I was calling the functions I wrote, but I learned that I simply needed to declare the functions before the main function. 

## How to Play

1. Start the program and enter your username.  
2. The game will print the range of the random number and the maximum allowed guesses.  
3. Enter your guesses, and the program will indicate whether your guess is "low" or "high."  
4. If you guess correctly within the allowed attempts, you win, and your score is saved in the game report.  
5. If you fail to guess the number within the allowed attempts, the game ends, and the correct number is displayed.

## Example Session

<img width="635" alt="Project2_Output" src="https://github.com/user-attachments/assets/0ffd0d30-2527-4a5c-b6c8-d21cf8185a49" />
