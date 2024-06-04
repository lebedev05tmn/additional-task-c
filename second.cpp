#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>

using namespace std;

struct Expense {
    string name;
    double amount;
    unordered_set<string> excluded;
};

void parseExpenseLine(const string &line, vector<Expense> &expenses) {
    istringstream iss(line);
    string name, amountStr, excludedStr;
    double amount;
    getline(iss, name, ':');
    getline(iss, amountStr, '/');
    amount = stod(amountStr);
    getline(iss, excludedStr);
    Expense expense({name, amount, {}});
    if (!excludedStr.empty()) {
        istringstream excl(excludedStr);
        string exclName;
        while (getline(excl, exclName, ',')) {
            expense.excluded.insert(exclName);
        }
    }
    expenses.push_back(expense);
}

int main() {
    setlocale(LC_ALL, "ru");
    ifstream file("input.txt");
    string line;
    vector<string> participants;
    vector<Expense> expenses;
    unordered_map<string, double> totalExpenses;
    unordered_map<string, double> shouldPay;
    unordered_map<string, double> actualPay;

    getline(file, line);
    istringstream iss(line);
    string participant;
    while (iss >> participant) {
        participants.push_back(participant);
        totalExpenses[participant] = 0;
        shouldPay[participant] = 0;
        actualPay[participant] = 0;
    }
    while (getline(file, line)) {
        parseExpenseLine(line, expenses);
    }
    for (const auto &expense : expenses) {
        actualPay[expense.name] += expense.amount;
        int includedCount = participants.size() - expense.excluded.size();
        double perPerson = expense.amount / includedCount;
        for (const auto &participant : participants) {
            if (expense.excluded.find(participant) == expense.excluded.end()) {
                shouldPay[participant] += perPerson;
            }
        }
    }
    cout << "Общие расходы и переводы:\n";
    for (const auto &participant : participants) {
        cout << participant << " трата: " << fixed << setprecision(2)
                  << actualPay[participant] << ", перевод: " << shouldPay[participant] << '\n';
    }

    cout << "\nОперации:\n";
    for (const auto &participant : participants) {
        if (actualPay[participant] < shouldPay[participant]) {
            for (const auto &other : participants) {
                if (actualPay[other] > shouldPay[other] && shouldPay[participant] - actualPay[participant] > 0) {
                    double amount = min(shouldPay[participant] - actualPay[participant], 
                                             actualPay[other] - shouldPay[other]);
                    cout << participant << " перевод " << other << ": " << amount << '\n';
                    actualPay[participant] += amount;
                    actualPay[other] -= amount;
                }
            }
        }
    }

    return 0;
}
