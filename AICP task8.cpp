#include <iostream>

using namespace std;

const int NUM_BOATS = 10;
const int OPENING_HOUR = 10;
const int CLOSING_HOUR = 17;
const double HOURLY_RATE = 20.0;
const double HALF_HOUR_RATE = 12.0;

struct Boat {
    int boatNumber;
    double moneyTaken;
    double totalHoursHired;
    int returnHour;
    bool isAvailable;
};

// Function prototypes
void hireBoat(Boat boats[]);
void bubbleSort(Boat boats[]);
int findNextAvailableBoat(const Boat boats[]);
int findNextAvailableTime(const Boat boats[], int currentHour);
void displayAvailableBoats(const Boat boats[]);
double calculateCost(double hireDuration);
void generateDailyReport(const Boat boats[]);

int main() {
    Boat boats[NUM_BOATS];

    // Initialize boats
    for (int i = 0; i < NUM_BOATS; ++i) {
        boats[i].boatNumber = i + 1;
        boats[i].moneyTaken = 0.0;
        boats[i].totalHoursHired = 0.0;
        boats[i].returnHour = OPENING_HOUR;
        boats[i].isAvailable = true;
    }

    int choice;
    do {
        cout << "\nMenu:\n1. Hire a boat\n2. Check for available boats\n3. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                hireBoat(boats);
                break;
            case 2:
                displayAvailableBoats(boats);
                break;
            case 3:
                generateDailyReport(boats);
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 3);

    return 0;
}

void hireBoat(Boat boats[]) {
    int availableBoatIndex = findNextAvailableBoat(boats);

    if (availableBoatIndex == -1) {
        // If no boats are available, sort the boats by return time
        bubbleSort(boats);

        // Find the first available return time
        int nextAvailableTime = findNextAvailableTime(boats, OPENING_HOUR);

        if (nextAvailableTime == -1) {
            cerr << "Error: No boats are currently available, and the next available time is unknown.\n";
            return;
        }

        cerr << "Error: No boats are currently available. The first available boat will be at " << nextAvailableTime << ":00.\n";
        return;
    }

    int boatIndex;
    cout << "Enter boat number (1 to " << NUM_BOATS << "): ";
    cin >> boatIndex;

    if (boatIndex < 1 || boatIndex > NUM_BOATS) {
        cerr << "Error: Invalid boat number. Must be between 1 and " << NUM_BOATS << ".\n";
        return;
    }

    if (!boats[boatIndex - 1].isAvailable) {
        cerr << "Error: Boat " << boats[boatIndex - 1].boatNumber << " is not available at this time.\n";
        return;
    }

    int startTime;
    double hireDuration;  // Change the type to double

    cout << "Enter start time (between 10 and 17): ";
    cin >> startTime;

    if (startTime < OPENING_HOUR || startTime >= CLOSING_HOUR) {
        cerr << "Error: Invalid start time. Must be between 10 and 17.\n";
        return;
    }

    cout << "Enter hire duration (in hours, e.g., 1 or 0.5 for half-hour): ";
    cin >> hireDuration;

    if (hireDuration <= 0 || startTime + hireDuration > CLOSING_HOUR) {
        cerr << "Error: Invalid hire duration or return time exceeds 17:00.\n";
        return;
    }

    double payment = calculateCost(hireDuration);
    boats[boatIndex - 1].moneyTaken += payment;
    boats[boatIndex - 1].totalHoursHired += hireDuration;
    boats[boatIndex - 1].returnHour = static_cast<int>(startTime + hireDuration);
    boats[boatIndex - 1].isAvailable = false;

    cout << "Boat " << boats[boatIndex - 1].boatNumber << " hired successfully. Payment: $" << payment << "\n";
    
}

void bubbleSort(Boat boats[]) {
    for (int i = 0; i < NUM_BOATS - 1; ++i) {
        for (int j = 0; j < NUM_BOATS - i - 1; ++j) {
            if (boats[j].returnHour > boats[j + 1].returnHour) {
                // Swap boats[j] and boats[j + 1]
                Boat temp = boats[j];
                boats[j] = boats[j + 1];
                boats[j + 1] = temp;
            }
        }
    }
}

int findNextAvailableBoat(const Boat boats[]) {
    for (int i = 0; i < NUM_BOATS; ++i) {
        if (boats[i].isAvailable) {
            return i;
        }
    }
    return -1;  // No available boats
}

int findNextAvailableTime(const Boat boats[], int currentHour) {
    for (int hour = currentHour; hour < CLOSING_HOUR; ++hour) {
        for (int i = 0; i < NUM_BOATS; ++i) {
            if (hour >= boats[i].returnHour) {
                return hour;
            }
        }
    }
    return -1;  // No available time
}

void displayAvailableBoats(const Boat boats[]) {
    cout << "\nAvailable Boats:\n";
    for (int i = 0; i < NUM_BOATS; ++i) {
        cout << "Boat " << boats[i].boatNumber << ": ";
        if (boats[i].isAvailable) {
            cout << "Available\n";
        } else {
            cout << "Not Available (Return time: " << boats[i].returnHour << ":";
            // Adjust the display for half-hour hires
            if (boats[i].totalHoursHired - static_cast<int>(boats[i].totalHoursHired) == 0.5) {
                cout << "30";
            } else {
                cout << "00";
            }
            cout << ")\n";
        }
    }
}

double calculateCost(double hireDuration) {
    if (hireDuration >= 1) {
        return hireDuration * HOURLY_RATE;
    } else {
        return (hireDuration+0.5) * HALF_HOUR_RATE;
    }
}

void generateDailyReport(const Boat boats[]) {
    double totalMoneyTaken = 0.0;
    double totalHoursHired = 0.0;
    int unusedBoats = 0;
    int maxUsedBoatIndex = 0;

    for (int i = 0; i < NUM_BOATS; ++i) {
        totalMoneyTaken += boats[i].moneyTaken;
        totalHoursHired += boats[i].totalHoursHired;

        if (boats[i].totalHoursHired == 0) {
            unusedBoats++;
        }

        if (boats[i].totalHoursHired > boats[maxUsedBoatIndex].totalHoursHired) {
            maxUsedBoatIndex = i;
        }
    }

    cout << "\n--- Daily Report ---\n";
    cout << "Total Money Taken: $" << totalMoneyTaken << "\n";
    cout << "Total Hours Hired: " << totalHoursHired << " hours\n";
    cout << "Number of Boats Not Used: " << unusedBoats << "\n";
    cout << "Boat Used the Most: Boat " << boats[maxUsedBoatIndex].boatNumber << " (Total Hours: " << boats[maxUsedBoatIndex].totalHoursHired << ")\n";
}

