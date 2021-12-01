#include <vector>

int backtrack = 0;


struct Pos
{
    int x, y;
};

struct EmptyCell
{
    Pos m_pos;
    std::vector<int> m_possibleValues;
};

std::vector<Pos> Deduce(std::vector<std::vector<int>> &sudoku)
{
    // Find the possible numbers in each cell e.g. {3, 5} based on current state of the sudoku
    // If there exists only one possible answer, then we have solved that cell

    bool hasNewImplication;
    std::vector<Pos> implications {};

    do
    {
        std::vector<EmptyCell> emptyCells {};
        hasNewImplication = false;
    
        // For each sector, calculate a set of possible numbers for each cell
        // by removing existing digit in the sector from 1-9

        for (int sector = 0; sector < 9; ++sector)
        {
            std::vector<int> set{ 1,2,3,4,5,6,7,8,9 };

            // Map sector value to the starting x y (top left cell) of each sector
            int secX = (sector / 3) * 3;
            int secY = sector * 3 % 9; 
            for (int row = secX; row < secX + 3; ++row)
            {
                for (int col = secY; col < secY + 3; ++col)
                {
                    if (sudoku[row][col] != 0)
                    {
                        auto iter = std::find(set.begin(), set.end(), sudoku[row][col]);
                        if (iter != set.end())
                        {
                            set.erase(iter);
                        }
                        
                    }
                }
            }

            for (int row = secX; row < secX + 3; ++row)
            {
                for (int col = secY; col < secY + 3; ++col)
                {
                    if (sudoku[row][col] == 0)
                    {
                        emptyCells.emplace_back(EmptyCell{ { row,col }, set });
                    }
                }
            }

            for (EmptyCell &cell : emptyCells)
            {
                // Horizontal Scan remove all existing digit from cell's possible digit
                for (int i = 0; i < 9; ++i)
                {
                    int value = sudoku[cell.m_pos.x][i];
                    if (value != 0)
                    {
                        auto iter = std::find(cell.m_possibleValues.begin(), cell.m_possibleValues.end(), value);
                        if (iter != cell.m_possibleValues.end())
                        {
                            cell.m_possibleValues.erase(iter);
                        }
                    }
                }

                // Vertical Scan remove all existing digit from cell's possible digit
                for (int i = 0; i < 9; ++i)
                {
                    if (sudoku[i][cell.m_pos.y] != 0)
                    {
                        auto iter = std::find(cell.m_possibleValues.begin(), cell.m_possibleValues.end(), sudoku[i][cell.m_pos.y]);
                        if (iter != cell.m_possibleValues.end())
                        {
                            cell.m_possibleValues.erase(iter);
                        }
                    }
                }

                // Check if the set is a singleton
                if (cell.m_possibleValues.size() == 1)
                {
                    sudoku[cell.m_pos.x][cell.m_pos.y] = cell.m_possibleValues[0];
                    implications.push_back({ cell.m_pos.x, cell.m_pos.y});
                    hasNewImplication = true;
                }
            }

        }

       

    } while (hasNewImplication);

    return implications;
}

void UndoImplications(std::vector<std::vector<int>> &sudoku, const std::vector<Pos> &implications)
{
    for (auto impl : implications)
    {
        sudoku[impl.x][impl.y] = 0;
    }
}

Pos GetNextEmptyCell(const std::vector<std::vector<int>> &sudoku, int x, int y)
{
    
    x = x % 9;
    y = y % 9;

    for (; x < 9; ++x)
    {
        for (; y < 9; ++y)
        {
            if (sudoku[x][y] == 0)
            {
                return { x,y };
            }
        }
        y = 0;
    }

    return { -1,-1 };

}
bool Validate(const std::vector<std::vector<int>> &sudoku, int x, int y, int e)
{
    // Check row
    for (int i = 0; i < 9; ++i)
    {
        if (e == sudoku[x][i])
        {
            return false;
        }
    }

    // Check col
    for (int j = 0; j < 9; ++j)
    {
        if (e == sudoku[j][y])
        {
            return false;
        }
    }
   

    // Check section
    int secTopX = 3 * (x / 3);
    int secTopY = 3 * (y / 3);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (e == sudoku[secTopX + i][secTopY + j])
            {
                return false;
            }
        }
    }

    return true;
   
}


bool Solve(std::vector<std::vector<int>> &sudoku, int x = 0, int y = 0)
{
    Deduce(sudoku);

    // Next cell to fill
    int nextX = GetNextEmptyCell(sudoku, x, y).x;
    int nextY = GetNextEmptyCell(sudoku, x, y).y;
    if (nextX == -1)
    {
        return true;
    }


    for (int e = 1; e < 10; ++e)
    {
        if (Validate(sudoku, nextX, nextY, e))
        {
            sudoku[nextX][nextY] = e;
            auto implications = Deduce(sudoku);

            if (Solve(sudoku, nextX, nextY))
            {
                return true;
            }

            sudoku[nextX][nextY] = 0;   // We need to reset the value, if 1-9 are all invalid
            UndoImplications(sudoku, implications);
            backtrack++;
        }
    }
	return false;
}

int main()
{
    std::vector<std::vector<int>> sudoku = {
        {0, 0, 5, 3, 0, 0, 0, 0, 0},
        {8, 0, 0, 0, 0, 0, 0, 2, 0},
        {0, 7, 0, 0, 1, 0, 5, 0, 0},
        {4, 0, 0, 0, 0, 5, 3, 0, 0},
        {0, 1, 0, 0, 7, 0, 0, 0, 6},
        {0, 0, 3, 2, 0, 0, 0, 8, 0},
        {0, 6, 0, 5, 0, 0, 0, 0, 9},
        {0, 0, 4, 0, 0, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 9, 7, 0, 0}
    };

    if (Solve(sudoku))
    {
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                printf("%d ", sudoku[i][j]);
            }
            printf("\n");
        }
    }

    printf("Backtracks: %d", backtrack);



}