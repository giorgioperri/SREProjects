#include <iostream>
#define date() __DATE__
#define time() __TIME__

int main() {
    using namespace std;

    int x; // waits for user input. Workaround for not closing window on Windows
    cout << date() << ", " << time() << endl;
    std::cin >> x;
    return 0;
}