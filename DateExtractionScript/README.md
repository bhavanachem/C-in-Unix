# Date Extraction Script

## Overview

This project contains a Bash script designed to identify and extract all possible date formats from a given text file. It is a utility for parsing documents and identifying dates, supporting multiple common formats. The program outputs the extracted dates and their count, both to the terminal and to a report file.

## Learning Process
While developing this Bash script, I learned how to handle input files, generate reports, use loops, and work with the grep command to extract date patterns. In the process of finding and extracting the patterns, I started by identifying numeric date formats such as "MM/DD/YYYY" and "YYYY-MM-DD." Afterward, I expanded the script to recognize alphabetical variations like "January 1st, 2023.”
One of the challenges I faced was distinguishing the word forms of months from regular words in the text ex. "for 20”. I resolved my script picking up unwanted phrases by adjusting the regular expressions to identify capitalized month names.
To ensure the accuracy of the extracted dates, I decided to use the -E option in grep, which allows advanced pattern matching features so I could use “+”, “?”, and “{}”. I also incorporated the -o option to extract only the matching date parts instead of entire lines, keeping the output clean.

## Features

- Extracts dates in various formats, including:
  - `January 1, 2023`
  - `01/01/2023`
  - `2023-01-15`
  - `15th January 2023`
  - `01-15-2023`
  - `Jan 20th, 2023`
  - `20th January, 2023`
- Displays the extracted dates and the total count on the terminal.
- Writes the extracted dates and total count to a report file (`date_report.txt`).

## Requirements

- A Unix-based operating system.
- Bash shell.
- A text file with dates in it.
