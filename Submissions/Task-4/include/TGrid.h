#pragma once

class TGrid {
public:
    int grid[100][100] = { 0 };

    TGrid() {
        srand(time(0));
        for (size_t i = 0; i < 100; ++i) {
            for (size_t j = 0; j < 100; ++j) {
                grid[i][j] = rand() % 10;
                //grid[i][j] = j;
            }
        }
    }

    int Get(TCoordinate coordinate) const {
        return grid[coordinate.GetX()][coordinate.GetY()];
    }

    void Dump() const {
        for (size_t i = 0; i < 100; ++i) {
            for (size_t j = 0; j < 100; ++j) {
                //printf("\033[38;5;%im%i ", grid[i][j]+1, grid[i][j]);
                printf("%i ", grid[i][j]); //^The version above colors the output, but that might pose an epelepsy risk.
            }
            printf("\n");
        }
        // printf("\033[0m");
    }
};