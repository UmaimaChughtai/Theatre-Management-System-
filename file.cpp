#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
using namespace std;

// Linked list node for food items
struct FoodNode {
    string name;
    int price;
    FoodNode* next;
    FoodNode(string n, int p) : name(n), price(p), next(nullptr) {}
};

// Linked list class for Food Menu
class FoodMenu {
private:
    FoodNode* head;
public:
    FoodMenu() : head(nullptr) {
        // Initialize with some items
        addFood("Popcorn", 150);
        addFood("Soda", 100);
        addFood("Nachos", 200);
        addFood("Chocolate", 120);
        addFood("Burger", 300);
    }

    void addFood(const string& name, int price) {
        FoodNode* newNode = new FoodNode(name, price);
        if (!head) {
            head = newNode;
        }
        else {
            FoodNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    void displayMenu() {
        cout << "\n🍔 Food Menu:\n";
        int index = 1;
        FoodNode* temp = head;
        while (temp) {
            cout << index << ". " << temp->name << " - Rs. " << temp->price << "\n";
            temp = temp->next;
            index++;
        }
        cout << "0. Finish order\n";
    }

    int getPriceByIndex(int index, string& name) {
        int count = 1;
        FoodNode* temp = head;
        while (temp) {
            if (count == index) {
                name = temp->name;
                return temp->price;
            }
            temp = temp->next;
            count++;
        }
        return -1; // invalid
    }
};

// Simple queue implementation for bookings using linked list
struct QueueNode {
    string customerName;
    QueueNode* next;
    QueueNode(string n) : customerName(n), next(nullptr) {}
};

class BookingQueue {
private:
    QueueNode* front;
    QueueNode* rear;
public:
    BookingQueue() : front(nullptr), rear(nullptr) {}

    void push(const string& name) {
        QueueNode* newNode = new QueueNode(name);
        if (!rear) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    string pop() {
        if (!front) return "";
        string name = front->customerName;
        QueueNode* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return name;
    }

    bool isEmpty() {
        return front == nullptr;
    }
};

struct Seat {
    int number;
    string status; // Available / Booked
    string customerName;
    string seatClass;
    // Store food ordered per seat
    vector<string> foodItems;
    vector<int> foodPrices;
};

struct Movie {
    string title;
    string hall;
    string time;
    map<int, Seat> seats;
};

class TheatreSystem {
private:
    map<string, Movie> movies;
    map<string, int> seatPrices;
    FoodMenu foodMenu;
    BookingQueue bookingQueue;

public:
    TheatreSystem() {
        seatPrices["Platinum"] = 1000;
        seatPrices["Gold"] = 700;
        seatPrices["Silver"] = 500;
    }

    bool isValidTimeFormat(const string& time) {
        if (time.length() < 5) return false;
        if (!(time.find("AM") != string::npos || time.find("PM") != string::npos)) return false;
        return true;
    }

    void addMovie() {
        Movie m;
        cout << "Enter movie title: ";
        cin.ignore();
        getline(cin, m.title);
        cout << "Enter hall name/number: ";
        getline(cin, m.hall);
        do {
            cout << "Enter show time (e.g., 7:00 PM): ";
            getline(cin, m.time);
            if (!isValidTimeFormat(m.time))
                cout << "❌ Invalid time format. Use AM/PM.\n";
        } while (!isValidTimeFormat(m.time));

        // initialize 30 seats with default values
        for (int i = 1; i <= 30; ++i) {
            Seat seat;
            seat.number = i;
            seat.status = "Available";
            seat.customerName = "";
            seat.seatClass = (i <= 10) ? "Platinum" : (i <= 20) ? "Gold" : "Silver";
            seat.foodItems.clear();
            seat.foodPrices.clear();
            m.seats[i] = seat;
        }
        movies[m.title] = m;
        cout << "✅ Movie added successfully.\n";
    }

    void displayMovies() {
        cout << "\n🎬 Available Movies:\n";
        for (auto& pair : movies) {
            cout << "- " << pair.first << " | Hall: " << pair.second.hall << " | Time: " << pair.second.time << "\n";
        }
    }

    void displaySeats(const string& movieTitle) {
        if (movies.find(movieTitle) == movies.end()) {
            cout << "❌ Movie not found.\n";
            return;
        }

        cout << "\nSeat Layout for " << movieTitle << ":\n";
        Movie& m = movies[movieTitle];

        for (int i = 1; i <= 30; ++i) {
            cout << "[" << i << "-" << m.seats[i].seatClass[0] << "-"
                << (m.seats[i].status == "Available" ? "A" : "B") << "] ";
            if (i % 10 == 0) cout << endl;
        }

        cout << "\nLegend: A=Available, B=Booked | P=Platinum, G=Gold, S=Silver\n";
    }

    void bookSeats() {
        string movieTitle;
        displayMovies();
        cout << "Enter movie title to book for: ";
        cin.ignore();
        getline(cin, movieTitle);

        if (movies.find(movieTitle) == movies.end()) {
            cout << "❌ Movie not found.\n";
            return;
        }

        displaySeats(movieTitle);
        int numSeats;
        cout << "How many seats do you want to book? ";
        cin >> numSeats;
        vector<int> seatsToBook;
        for (int i = 0; i < numSeats; ++i) {
            int seatNum;
            cout << "Enter seat number #" << (i + 1) << ": ";
            cin >> seatNum;
            if (seatNum < 1 || seatNum > 30) {
                cout << "❌ Invalid seat number. Choose between 1 and 30.\n";
                i--;
                continue;
            }
            if (movies[movieTitle].seats[seatNum].status == "Booked") {
                cout << "❌ Seat " << seatNum << " already booked. Choose another.\n";
                i--;
                continue;
            }
            seatsToBook.push_back(seatNum);
        }

        cin.ignore();
        string name;
        cout << "Enter your name: ";
        getline(cin, name);

        for (int seatNum : seatsToBook) {
            cout << "\nDo you want to order food for seat " << seatNum << "? (Y/N): ";
            char orderFood;
            cin >> orderFood;
            cin.ignore();

            vector<string> foodItems;
            vector<int> foodPrices;

            if (orderFood == 'Y' || orderFood == 'y') {
                bool ordering = true;
                while (ordering) {
                    foodMenu.displayMenu();
                    cout << "Select food item number (or 0 to finish): ";
                    int choice;
                    cin >> choice;
                    cin.ignore();

                    if (choice == 0) {
                        ordering = false;
                    }
                    else {
                        string itemName;
                        int price = foodMenu.getPriceByIndex(choice, itemName);
                        if (price > 0) {
                            foodItems.push_back(itemName);
                            foodPrices.push_back(price);
                            cout << "Added " << itemName << " (Rs. " << price << ") to order.\n";
                        }
                        else {
                            cout << "❌ Invalid food choice.\n";
                        }
                    }
                }
            }

            // Book the seat
            Seat& s = movies[movieTitle].seats[seatNum];
            s.status = "Booked";
            s.customerName = name;
            s.foodItems = foodItems;
            s.foodPrices = foodPrices;

            int seatPrice = seatPrices[s.seatClass];
            int totalFoodPrice = 0;
            for (int p : foodPrices) totalFoodPrice += p;

            cout << "\n✅ Seat " << seatNum << " (" << s.seatClass << ") booked for " << name << endl;
            cout << "Seat Price: Rs. " << seatPrice << endl;
            if (!foodItems.empty()) {
                cout << "Food Ordered:\n";
                for (size_t i = 0; i < foodItems.size(); ++i) {
                    cout << " - " << foodItems[i] << ": Rs. " << foodPrices[i] << endl;
                }
            }
            cout << "Total Bill for this seat: Rs. " << seatPrice + totalFoodPrice << endl;
        }

        bookingQueue.push(name);
    }

    void cancelSeat() {
        string movieTitle;
        displayMovies();
        cout << "Enter movie title: ";
        cin.ignore();
        getline(cin, movieTitle);

        if (movies.find(movieTitle) == movies.end()) {
            cout << "❌ Movie not found.\n";
            return;
        }

        int seatNum;
        cout << "Enter seat number to cancel: ";
        cin >> seatNum;

        if (seatNum < 1 || seatNum > 30) {
            cout << "❌ Invalid seat number. Choose between 1 and 30.\n";
            return;
        }

        Seat& s = movies[movieTitle].seats[seatNum];
        if (s.status == "Available") {
            cout << "❌ Seat is already available.\n";
            return;
        }

        cout << "Cancelling seat " << seatNum << " booked by " << s.customerName << ".\n";
        s.status = "Available";
        s.customerName = "";
        s.foodItems.clear();
        s.foodPrices.clear();
        cout << "✅ Booking cancelled.\n";
    }

    void summary() {
        cout << "\n🎟 Booking Summary:\n";
        for (auto& moviePair : movies) {
            Movie& m = moviePair.second;
            cout << "\nMovie: " << m.title << " | Hall: " << m.hall << " | Time: " << m.time << "\n";
            for (int i = 1; i <= 30; ++i) {
                if (m.seats[i].status == "Booked") {
                    cout << "Seat " << i << " - " << m.seats[i].seatClass
                        << " - " << m.seats[i].customerName << endl;
                    if (!m.seats[i].foodItems.empty()) {
                        cout << "  Food Ordered:\n";
                        for (size_t j = 0; j < m.seats[i].foodItems.size(); ++j) {
                            cout << "    * " << m.seats[i].foodItems[j] << " (Rs. " << m.seats[i].foodPrices[j] << ")\n";
                        }
                    }
                }
            }
        }
    }
};

int main() {
    TheatreSystem system;
    int choice;

    while (true) {
        cout << "\n--- Advanced Theatre Management System ---\n";
        cout << "1. Add Movie (Admin)\n2. Display Movies\n3. View Seats\n4. Book Seats\n5. Cancel Seat\n6. View Summary\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: system.addMovie(); break;
        case 2: system.displayMovies(); break;
        case 3: {
            string title;
            cin.ignore();
            cout << "Enter movie title: ";
            getline(cin, title);
            system.displaySeats(title);
            break;
        }
        case 4: system.bookSeats(); break;
        case 5: system.cancelSeat(); break;
        case 6: system.summary(); break;
        case 7: cout << "Goodbye!\n"; return 0;
        default: cout << "Invalid choice!\n";
        }
    }
}