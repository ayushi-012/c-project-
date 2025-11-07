#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

// ===================== CLASS & OBJECT + CONSTRUCTORS + DESTRUCTOR =====================
class Food {
    int id;
    string name;
    float price;
public:
    Food(int i=0, string n="", float p=0) { id=i; name=n; price=p; }
    void show() const { cout << id << ". " << name << " - Rs." << price << endl; }
    float getPrice() const { return price; }
    string getName() const { return name; }
    int getId() const { return id; }
    ~Food() { /* destructor example */ }
};

// ===================== INHERITANCE =====================
class Customer {
public:
    virtual void info() { cout << "Customer Info" << endl; }
};

class OnlineCustomer : public Customer {
    string name;
public:
    OnlineCustomer(string n){ name=n; }
    void info() override { cout << "Customer: " << name << endl; }
};

// ===================== OPERATOR OVERLOADING =====================
class DeliveryTime {
    int minutes;
public:
    DeliveryTime(int m=0){ minutes=m; }
    DeliveryTime operator+(DeliveryTime d){ return DeliveryTime(minutes + d.minutes); }
    void show(){ cout << "Estimated Delivery Time: " << minutes << " mins" << endl; }
};

// ===================== FUNCTION OVERLOADING =====================
class Bill {
    float total;
public:
    Bill(){ total=0; }
    void add(float price){ total += price; }
    void add(float price, int qty){ total += price * qty; }
    float getTotal(){ return total; }
};

// ===================== STATIC MEMBER =====================
class Order {
    static int totalOrders;
    Bill bill;
public:
    Order(){ totalOrders++; }
    void addItem(Food f, int q){ bill.add(f.getPrice(), q); }
    float getBill(){ return bill.getTotal(); }
    static void showCount(){ cout << "Total Orders: " << totalOrders << endl; }
};
int Order::totalOrders = 0;

// ===================== FRIEND FUNCTION =====================
class Offer {
    float discount;
public:
    Offer(float d){ discount=d; }
    friend float applyOffer(Offer o, float total);
};
float applyOffer(Offer o, float total){ return total - (total * o.discount / 100); }

// ===================== TEMPLATE FUNCTION =====================
template<typename T>
T addTax(T price){ return price + (price * 0.05); }

// ===================== EXCEPTION HANDLING =====================
void checkQty(int q){
    if(q <= 0) throw invalid_argument("Invalid Quantity Entered!");
}

// ===================== FILE HANDLING =====================
void saveBillToFile(string name, float total){
    ofstream fout("bill.txt", ios::app);
    fout << "Customer: " << name << " | Total Bill: Rs." << total << endl;
    fout.close();
}

// ===================== MAIN PROGRAM =====================
int main(){
    try{
        // Static Array (Static Memory Allocation)
        Food menu[5] = {
            {1,"Pizza",200},
            {2,"Burger",120},
            {3,"Pasta",150},
            {4,"Sandwich",100},
            {5,"Coffee",80}
        };

        cout << "======== WELCOME TO FOOD DELIVERY SYSTEM ========\n";
        cout << "\n------ MENU ------\n";
        for(int i=0;i<5;i++) menu[i].show();

        string cname;
        cout << "\nEnter your name: ";
        getline(cin, cname);
        OnlineCustomer cust(cname);
        cust.info();

        Order* order = new Order(); // Dynamic memory allocation

        char again;
        do {
            int id, qty;
            cout << "\nEnter food ID to order: ";
            cin >> id;
            cout << "Enter quantity: ";
            cin >> qty;

            checkQty(qty); // may throw exception

            bool found = false;
            for(int i=0;i<5;i++){
                if(menu[i].getId() == id){
                    order->addItem(menu[i], qty);
                    cout << qty << " " << menu[i].getName() << "(s) added to your order.\n";
                    found = true;
                    break;
                }
            }
            if(!found) cout << "Invalid choice!\n";

            cout << "Do you want to order more? (y/n): ";
            cin >> again;
        } while(again=='y' || again=='Y');

        float total = order->getBill();
        cout << "\nYour Bill Before Offer & Tax: Rs." << total << endl;

        // Apply 10% discount and 5% tax using template + friend function
        Offer off(10);
        total = applyOffer(off, addTax(total));
        cout << "After 10% offer & tax: Rs." << total << endl;

        // Operator Overloading Example
        DeliveryTime t1(20), t2(15);
        DeliveryTime totalTime = t1 + t2;
        totalTime.show();

        // Save to file
        saveBillToFile(cname, total);

        Order::showCount();

        delete order; // Free dynamically allocated memory
        cout << "\nThank you for ordering! Your bill has been saved.\n";

    } catch(exception &e){
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
