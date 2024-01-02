#include <bits/stdc++.h>

using namespace std;

const int arrSize = 30;
bool usable = false;
int counter = 0;
int arr[arrSize] = { 0 };
float average = 0.0;
int i = 0;

int main() {
    while(true) {
        int data = 0;
        cout << "Enter a number to put into array: ";
        cin >> data;
        if(counter < arrSize) {
            i = counter % arrSize;
            arr[i] = data;
            counter++;
            int total = 0;
            for(int val = 0; val < counter; val++) {
                cout << arr[val] << " ";
                total += arr[val];
            } 
            cout << endl;
            average = total * 1.0 / counter;
            cout << average << endl;
        } 
        else {
            usable = true;
            i = counter % arrSize;
            average -= arr[i];
            arr[i] = data;
            counter++;
            int total = 0;
            for(int val = 0; val < arrSize; val++) {
                cout << arr[val] << " ";
                total += arr[val];
            } 
            cout << endl;
            average = total * 1.0 / arrSize;
            cout << average << endl;
        }
    }
}