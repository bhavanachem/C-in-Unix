# Date Extraction Script

## Overview

This project contains a Bash script designed to identify and extract all possible date formats from a given text file. It is a utility for parsing documents and identifying dates, supporting multiple common formats. The program outputs the extracted dates and their count, both to the terminal and to a report file.

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
