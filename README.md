Pharmacy Management System

This project is a comprehensive program designed to manage the operations of a pharmacy, from managing medicines and supplies to handling customers, prescriptions, and purchases. It's built to streamline pharmacy operations while providing a robust and interactive menu-driven interface. Perfect for demonstrating your C programming skills while tackling real-world problems.

Features

Here’s what the program can do:
Add Items: Add medicines, medical supplies, pharmacists, customers, and even prescriptions to the system.
View Data: Print details about medicines, supplies, pharmacists, customers, purchases, or the entire pharmacy.
Update Stock: Adjust stock levels of medicines and supplies as needed.
Sort and Search: Sort medicines and search for specific items based on various criteria.
Make Purchases: Simulate customer purchases while updating their purchase history and pharmacy stock.
Pharmacist Shifts: Update the working shifts of pharmacists.
Save and Load Data: Import data from text or binary files, and save updated data at the end of the session.
Quit: Exit the program gracefully.

How It Works
When you run the program, you’ll first choose whether to load data from a text file or a binary file. After that, you’re presented with a menu containing all the available options. Simply enter the corresponding number to perform an action, and the program will guide you through the rest.

Breadown of the menu options:
Add Medicine, Supply, Pharmacist, Customer, or Prescription: Allows you to input details and add new records to the system.
Print Functions: Displays all items of a specific category or even the entire pharmacy at once.
Update Stock: Adjust the quantities of medicines or supplies based on changes in inventory.
Sort and Search Medicines: Sort by name, brand, price, or stock and find medicines that meet specific criteria.
Change Pharmacist Shift: Update which shifts the pharmacists are working.
Make a Purchase: Allows a customer to buy medicines or supplies, updates the stock, and tracks the purchase.
Quit: Saves all data and exits the program.

Behind the Scenes

The program uses a combination of:
Dynamic Memory Allocation: Handles arrays of data that grow and shrink as needed.
Structs: Organizes related data like Pharmacy, CustomerManagement, Medicine, Supplies, Prescription, and more.
Linked Lists: Tracks prescriptions for individual customers.
Binary Search and Sorting: Ensures efficient searching and sorting functionality.
File Handling: Loads and saves data seamlessly.

How to Use It
Run the Program: Compile and execute the main.c file.
Load Data: Choose whether to load data from a text or binary file.
Navigate the Menu: Use the numbers to select actions from the menu.
Follow the Prompts: The program will guide you through any input it needs.
Save and Exit: When you’re done, exit the program to save your progress.

File Organization
Pharmacy.h / .c: Handles everything related to pharmacy operations, including medicines, supplies, and pharmacists.
CustomerManagement.h / .c: Manages customers, prescriptions, and purchases.
main.h / .c: Ties everything together, handles the menu, and runs the program loop.

Why This Project?
This project is a solid way to showcase your C programming skills. It demonstrates:
Practical use of dynamic memory.
Advanced strut management for organizing complex data.
Application of file I/O for real-world use cases.
User interaction through a menu-driven interface.
