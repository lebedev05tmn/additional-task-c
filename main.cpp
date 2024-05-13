#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

struct Cell {
    bool hasBacteria;

    Cell() : hasBacteria(false) {}
};

struct Result {
    int averageDays;
    int maxDays;
    int minDays;
    double notFilledProbability;

    Result() : averageDays(0), maxDays(0), minDays(0), notFilledProbability(0.0) {}
};

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

void populateField(vector<vector<Cell> >& field, int D, int K1, int K2) {
    int N = field.size();
    for (int day = 0; day < D; day++) {
        for (int k = 0; k < getRandomNumber(K1, K2); k++) {
            int x = getRandomNumber(0, N - 1);
            int y = getRandomNumber(0, N - 1);
            field[x][y].hasBacteria = true;
        }
    }
}


void reproduceBacteria(vector<vector<Cell> >& field, double P_plus) {
    int N = field.size();
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            if (field[x][y].hasBacteria) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < N && ny >= 0 && ny < N && !field[nx][ny].hasBacteria) {
                            if ((double)rand() / RAND_MAX < P_plus) {
                                field[nx][ny].hasBacteria = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

Result experiment(int size, int days, int life_cycle, int min_bacteria, int max_bacteria, double coef_push, double coef_pop) {
    int totalExperiments = 1000; // Количество экспериментов

    int sumDays = 0;
    int maxDays = INT_MIN;
    int minDays = INT_MAX;
    int notFilledCount = 0;

    for (int i = 0; i < totalExperiments; ++i) {
        vector<vector<Cell> > field(size, vector<Cell>(size, Cell()));
        populateField(field, days, min_bacteria, max_bacteria);
        int currentDays = 0;

        while (currentDays <= life_cycle) {
            reproduceBacteria(field, coef_push);

            bool isFilled = true;
            for (int x = 0; x < size && isFilled; ++x) {
                for (int y = 0; y < size; ++y) {
                    if (!field[x][y].hasBacteria) {
                        isFilled = false;
                        break;
                    }
                }
                if (!isFilled) break;
            }

            if (isFilled) {
                sumDays += currentDays;
                maxDays = max(maxDays, currentDays);
                minDays = min(minDays, currentDays);
                break;
            }

            if (currentDays == life_cycle) {
                notFilledCount++;
            }

            currentDays++;
        }
    }

    Result result;
    result.averageDays = sumDays / totalExperiments > 0 ? sumDays / totalExperiments : (maxDays+minDays)/2;
    result.maxDays = maxDays;
    result.minDays = minDays;
    result.notFilledProbability = static_cast<double>(notFilledCount) / totalExperiments;

    return result;
}

void firstTask(int days, int life_cycle) {
    vector<int> sizes;
    sizes.push_back(5);
    sizes.push_back(10);
    sizes.push_back(15);

    vector<int> min_bacteria_values;
    min_bacteria_values.push_back(1);
    min_bacteria_values.push_back(2);
    min_bacteria_values.push_back(3);

    vector<int> max_bacteria_values;
    max_bacteria_values.push_back(4);
    max_bacteria_values.push_back(5);
    max_bacteria_values.push_back(6);

    int minDays = INFINITY;
    int maxDays = -INFINITY;
    int sumDays = 0;
    int countDays = 0;

    for (int size : sizes) {
        for (int min_bacteria : min_bacteria_values) {
            for (int max_bacteria : max_bacteria_values) {
                Result result = experiment(size, days, life_cycle, min_bacteria, max_bacteria, 0.05, 0.02);
                cout << "N = " << size << ", K1 = " << min_bacteria << ", K2 = " << max_bacteria << endl;
                cout << "Porablity: " << result.notFilledProbability << endl;
                cout << "Min days: " << result.minDays << endl;
                cout << "Max days: " << result.maxDays << endl;
                cout << "Average days: " << result.averageDays << endl;
                cout << "----------------------" << endl;
            }
        }
    }
}


 void secondTask(int days, int life_cycle) {
    vector<int> sizes;
    sizes.push_back(5);
    sizes.push_back(10);
    sizes.push_back(15);

    vector<int> min_bacteria_values;
    min_bacteria_values.push_back(1);
    min_bacteria_values.push_back(2);
    min_bacteria_values.push_back(3);

    vector<int> max_bacteria_values;
    max_bacteria_values.push_back(4);
    max_bacteria_values.push_back(5);
    max_bacteria_values.push_back(6);

    for (int size : sizes) {
        for (int min_bacteria : min_bacteria_values) {
            for (int max_bacteria : max_bacteria_values) {
                int totalExperiments = 1000;
                int sumDays = 0;
                vector<pair<int, int> > bestPositions;
                int bestAverageDays = INT_MAX;

                for (int i = 0; i < totalExperiments; ++i) {
                    vector<vector<Cell> > field(size, vector<Cell>(size, Cell()));

                    // Определение позиций поля для размещения бактерий
                    int center_x = size / 2;
                    int center_y = size / 2;

                    // Определение случайного смещения относительно центра
                    int offset_x = getRandomNumber(-2, 2);
                    int offset_y = getRandomNumber(-2, 2);

                    int x = center_x + offset_x;
                    int y = center_y + offset_y;

                    // Убедимся, что координаты находятся в пределах поля
                    x = max(0, min(size - 1, x));
                    y = max(0, min(size - 1, y));

                    // Размещение бактерий в позиции (x, y)
                    field[x][y].hasBacteria = true;

                    int currentDays = 0;

                    while (currentDays <= life_cycle) {
                        // Репликация бактерий
                        reproduceBacteria(field, 1.0);

                        // Проверка, заполнено ли поле
                        bool isFilled = true;
                        for (int x = 0; x < size && isFilled; ++x) {
                            for (int y = 0; y < size; ++y) {
                                if (!field[x][y].hasBacteria) {
                                    isFilled = false;
                                    break;
                                }
                            }
                        }

                        if (isFilled) {
                            sumDays += currentDays;
                            break;
                        }

                        ++currentDays;
                    }

                    // Проверяем, является ли текущее решение лучше предыдущих
                    if (currentDays < bestAverageDays) {
                        bestAverageDays = currentDays;
                        bestPositions.clear();
                        bestPositions.push_back(make_pair(x, y));
                    } else if (currentDays == bestAverageDays) {
                        bestPositions.push_back(make_pair(x, y));
                    }
                }

                // Вывод лучших позиций
                cout << "N = " << size << ", K1 = " << min_bacteria << ", K2 = " << max_bacteria << endl;
                cout << "best position" << endl;
                for (const auto& pos : bestPositions) {
                    cout << "(" << pos.first << ", " << pos.second << ")" << endl;
                }
                cout << "fill the field: " << bestAverageDays << " days" << endl;
                cout << "----------------------" << endl;
            }
        }
    }
}





int main() {
    setlocale(LC_ALL, "ru_mac");
    srand(time(0));

    int days1 = 7;
    int life_cycle1 = 14;

    cout << "First task" << endl;
    cout << "----------------------" << endl;
    cout << "---------------------" << endl;
    cout << "--------------------" << endl;

    firstTask(days1, life_cycle1);

    int days2 = 1;
    int life_cycle2 = INFINITY;

    cout << "Second task" << endl;
    cout << "----------------------" << endl;
    cout << "---------------------" << endl;
    cout << "--------------------" << endl;

    secondTask(days2, life_cycle2);


    return 0;
}
