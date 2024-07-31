#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

using namespace std;


class BaseMedication {
public:
    virtual ~BaseMedication() = default;
    
    
    virtual string getName() const = 0;
    virtual int getQuantity() const = 0;
    virtual float getPrice() const = 0;

    
    virtual void dispense(int amount) = 0;
    virtual void saveToFile(ofstream &outFile) = 0;
};


class GenericMedication : public BaseMedication {
private:
    string name;
    int quantity;
    float price;

public:
    GenericMedication(string _name, int _quantity, float _price) : name(_name), quantity(_quantity), price(_price) {}

    // Getters
    string getName() const override { return name; }
    int getQuantity() const override { return quantity; }
    float getPrice() const override { return price; }

    // Setters
    void setName(string _name) { name = _name; }
    void setQuantity(int _quantity) { quantity = _quantity; }
    void setPrice(float _price) { price = _price; }

    // Dispense medication
    void dispense(int amount) override {
        if (quantity >= amount) {
            quantity -= amount;
            cout << amount << " " << name << " dispensed." << endl;
        } else {
            cout << "Not enough " << name << " in stock." << endl;
        }
    }

    // Save Medication data to file
    void saveToFile(ofstream &outFile) override {
        outFile << name << "," << quantity << "," << price << "\n";
    }

    // Load Medication data from file
    static GenericMedication loadFromFile(const string &line) {
        stringstream ss(line);
        string name;
        int quantity;
        float price;

        getline(ss, name, ',');
        ss >> quantity;
        ss.ignore();
        ss >> price;

        return GenericMedication(name, quantity, price);
    }
};

// Base class for Symptom
class Symptom {
private:
    string symptom;

public:
    Symptom(string _symptom) : symptom(_symptom) {}

    // Getter
    string getSymptom() const { return symptom; }
};

// Derived class from Symptom to associate symptoms with medications
class SymptomToMedication : public Symptom {
private:
    map<string, string> symptomMedicationMap;

public:
    SymptomToMedication() : Symptom("") {
      
        symptomMedicationMap = {
            {"headache", "Aspirin"},
            {"fever", "Paracetamol"},
            {"cough", "Cough Syrup"},
            {"sore throat", "Lozenges"},
            {"pain", "Ibuprofen"},
            {"heartburn", "Antacid"},
            {"allergy", "Antihistamine"},
            {"infection", "Antibiotic"}
            
        };
    }

    // Function to get medication name based on the symptom
    string getMedicationName(const string &symptom) {
        auto it = symptomMedicationMap.find(symptom);
        if (it != symptomMedicationMap.end()) {
            return it->second;
        }
        return "";
    }
};

// Derived class from SymptomToMedication for Pharmacy functionality
class Pharmacy : public SymptomToMedication {
private:
    vector<BaseMedication*> inventory;  // Use polymorphism with BaseMedication*
    string inventoryFile = "inventory.txt";

public:
    // Load inventory from file
    void loadInventory() {
        ifstream inFile(inventoryFile);
        if (!inFile.is_open()) {
            cout << "Error: Unable to open inventory file.\n";
            return;
        }

        // Clear current inventory
        for (auto& med : inventory) {
            delete med;
        }
        inventory.clear();

        string line;
        while (getline(inFile, line)) {
            BaseMedication* med = new GenericMedication(GenericMedication::loadFromFile(line));
            inventory.push_back(med);
        }

        inFile.close();
    }

    // Save inventory to file
    void saveInventory() {
        ofstream outFile(inventoryFile);
        if (!outFile.is_open()) {
            cout << "Error: Unable to open inventory file for writing.\n";
            return;
        }

        for (const auto &med : inventory) {
            med->saveToFile(outFile);
        }

        outFile.close();
    }

    // Function to add medication to inventory
    void addMedication(BaseMedication* med) {
        inventory.push_back(med);
    }

    // Function to search for medication by name
    BaseMedication* findMedication(string name) {
        for (auto& med : inventory) {
            if (med->getName() == name) {
                return med;
            }
        }
        return NULL;
    }

    // Function to check if medication is available
    bool isMedicationAvailable(string name) {
        return findMedication(name) != NULL;
    }

    // Function to display all medications in inventory
    void displayInventory() {
        cout << "Inventory:" << endl;
        for (const auto& med : inventory) {
            cout << med->getName() << " - Quantity: " << med->getQuantity() << " - Price: $" << med->getPrice() << endl;
        }
    }

    // Function to update medication quantity
    void updateMedicationQuantity(string name, int quantity) {
        BaseMedication* med = findMedication(name);
        if (med != NULL) {
            GenericMedication* genMed = dynamic_cast<GenericMedication*>(med);
            if (genMed) {
                genMed->setQuantity(quantity);
                cout << "Updated quantity of " << name << " to " << quantity << "." << endl;
            }
        } else {
            cout << "Medication not found." << endl;
        }
    }

    // Function to update medication price
    void updateMedicationPrice(string name, float price) {
        BaseMedication* med = findMedication(name);
        if (med != NULL) {
            GenericMedication* genMed = dynamic_cast<GenericMedication*>(med);
            if (genMed) {
                genMed->setPrice(price);
                cout << "Updated price of " << name << " to $" << price << "." << endl;
            }
        } else {
            cout << "Medication not found." << endl;
        }
    }

    // Function to process a prescription and generate bill
    
void processPrescription() {
    map<string, int> prescription;
    string medName;
    int quantity;

    cout << "Enter prescription details ('end' to finish):\n";
    while (true) {
        cout << "Medication name: ";
        getline(cin, medName);
        if (medName == "end") break;

        // Check if medication exists in inventory
        if (!isMedicationAvailable(medName)) {
            cout << "Medication '" << medName << "' is not available in the inventory." << endl;
            continue; // Ask for another medication
        }

        cout << "Quantity: ";
        cin >> quantity;
        cin.ignore(); // Clear the input buffer

        prescription[medName] = quantity;
    }

    float totalBill = 0;
    bool allAvailable = true;

    cout << "Prescription Bill:" << endl;
    for (const auto& item : prescription) {
        string medName = item.first;
        int quantity = item.second;

        BaseMedication* med = findMedication(medName);
        if (med != nullptr && med->getQuantity() >= quantity) {
            totalBill += med->getPrice() * quantity;
            med->dispense(quantity);
            cout << medName << " - Quantity: " << quantity << " - Price: $" << med->getPrice() * quantity << endl;
        } else {
            cout << "Not enough " << medName << " in stock for the prescribed quantity." << endl;
            allAvailable = false;
        }
    }

    if (allAvailable) {
        cout << "Total Bill: $" << totalBill << endl;
    } else {
        cout << "Prescription cannot be fulfilled completely." << endl;
    }
}

    // Function to prescribe medication based on symptoms
    void prescribeMedication(const vector<string>& symptoms) {
        bool prescribed = false;
        for (const auto& symptom : symptoms) {
            string medName = getMedicationName(symptom);
            if (!medName.empty()) {
                BaseMedication* med = findMedication(medName);
                if (med != NULL && med->getQuantity() > 0) {
                    cout << "Prescribed " << medName << " for " << symptom << "." << endl;
                    prescribed = true;
                    break;
                }
            }
        }
        if (!prescribed) {
            cout << "No medication prescribed for given symptoms." << endl;
        }
    }
};

int main() {
    Pharmacy pharmacy;

    // Load inventory from file
    pharmacy.loadInventory();

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Display Inventory\n";
        cout << "2. Check Medication Availability\n";
        cout << "3. Process Prescription\n";
        cout << "4. Prescribe Medication\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer

        switch (choice) {
            case 1:
                pharmacy.displayInventory();
                break;
            case 2: {
                string medName;
                cout << "Enter medication name: ";
                getline(cin, medName);
                if (pharmacy.isMedicationAvailable(medName)) {
                    cout << "Medication '" << medName << "' is available in the inventory." << endl;
                } else {
                    cout << "Medication '" << medName << "' is not available in the inventory." << endl;
                }
                break;
            }
            case 3:
                pharmacy.processPrescription();
                break;
            case 4: {
                string input;
                cout << "Enter symptoms separated by spaces: ";
                getline(cin, input);
                stringstream ss(input);
                vector<string> symptoms;
                string symptom;
                while (ss >> symptom) {
                    symptoms.push_back(symptom);
                }
                pharmacy.prescribeMedication(symptoms);
                break;
            }
            case 5:
                cout << "Exiting program...\n";
                // Save inventory to file before exiting
                pharmacy.saveInventory();
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}
