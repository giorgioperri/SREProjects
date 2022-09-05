#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include <iostream>
#include <cmath>

using namespace std;

int nearestPerfectSquare(int N) {
        int sr = sqrt(N);

        int a = sr * sr;
        int b = (sr + 1) * (sr + 1);

        if (sr * sr == N) return a;
        return b;
}

int main() {

    string input;
    cout << "Enter message to be encrypted:\n";
    std::cin >> input;

    int stringLength = input.length();
    int closestSquare = nearestPerfectSquare(stringLength);
    int rectSize = sqrt(closestSquare);

    string secretMessage = "";

    for (int j = 0; j < rectSize; ++j) {

        for (int i = j; i < closestSquare; i+=rectSize) {
            if(input[i]) secretMessage += input[i];
        }

        secretMessage += " ";
    }

    cout << secretMessage << endl;

    int x;
    std::cin >> x;

    return 0;
}
#pragma clang diagnostic pop