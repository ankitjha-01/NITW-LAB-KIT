#include <bits/stdc++.h>
using namespace std;

int main() {
    bool channelBusy;

    for (int i = 0; i < 5; i++) {
        channelBusy = rand() % 2;

        if (!channelBusy) {
            cout << "Sending data\n";
        } else {
            cout << "Channel busy, waiting\n";
        }
    }
}