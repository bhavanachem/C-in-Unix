# C-in-Unix
A collection of C programs designed to explore and implement core concepts of Unix systems programming. This repository includes topics such as file handling, process management, inter-process communication, shell scripting, and system calls.

---

## Programs

### Date Extraction Program

**Description:**  
This program extracts dates in `MM/DD/YYYY` or `MM-DD-YYYY` format from a given text file and saves the extracted dates into a new output file. It is useful for parsing documents to identify and isolate date patterns.

**Features:**  
- Reads input from a specified text file.  
- Extracts all dates in many formats including `MM/DD/YYYY` and `MM-DD-YYYY`.  
- Writes the extracted dates to a separate output file.

**How to Use:**  
1. Run the program and provide the path to the input text file when prompted.  
2. The program will generate an output file containing the extracted dates.  

---

### Number Guessing Game

**Description:**  
This interactive game challenges the user to guess a random number between -100 and 100. The program provides hints for each guess and tracks the user's performance. A game report is maintained for each session.

**Features:**  
- Generates a random number using a custom function with user-defined range parameters.  
- Provides feedback on whether guesses are too high or too low.  
- Limits the user to 7 guesses and congratulates successful players.  
- Writes the username and number of attempts to a game report file.  

**How to Use:**  
1. Run the program and enter your username.  
2. Guess the random number within the allowed number of attempts.  
3. Check your performance in the `game_report.txt` file.  
