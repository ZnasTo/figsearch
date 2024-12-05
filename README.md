# figsearch
## Description
`figsearch` is a program designed to search for specific geometric patterns in a bitmap. It can identify:
- The longest horizontal line
- The longest vertical line
- The largest square
The program outputs the coordinates of the identified pattern closest to the origin (top-left corner: `(0, 0)`).


## Command Syntax

### --help
Displays the user guide.
```
./figsearch --help
```
### Test Mode
Validates the format of the input file to ensure it adheres to the required bitmap specifications.
```
./figsearch test FILE
```

### Pattern Search
Runs the main program to search for a specified pattern in a bitmap.
```
./figsearch OBJECT FILE
```
- **`OBJECT`:** The pattern to search for in the bitmap. Supported values are:
  - `hline`: Searches for the longest horizontal line.
  - `vline`: Searches for the longest vertical line.
  - `square`: Searches for the largest square.
- **`FILE`:** Path to the file containing the bitmap.

## Bitmap Format
Input files must adhere to the following format:
```
HEIGHT WIDTH
ROW_1
ROW_2
...
ROW_N
```
- The first line specifies the `HEIGHT` and `WIDTH` of the bitmap.
- The following lines define the bitmap using binary values (`0` and `1`) separated by any number of white characters.

Example:
```
4 5
0 0 1 1 1
0 0 1 0 1
1 0 1 1 1
1 1 1 1 1
```
## Output 
The program outputs the coordinates of the **start** and **end** points defining the identified pattern:
- **`start` point:** Closest point to the origin (top-left corner for a square).
- **`end` point:** Farthest point from the origin (bottom-right corner for a square).


### Output format
```R1 C1 R2 C2```
- `R1 C1` - Start point coordinates
- `R2 C2` - End point coordinates

## Example Usage

### Command Example
Search for the longest horizontal line in a bitmap:
```
./figsearch hline matrix.txt
```

### Input File (`matrix.txt`)
```
4 5
0 0 1 1 1
0 0 1 0 1
1 0 1 1 1
1 1 1 1 1
```
### Output
```
3 0 3 4
```
