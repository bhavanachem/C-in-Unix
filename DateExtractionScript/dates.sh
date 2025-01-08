#!/bin/bash

input_file="$1"
report_file="date_report.txt"

> "$report_file"

date_patterns=(
'\b[0-9]{2}/[0-9]{2}/[0-9]{4}\b' #MM/DD/YYYY
'\b[0-9]{2}-[0-9]{2}-[0-9]{4}\b' #MM-DD-YYYY
'\b[0-9]{4}-[0-9]{2}-[0-9]{2}\b' #YYY-MM-DD
'\b[0-9]{1,2}(st|nd|rd|th)? [A-Z][a-z]+,? [0-9]{4}\b' #22nd February 2016
'\b[A-Z][a-z]+ [0-9]{1,2}(st|nd|rd|th)?(,)? [0-9]{4}\b' #February 1/1st, 2010
'\b[A-Z][a-z]{2} [0-9]{1,2}(st|nd|rd|th)?\b'
)
extracted_dates=$(mktemp)
for pattern in "${date_patterns[@]}"; do
grep -E -o "$pattern" "$input_file" >> "$extracted_dates"
done

sort "$extracted_dates" | uniq > "$report_file"
rm "$extracted_dates"

total_dates=$(wc -l < "$report_file")

echo "Extracted Dates:"
cat "$report_file"
echo
echo "Total number of extracted dates: $total_dates"

echo >> "$report_file"
echo "Total number of extracted dates: $total_dates" >> "$report_file"

echo "Date extraction completed. Check the report file: $report_file"
