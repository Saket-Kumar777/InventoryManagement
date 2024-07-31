# Pharmacy Management System

This project is a Pharmacy Management System developed as part of the fourth semester of engineering. The system is implemented in C++ using Object-Oriented Programming (OOP) principles. The primary functionality of this system is to manage the inventory of a pharmacy, including adding, dispensing, and prescribing medications based on symptoms.

## Features

- **Inventory Management**: Add, update, and display medications in the inventory.
- **Medication Dispensation**: Dispense medications and update the inventory accordingly.
- **Prescription Processing**: Process prescriptions and generate bills.
- **Symptom-Based Prescription**: Prescribe medications based on symptoms.
- **Persistent Storage**: Load and save inventory data from/to a file.

## File Structure

- `src.cpp`: The main source code file containing the implementation of the pharmacy management system.

## Classes

### `BaseMedication`
An abstract base class for medications with pure virtual functions for getting medication details, dispensing, and saving to file.

### `GenericMedication`
A derived class from `BaseMedication` that represents a generic medication with attributes like name, quantity, and price. It includes methods for setting and getting these attributes, dispensing medication, and saving/loading data from a file.

### `Symptom`
A class representing a symptom with a single attribute for the symptom's name.

### `SymptomToMedication`
A derived class from `Symptom` that maps symptoms to corresponding medications. It includes a method to get the medication name based on a symptom.

### `Pharmacy`
A derived class from `SymptomToMedication` that handles the pharmacy's inventory. It includes methods for loading and saving inventory, adding and finding medications, checking availability, displaying inventory, updating medication details, processing prescriptions, and prescribing medications based on symptoms.

## How to Use

1. **Compile the Code**
   ```sh
   g++ -o pharmacy src.cpp
   ```

2. **Run the Program**
   ```sh
   ./pharmacy
   ```

3. **Menu Options**
   - Display Inventory: Shows all medications in the inventory.
   - Check Medication Availability: Check if a specific medication is available in the inventory.
   - Process Prescription: Enter prescription details and generate a bill.
   - Prescribe Medication: Enter symptoms and get a prescribed medication based on the symptoms.
   - Exit: Save the inventory to the file and exit the program.

## Example

```sh
Menu:
1. Display Inventory
2. Check Medication Availability
3. Process Prescription
4. Prescribe Medication
5. Exit
Enter your choice: 1
Inventory:
Aspirin - Quantity: 50 - Price: $5.0
Paracetamol - Quantity: 100 - Price: $3.5
Cough Syrup - Quantity: 75 - Price: $7.0
```

## Dependencies

- C++11 or higher
- Standard C++ Library



## Authors

- Saket Kumar
- Sumit Debnath

---

Feel free to contribute to this project by submitting issues or pull requests.

---

**Note**: The inventory data is stored in `inventory.txt`. Make sure this file exists in the same directory as the executable or modify the code to specify a different path.

---

Happy Coding!
