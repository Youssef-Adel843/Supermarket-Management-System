#define _CRT_SECURE_NO_WARNINGS
#pragma codepage(65001) // Ensures compatibility with Visual Studio

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MANAGER_PASSWORD "admin123"

// --- Data Structures ---
typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

// --- Function Prototypes ---
void loadInventory(Product* inventory, int* count);
void saveInventory(Product* inventory, int count);
void displayMenu();
void managerMode(Product* inventory, int* count);
void customerMode(Product* inventory, int* count);
void viewStock(Product* inventory, int count);
void addProduct(Product* inventory, int* count);
void addToCart(Product* inventory, int inventoryCount, Product* cart, int* cartCount);
void checkout(Product* cart, int cartCount);
int findProduct(Product* inventory, int count, int id);

// --- Main Program ---
int main() {
    Product inventory[MAX_ITEMS];
    int itemCount = 0;
    int choice;

    // Load data from file at startup
    loadInventory(inventory, &itemCount);

    // Main program loop
    do {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
        case 1:
            managerMode(inventory, &itemCount);
            break;
        case 2:
            customerMode(inventory, &itemCount);
            break;
        case 3:
            printf("\nSaving data and shutting down...\n");
            saveInventory(inventory, itemCount);
            break;
        default:
            printf("\nInvalid option. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

// --- Menu UI ---
void displayMenu() {
    printf("\n===================================\n");
    printf("   SUPERMARKET MANAGEMENT SYSTEM   \n");
    printf("===================================\n");
    printf("1. Manager Mode\n");
    printf("2. Customer Mode\n");
    printf("3. Exit\n");
    printf("Select an option: ");
}

// --- File I/O Operations ---
void loadInventory(Product* inventory, int* count) {
    FILE* file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("No existing inventory file found. Starting fresh.\n");
        *count = 0;
        return;
    }

    *count = 0;
    // Read formatted data from the file
    while (fscanf(file, "%d %49s %f %d",
        &inventory[*count].id,
        inventory[*count].name,
        &inventory[*count].price,
        &inventory[*count].quantity) == 4) {
        (*count)++;
    }
    fclose(file);
    printf("Inventory loaded successfully. (%d items found)\n", *count);
}

void saveInventory(Product* inventory, int count) {
    FILE* file = fopen("inventory.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open inventory file for saving.\n");
        return;
    }

    // Write formatted data back to the file using a for loop
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %.2f %d\n",
            inventory[i].id,
            inventory[i].name,
            inventory[i].price,
            inventory[i].quantity);
    }
    fclose(file);
    printf("Inventory saved successfully.\n");
}

// --- Helper Functions ---
int findProduct(Product* inventory, int count, int id) {
    // Search the inventory array for a matching ID
    for (int i = 0; i < count; i++) {
        if (inventory[i].id == id) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

void viewStock(Product* inventory, int count) {
    if (count == 0) {
        printf("\nInventory is currently empty.\n");
        return;
    }

    printf("\n--- Current Inventory ---\n");
    printf("%-5s | %-20s | %-8s | %-8s\n", "ID", "Name", "Price", "Stock");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-20s | $%-7.2f | %-8d\n",
            inventory[i].id,
            inventory[i].name,
            inventory[i].price,
            inventory[i].quantity);
    }
}

// --- Manager Mode Functions ---
void managerMode(Product* inventory, int* count) {
    char password[50];
    printf("\n--- Manager Login ---\n");
    printf("Enter Password: ");
    scanf("%49s", password);

    if (strcmp(password, MANAGER_PASSWORD) != 0) {
        printf("Access Denied: Incorrect password.\n");
        return;
    }

    int choice;
    do {
        printf("\n--- Manager Menu ---\n");
        printf("1. View Stock\n");
        printf("2. Add New Product\n");
        printf("3. Back to Main Menu\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1:
            viewStock(inventory, *count);
            break;
        case 2:
            addProduct(inventory, count);
            break;
        case 3:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid option.\n");
        }
    } while (choice != 3);
}

void addProduct(Product* inventory, int* count) {
    if (*count >= MAX_ITEMS) {
        printf("Inventory is full! Cannot add more items.\n");
        return;
    }

    Product newProduct;
    printf("\nEnter Product ID: ");
    scanf("%d", &newProduct.id);

    // Edge Case: Check if ID already exists
    if (findProduct(inventory, *count, newProduct.id) != -1) {
        printf("Error: Product with ID %d already exists.\n", newProduct.id);
        return;
    }

    printf("Enter Product Name: ");
    scanf("%49s", newProduct.name);

    printf("Enter Product Price: ");
    scanf("%f", &newProduct.price);
    if (newProduct.price < 0) {
        printf("Error: Price cannot be negative.\n");
        return;
    }

    printf("Enter Initial Quantity: ");
    scanf("%d", &newProduct.quantity);
    if (newProduct.quantity < 0) {
        printf("Error: Quantity cannot be negative.\n");
        return;
    }

    // Add to array using pointer reference
    inventory[*count] = newProduct;
    (*count)++;
    printf("Product added successfully!\n");
}

// --- Customer Mode Functions ---
void customerMode(Product* inventory, int* count) {
    Product cart[MAX_ITEMS];
    int cartCount = 0;
    int choice;

    do {
        printf("\n--- Customer Menu ---\n");
        printf("1. View Available Products\n");
        printf("2. Add Item to Cart\n");
        printf("3. Checkout & Print Receipt\n");
        printf("4. Cancel & Return to Main Menu\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1:
            viewStock(inventory, *count);
            break;
        case 2:
            addToCart(inventory, *count, cart, &cartCount);
            break;
        case 3:
            if (cartCount > 0) {
                checkout(cart, cartCount);
                return; // Exit customer mode after successful checkout
            }
            else {
                printf("\nYour cart is empty. Nothing to checkout.\n");
            }
            break;
        case 4:
            // Note: If cancelling, we should ideally return deducted items to inventory. 
            // For simplicity in this logic flow, items are deducted directly in addToCart. 
            // In a robust system, we would restore the inventory here.
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid option.\n");
        }
    } while (choice != 4);
}

void addToCart(Product* inventory, int inventoryCount, Product* cart, int* cartCount) {
    int id, qty;

    printf("\nEnter Item ID to purchase: ");
    scanf("%d", &id);

    int index = findProduct(inventory, inventoryCount, id);

    // Edge Case: Item does not exist
    if (index == -1) {
        printf("Error: Item ID %d not found.\n", id);
        return;
    }

    printf("Enter quantity to purchase: ");
    scanf("%d", &qty);

    // Edge Case: Negative quantity
    if (qty <= 0) {
        printf("Error: Quantity must be greater than zero.\n");
        return;
    }

    // Edge Case: Insufficient stock
    if (qty > inventory[index].quantity) {
        printf("Error: Insufficient stock. Only %d remaining.\n", inventory[index].quantity);
        return;
    }

    // Deduct from main inventory
    inventory[index].quantity -= qty;

    // Add to shopping cart array
    cart[*cartCount].id = inventory[index].id;
    strcpy(cart[*cartCount].name, inventory[index].name);
    cart[*cartCount].price = inventory[index].price;
    cart[*cartCount].quantity = qty;
    (*cartCount)++;

    printf("Successfully added %d x %s to your cart.\n", qty, inventory[index].name);
}

void checkout(Product* cart, int cartCount) {
    FILE* receipt = fopen("receipt.txt", "w");
    float grandTotal = 0.0;

    printf("\n===================================\n");
    printf("             RECEIPT               \n");
    printf("===================================\n");

    if (receipt != NULL) {
        fprintf(receipt, "===================================\n");
        fprintf(receipt, "             RECEIPT               \n");
        fprintf(receipt, "===================================\n");
    }

    for (int i = 0; i < cartCount; i++) {
        float itemTotal = cart[i].price * cart[i].quantity;
        grandTotal += itemTotal;

        // Print to console
        printf("%-20s x %-3d | $%.2f\n", cart[i].name, cart[i].quantity, itemTotal);

        // Write to file
        if (receipt != NULL) {
            fprintf(receipt, "%-20s x %-3d | $%.2f\n", cart[i].name, cart[i].quantity, itemTotal);
        }
    }

    printf("-----------------------------------\n");
    printf("GRAND TOTAL:                $%.2f\n", grandTotal);
    printf("===================================\n");
    printf("Thank you for shopping with us!\n");

    if (receipt != NULL) {
        fprintf(receipt, "-----------------------------------\n");
        fprintf(receipt, "GRAND TOTAL:                $%.2f\n", grandTotal);
        fprintf(receipt, "===================================\n");
        fprintf(receipt, "Thank you for shopping with us!\n");
        fclose(receipt);
        printf("(Receipt saved to receipt.txt)\n");
    }
    else {
        printf("Error: Could not save receipt.txt file.\n");
    }
}