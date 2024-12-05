// Inclusions of libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro definitions
#define FIRST_WHITE_CHAR_VAL 9
#define LAST_WHITE_CHAR_VAL 13

/**
 * Represents a matrix
 * @struct MATRIX
 */
typedef struct {
    int numOfRows;
    int numOfCols;
    char **values;
} MATRIX;

/**
 * Represents a point in 2D space
 * @struct POINT
 */
typedef struct {
    int row;
    int col;
} POINT;

/**
 * Represents a line in a matrix
 * @struct LINE
 */
typedef struct {
    POINT startCoordinates;
    POINT endCoordinates;
    int length;
} LINE;

/**
 * Represents a line in a matrix
 * @struct SQUARE
 */
typedef struct {
    POINT startCoordinates;
    POINT endCoordinates;
    int size;
} SQUARE;

// Function declarations
void printHelpMsg();

void printLargestSquare(MATRIX *matrix);

void printLongestLine(LINE longestLine);

int readMatrix(FILE *file, MATRIX *matrix);

int checkNumOfParams(int argc);

FILE *openInputFile(char *fileName);

int testInputFileValidity(FILE *file);

int checkMatrixDimensions(FILE *file);

int checkMatrixValues(FILE *file, int numOfCols, int numOfRows);

void allocateMatrix(MATRIX *matrix, int numOfRows, int numOfCols);

void printMatrix(MATRIX *matrix);

void readMatrixFromFile(MATRIX *matrix, FILE *file);

LINE findLongestHorizontalLine(MATRIX *matrix);

LINE findLongestVerticalLine(MATRIX *matrix);

void setPoint(POINT *point, int row, int col);

void printPoint(POINT point);

SQUARE findLargestSquare(MATRIX *matrix);

SQUARE findSquareFromPoint(POINT point, MATRIX *matrix, int largestSquareSize);

int lenOfTheLongestHLineFromPoint(POINT point, MATRIX *matrix, int cap);

int lenOfTheLongestVLineFromPoint(POINT point, MATRIX *matrix, int cap);

SQUARE findLargestSubsquare(POINT point, MATRIX *matrix, int squareSize, int largestSquareSize);

int squareCloserToTheOrigin(SQUARE square1, SQUARE square2);

int lineCloserToTheOrigin(LINE line1, LINE line2);

void searchForLargestSquareInCol(SQUARE *largestSquare, MATRIX *matrix, int searchedSquareCol, int searchedSquareRow);

void searchForLargestSquareInRow(SQUARE *largestSquare, MATRIX *matrix, int searchedSquareCol, int searchedSquareRow);

void elongateLine(LINE *line, int row, int col);

void setNewLongestLine(LINE *possibleLongestLine, LINE *longestLine);

int testInputFile(FILE *file);

void searchMatrix(char *argument, MATRIX *matrix);

void freeMatrix(MATRIX *matrix);

int isWhiteChar(char character);

int main(int argc, char *argv[]) {
    if (checkNumOfParams(argc) == -1)
        return -1;

    if (!strcmp(argv[1], "--help")) {
        printHelpMsg();
        return 0;
    }

    FILE *file;
    if ((file = openInputFile(argv[2])) == NULL) {
        fprintf(stderr,"Input File could not be opened");
        return -1;
    }

    if (!strcmp(argv[1], "test"))
        return testInputFile(file);

    MATRIX *matrix = malloc(sizeof(MATRIX));
    if (matrix == NULL)
        return -1;

    if (readMatrix(file, matrix) == -1) {
        return -1;
    }
    // printMatrix(matrix);

    searchMatrix(argv[1], matrix);
    freeMatrix(matrix);
    fclose(file);
    return 0;
}

/// frees the memory allocated for the Matrix
void freeMatrix(MATRIX *matrix) {
    for (int i = 0; i < matrix->numOfRows; ++i) {
        free(matrix->values[i]);
    }
    free(matrix->values);
    free(matrix);
}

/// prints the Usage guide to stdio
void printHelpMsg() {
    printf("Usage guide:\n"
        "\tfirst argument:\n"
        "\t\t --help - prints usage guide\n"
        "\t\t test - checks the validity of the input file \n"
        "\t\t hline - finds the longest horizontal line\n"
        "\t\t vline - finds the longest vertical line\n"
        "\t\t square - finds the largest square\n"
        "\tsecond argument:\n"
        "\t\t input file\n"
    );
}

/**
 * Searches matrix for a shape specified by the argument
 * @param argument program argument
 * @param matrix matrix to be searched
 */
void searchMatrix(char *argument, MATRIX *matrix) {
    if (!strcmp(argument, "hline")) {
        LINE longestLine = findLongestHorizontalLine(matrix);
        printLongestLine(longestLine);
    } else if (!strcmp(argument, "vline")) {
        LINE longestLine = findLongestVerticalLine(matrix);
        printLongestLine(longestLine);
    } else if (!strcmp(argument, "square")) {
        printLargestSquare(matrix);
    } else {
        fprintf(stderr, "Second parameter is not valid option\n use --help to get user guide");
    }
}

/**
 * Prints out the largest square from matrix
 * @param matrix metrix in which the square is located
 */
void printLargestSquare(MATRIX *matrix) {
    SQUARE square = findLargestSquare(matrix);
    if (square.size == 0) {
        printf("Not Found");
    } else {
        printPoint(square.startCoordinates);
        printf(" ");
        printPoint(square.endCoordinates);
        printf("\n");
        // printf("%d\n", square.size);
    }
}

/**
 * Reads matrix from a file
 * @param file file from which matrix is read
 * @param matrix matrix in which read values are stored
 * @return 0 if matrix was successfuly read
 * @return -1 in case of error
 */
int readMatrix(FILE *file, MATRIX *matrix) {
    if (testInputFileValidity(file) == -1) {
        fprintf(stderr, "Data in the input file are not valid");
        free(matrix);
        return -1;
    }

    readMatrixFromFile(matrix, file);
    if (matrix == NULL) {
        return -1;
    }
    return 0;
}

/**
 * prints line to stdio if it is a line otherwise print "Not Found"
 * @param longestLine line to be printed
 */
void printLongestLine(LINE longestLine) {
    if (longestLine.length == 0) {
        printf("Not Found");
    } else {
        printPoint(longestLine.startCoordinates);
        printf(" ");
        printPoint(longestLine.endCoordinates);
    }
}

/**
 * Tests the validity of the data in the input file
 * @param file input file
 * @return -1 if data are invalid
 * @return 0 if data are valid
 */
int testInputFileValidity(FILE *file) {

    int numOfCols, numOfRows;
    // resets cursor
    fseek(file, 0,SEEK_SET);
    fscanf(file, "%d%d\n", &numOfRows, &numOfCols);

    // checks if number of rows and columns is not negative
    if (numOfRows < 0 || numOfCols < 0) {
        return -1;
    }

    // checks the validity of matrix data
    if (checkMatrixValues(file, numOfCols, numOfRows) == -1) {
        return -1;
    }

    return 0;
}

/**
 * Checks if the number of program arguments is valid
 * @param argc number of program arguments
 * @return 0 if number of arguments is valid
 * @return -1 if number of arguments is not valid
 */
int checkNumOfParams(int argc) {
    if (argc > 3 || argc < 2) {
        fprintf(stderr, "Invalid number of arguments");
        return -1;
    }
    return 0;
}


/**
 * Opens input file
 * @param fileName fileName name of a file to be oppened
 * @return file pointer if file was successfuly opened
 * @return -1 if file could not be opened
 */
FILE *openInputFile(char fileName[]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "File could not be opened");
    }
    return file;
}

/**
 * Test if data in the input file are in a valid format
 * @param file file to be tested
 * @return 0 if data in file are valid
 * @return -1 if data in file are Invalid
 */
int testInputFile(FILE *file) {
    if (testInputFileValidity(file) == 0) {
        printf("Valid");
        return 0;
    }
    fprintf(stderr, "Invalid");
    return -1;
}

/**
 * Checks if matrix values are in a valid format
 * @param file file in which matrix is located
 * @param numOfCols number of colums of matrix
 * @param numOfRows number of row of the matrix
 * @return 0 if matrix values are in valid
 * @return -1 if matrix values are in valid
 */
int checkMatrixValues(FILE *file, int numOfCols, int numOfRows) {
    int numOfNums = 0;
    char bit, previousBit = ' ';

    while((bit = fgetc(file)) != EOF) {
        if (bit == '1' || bit == '0') {
            if (previousBit == '1' || previousBit == '0')
                return -1;
            numOfNums ++;
        } else if (!isWhiteChar(bit)) {
            return -1;
        }
        previousBit = bit;
    }

    // checks if there is valid number of numbers
    if (numOfNums != numOfCols * numOfRows)
        return -1;

    return 0;
}

/**
 * Allocates the memory for a matrix
 * @param numOfRows number of rows
 * @param numOfCols number of columns
 * @return pointer to matrix, NULL if allocation failed
 */
void allocateMatrix(MATRIX *matrix, int numOfRows, int numOfCols) {
    matrix->numOfRows = numOfRows;
    matrix->numOfCols = numOfCols;

    // allocation of rows
    matrix->values = malloc(sizeof(char *) * matrix->numOfRows);
    if (matrix->values == NULL) {
        free(matrix);
        return;
    }

    // allocation of cols
    for (int i = 0; i < matrix->numOfRows; ++i) {
        matrix->values[i] = malloc(sizeof(char) * matrix->numOfCols);
        if (matrix->values[i] == NULL) {
            for (int j = 0; j < i; ++j) {
                free(matrix->values[j]);
            }
            free(matrix->values);
            free(matrix);
            return;
        }
    }
}

/**
 * Reads the matrix file and puts in into a matrix struct
 * @param matrix pointer to the matrix to which are values are stored
 * @param file pointer to the input file from which the matrix is read
 */
void readMatrixFromFile(MATRIX *matrix, FILE *file) {
    int numOfRows = 0, numOfCols = 0;

    // reset cursor to the file start
    fseek(file, 0,SEEK_SET);
    fscanf(file, "%d%d\n", &numOfRows, &numOfCols);

    allocateMatrix(matrix, numOfRows, numOfCols);
    if (matrix == NULL) {
        fprintf(stderr, "Matrix memory allocation failed");
        freeMatrix(matrix);
        return;
    }

    char bit;
    for(int i = 0, j = 0; (bit = fgetc(file)) != EOF;) {
        // reseting the col counter
        if (j == numOfCols) {
            j = 0;
            i++;
        }
        // check if values arent out of bounds
        if (i == numOfRows) {
            break;
        }
        // storing value to the matrix
        if (bit == '1' || bit == '0') {
            matrix->values[i][j] = bit;
            j++;
        }
    }

}

/**
 * Seraches for the longest horizontal line
 * @param matrix matrix which in which the line is searched
 * @return returnst the first longest horizontal line
 */
LINE findLongestHorizontalLine(MATRIX *matrix) {
    LINE longestLine = {0}, possibleLongestLine = {0};

    for (int i = 0; i < matrix->numOfRows; ++i) {
        for (int j = 0; j < matrix->numOfCols; ++j) {
            // checks if any longer line can be found in the row
            if (matrix->numOfCols - j + possibleLongestLine.length < longestLine.length) {
                break;
            }
            if (matrix->values[i][j] == '1') {
                elongateLine(&possibleLongestLine, i, j);
            } else {
                setNewLongestLine(&possibleLongestLine, &longestLine);
            }
        }
        setNewLongestLine(&possibleLongestLine, &longestLine);

        // chcecks if there can be any longer line in the matrix
        if (longestLine.length == matrix->numOfCols) {
            break;
        }
    }
    return longestLine;
}

/**
 * Seraches for the longest vertical line
 * @param matrix matrix which in which the line is searched
 * @return returnst the first longest vertical line
 */
LINE findLongestVerticalLine(MATRIX *matrix) {
    LINE longestLine = {0}, possibleLongestLine = {0};

    for (int i = 0; i < matrix->numOfCols; ++i) {
        for (int j = 0; j < matrix->numOfRows; ++j) {
            // checks if any longer line can be found in the row
            if (matrix->numOfRows - j + possibleLongestLine.length < longestLine.length) {
                break;
            }
            if (matrix->values[j][i] == '1') {
                elongateLine(&possibleLongestLine, j, i);
            } else {
                setNewLongestLine(&possibleLongestLine, &longestLine);
            }
        }
        setNewLongestLine(&possibleLongestLine, &longestLine);
        // chcecks if there can be any longer line in the matrix
        if (longestLine.length == matrix->numOfRows) {
            break;
        }
    }
    return longestLine;
}

/// elongates line and sets start and end coordinates
void elongateLine(LINE *line, int row, int col) {
    // checks if any the possibleLongesLine has started
    if (line->length == 0) {
        setPoint(&line->startCoordinates, row, col);
        setPoint(&line->endCoordinates, row, col);
    } else {
        setPoint(&line->endCoordinates, row, col);
    }
    line->length++;
}

/// if possibleLongestLine is longer and closer to the origin than longestLine it becomes the new longest line and resets its lengt
void setNewLongestLine(LINE *possibleLongestLine, LINE *longestLine) {
    if (possibleLongestLine->length > longestLine->length) {
        *longestLine = *possibleLongestLine;
    } else if(possibleLongestLine->length == longestLine->length
        && lineCloserToTheOrigin(*possibleLongestLine, *longestLine) == 1) {
        *longestLine = *possibleLongestLine;
    }
    // resets the possibleLongestLine lenght
    possibleLongestLine->length = 0;
}

/**
 * Sets a new coordinates to a poin in 2D space
 * @param point point in 2D space which coordinates are to be changed
 * @param row new row coordinate value
 * @param col new col coordinate value
 */
void setPoint(POINT *point, int row, int col) {
    point->row = row;
    point->col = col;
}

/// Prints out a point to stdio
void printPoint(POINT point) {
    printf("%d %d", point.row, point.col);
}

/**
 *  Searches for the largest square in matrix
 * @param matrix matrix that is searched for the square
 * @return largest square
 */
SQUARE findLargestSquare(MATRIX *matrix) {
    SQUARE largestSquare = {0};

    for (int searchedSquareCol = -1, searchedSquareRow = -1;
         searchedSquareCol + 1 != matrix->numOfCols
         || searchedSquareRow + 1 != matrix->numOfRows;) {

        if (searchedSquareRow + 1 < matrix->numOfRows) {
            searchedSquareRow++;
        }
        if (searchedSquareCol + 1 < matrix->numOfCols) {
            searchedSquareCol++;
        }

        searchForLargestSquareInCol(&largestSquare, matrix, searchedSquareCol, searchedSquareRow);
        searchForLargestSquareInRow(&largestSquare, matrix, searchedSquareCol, searchedSquareRow);

        // checks if a larger sqaure can be found in the matrix
        if (matrix->numOfCols - searchedSquareCol < largestSquare.size
            && matrix->numOfRows - searchedSquareRow < largestSquare.size) {
            break;
        }
    }
    return largestSquare;
}

/**
 * Searches for the largest square from the point
 * @param point point of origin of the square
 * @param matrix matrix in which square is searched
 * @param largestSquareSize the size of the largest already found square
 * @return the largest square originating from the point (if there isnt a square
 * of same of larger size than the one already found returns square of size 0)
 */
SQUARE findSquareFromPoint(POINT point, MATRIX *matrix, int largestSquareSize) {
    int HLineLenght = lenOfTheLongestHLineFromPoint(point, matrix, matrix->numOfCols);
    int VLineLenght = lenOfTheLongestVLineFromPoint(point, matrix, matrix->numOfRows);
    int searchedSquareSize;

    if (HLineLenght > VLineLenght) {
        searchedSquareSize = VLineLenght;
    } else {
        searchedSquareSize = HLineLenght;
    }
    return findLargestSubsquare(point, matrix, searchedSquareSize, largestSquareSize);
}

/**
 * Searches for the largest subsqare larger with size larger than largestSquareSize
 * @param point point from which to search fro a subsquare
 * @param matrix matrix in which the point is located
 * @param squareSize the maximal size subsquare can have
 * @param largestSquareSize the size of the largest square already found
 * @return returns the largest square if subsquare is larger than largestSquareSize,
 * otherwise returns square with length 0
 */
SQUARE findLargestSubsquare(POINT point, MATRIX *matrix, int squareSize, int largestSquareSize) {
    POINT endPoint = {0};

    // check if the searched area can have a square larger or equal to the one already found
    if (squareSize < largestSquareSize) {
        setPoint(&endPoint, 0, 0);
        squareSize = 0;
    } else {
        // setting other corners of the matrix
        POINT bottomLeftCorner = {point.row + squareSize - 1, point.col};
        POINT topRightCorner = {point.row, point.col + squareSize - 1};

        // checking if the square can be created
        // checking from the largest to the smallest
        if (lenOfTheLongestHLineFromPoint(bottomLeftCorner, matrix, squareSize + point.col) < squareSize
            || lenOfTheLongestVLineFromPoint(topRightCorner, matrix, squareSize + point.row) < squareSize) {
            // recursively checking smaller squares
            return findLargestSubsquare(point, matrix, squareSize - 1, largestSquareSize);
        }
        setPoint(&endPoint, point.row + squareSize - 1, point.col + squareSize - 1);
    }
    SQUARE square = {point, endPoint, squareSize};
    return square;
}

/// searches for the longest horizontal line from a poin and returns its size
int lenOfTheLongestHLineFromPoint(POINT point, MATRIX *matrix, int cap) {
    int i = point.col;
    for (; i < cap && matrix->values[point.row][i] == '1'; ++i) {
    }
    return (i - point.col);
}

/// searches for the longest vertical line from a poin and returns its size
int lenOfTheLongestVLineFromPoint(POINT point, MATRIX *matrix, int cap) {
    int i = point.row;
    for (; i < cap && matrix->values[i][point.col] == '1'; ++i) {
    }
    return (i - point.row);
}

/**
 * Compares which square is closer to the orign (0,0)
 * @param square1 square 1 to be compared
 * @param square2 square 2 to be compared
 * @return 1 if square1 is closer to the origin
 * @return returns 0 if the distance is the same
 * @return -1 if the square 2 is closer to the origin
 */
int squareCloserToTheOrigin(SQUARE square1, SQUARE square2) {
    int sq1PosValue = square1.startCoordinates.col * square1.startCoordinates.col +
                      square1.startCoordinates.row * square1.startCoordinates.row;
    int sq2PosValue = square2.startCoordinates.col * square2.startCoordinates.col +
                      square2.startCoordinates.row * square2.startCoordinates.row;

    if (sq1PosValue < sq2PosValue) {
        return 1;
    }
    if (sq1PosValue == sq2PosValue) {
        if (square1.startCoordinates.row < square2.startCoordinates.row) {
            return 1;
        }
        return 0;
    }
    return -1;
}

/**
 * Compares which line is closer to the orign (0,0)
 * @param line1 square 1 to be compared
 * @param line2 square 2 to be compared
 * @return 1 if line1 is closer to the origin
 * @return returns 0 if the distance is the same
 * @return -1 if line2 is closer to the origin
 */
int lineCloserToTheOrigin(LINE line1, LINE line2) {
    int sq1PosValue = line1.startCoordinates.col * line1.startCoordinates.col +
                      line1.startCoordinates.row * line1.startCoordinates.row;
    int sq2PosValue = line2.startCoordinates.col * line2.startCoordinates.col +
                      line2.startCoordinates.row * line2.startCoordinates.row;

    if (sq1PosValue < sq2PosValue) {
        return 1;
    }
    if (sq1PosValue == sq2PosValue) {
        if (line1.startCoordinates.row < line2.startCoordinates.row) {
            return 1;
        }
        return 0;
    }
    return -1;
}

/**
 * Searches for the largest Square in column
 * if larger square is found it becomes the new largestSquare
 * @param largestSquare largest found square
 * @param matrix matrix to be searched for a larger square
 * @param searchedSquareCol column which is searched
 * @param searchedSquareRow row which is searched
 */
void searchForLargestSquareInCol(SQUARE *largestSquare, MATRIX *matrix, int searchedSquareCol, int searchedSquareRow) {
    for (int i = 0; i < searchedSquareRow; ++i) {
        if (matrix->values[i][searchedSquareCol] == '1') {
            POINT point = {i, searchedSquareCol};
            SQUARE possibleLargestSquare = findSquareFromPoint(point, matrix, largestSquare->size);

            // check if new square is larger than already found
            if (possibleLargestSquare.size > largestSquare->size) {
                *largestSquare = possibleLargestSquare;
            }
        }
    }
}

/**
 * Searches for the largest Square in row
 * if larger square is found it becomes the new largestSquare
 * @param largestSquare largest found square
 * @param matrix matrix to be searched for a larger square
 * @param searchedSquareCol column which is searched
 * @param searchedSquareRow row which is searched
 */
void searchForLargestSquareInRow(SQUARE *largestSquare, MATRIX *matrix, int searchedSquareCol, int searchedSquareRow) {
    for (int i = 0; i <= (searchedSquareCol)
         ; ++i) {
        if (matrix->values[searchedSquareRow][i] == '1') {
            POINT point = {searchedSquareRow, i};
            SQUARE possibleLargestSquare = findSquareFromPoint(point, matrix, largestSquare->size);

            // check if new square is larger than already found
            if (possibleLargestSquare.size > largestSquare->size) {
                *largestSquare = possibleLargestSquare;
            } else if (possibleLargestSquare.size == largestSquare->size &&
                       // if the square sizes are equal, checks if the new one is closer to the origin
                       squareCloserToTheOrigin(possibleLargestSquare, *largestSquare) == 1) {
                *largestSquare = possibleLargestSquare;
            }
        }
    }
}

/**
 * Checks if character is a white character
 * @param character charcter to be checked
 * @return 1 if character is a white character
 * @return 0 if character is not a white character
 */
int isWhiteChar(char character) {
    if((character >= FIRST_WHITE_CHAR_VAL
        && character <= LAST_WHITE_CHAR_VAL)
        || character == ' ') {
        return 1;
    }
    return 0;
}


//------------------------ Testing Functions ---------------------------------
// Function for printing the matrix
void printMatrix(MATRIX *matrix) {
    for (int i = 0; i < matrix->numOfRows; ++i) {
        for (int j = 0; j < matrix->numOfCols; ++j) {
            printf("%c ", matrix->values[i][j]);
        }
        printf("\n");
    }
}
