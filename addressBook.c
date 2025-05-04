#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Contact structure definition
typedef struct Contact {
    char *firstName;
    char *familyName;
    long long phoneNum; // 10-digit phone number as 64-bit integer
    char *address;
    int age;
} Contact;

// Function prototypes (as required, unchanged)
int countContacts(Contact **contacts);
Contact *readNewContact();
Contact **appendContact(Contact **contacts, Contact *newContact);
Contact **insertContactAlphabetical(Contact **contacts, Contact *newContact);
Contact **removeContactByIndex(Contact **contacts);
int removeContactByFullName(Contact ***contacts);
void listContacts(Contact **contacts);
void saveContactsToFile(Contact **contacts, char *filename);
void printContactsToFile(Contact **contacts, char *filename);
Contact **loadContactsFromFile(char *filename);
Contact **appendContactsFromFile(Contact **contacts, char *filename);
Contact **mergeContactsFromFile(Contact **contacts, char *filename);
Contact *editContact(Contact **contacts, int index);

// Helper function to free a Contact
void freeContact(Contact *contact) {
    if (contact) {
        free(contact->firstName);
        free(contact->familyName);
        free(contact->address);
        free(contact);
    }
}

// Helper function to check for duplicates
int isDuplicate(Contact **contacts, Contact *newContact) {
    int count = countContacts(contacts);
    for (int i = 0; i < count; i++) {
        if (strcmp(contacts[i]->firstName, newContact->firstName) == 0 &&
            strcmp(contacts[i]->familyName, newContact->familyName) == 0) {
            return 1;
        }
    }
    return 0;
}

// Count contacts in NULL-terminated array
int countContacts(Contact **contacts) {
    if (!contacts) return 0;
    int count = 0;
    while (contacts[count] != NULL) count++;
    return count;
}

// Read and validate new contact details
Contact *readNewContact() {
    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    if (!newContact) {
        printf("Error: Memory allocation failed for Contact in readNewContact\n");
        return NULL;
    }
    char buffer[256];
    int attempts;

    // First name
    printf("Enter the first name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    newContact->firstName = (char *)malloc(strlen(buffer) + 1);
    if (!newContact->firstName) {
        printf("Error: unable to allocate memory for the first name string\n");
        free(newContact);
        return NULL;
    }
    strcpy(newContact->firstName, buffer);

    // Family name
    printf("Enter the family name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    newContact->familyName = (char *)malloc(strlen(buffer) + 1);
    if (!newContact->familyName) {
        printf("Error: unable to allocate memory for the family name string\n");
        free(newContact->firstName);
        free(newContact);
        return NULL;
    }
    strcpy(newContact->familyName, buffer);

    // Address
    printf("Enter the address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    newContact->address = (char *)malloc(strlen(buffer) + 1);
    if (!newContact->address) {
        printf("Error: unable to allocate memory for the address string\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact);
        return NULL;
    }
    strcpy(newContact->address, buffer);

    // Phone number
    attempts = 0;
    while (attempts < 5) {
        printf("Enter 10-digit phone number that must not start with 0: ");
        if (scanf("%lld", &newContact->phoneNum) == 1) {
            char phoneStr[20];
            sprintf(phoneStr, "%lld", newContact->phoneNum);
            if (strlen(phoneStr) == 10 && phoneStr[0] != '0') break;
        }
        printf("Error: Invalid phone number. Try again:\n");
        while (getchar() != '\n');
        attempts++;
    }
    if (attempts == 5) {
        printf("Error: Could not read a valid phone number\n");
        newContact->phoneNum = 0;
    }

    // Age
    attempts = 0;
    while (attempts < 5) {
        printf("Enter the age: ");
        if (scanf("%d", &newContact->age) == 1 && newContact->age >= 1 && newContact->age <= 150) break;
        printf("Error: Invalid age. Try again:\n");
        while (getchar() != '\n');
        attempts++;
    }
    if (attempts == 5) {
        printf("Error: Could not read a valid age\n");
        newContact->age = 0;
    }
    while (getchar() != '\n');
    return newContact;
}

// Appends contact to the end
Contact **appendContact(Contact **contacts, Contact *newContact) {
    if (!newContact) return contacts;
    int count = countContacts(contacts);
    Contact **newArray = (Contact **)realloc(contacts, (count + 2) * sizeof(Contact *));
    if (!newArray) {
        printf("Memory reallocation error in appendContact\n");
        return contacts;
    }
    newArray[count] = newContact;
    newArray[count + 1] = NULL;
    printf("Contact appended successfully by appendContact\n");
    return newArray;
}

// Inserts contact alphabetically
Contact **insertContactAlphabetical(Contact **contacts, Contact *newContact) {
    if (!newContact) return contacts;
    int count = countContacts(contacts);
    Contact **newArray = (Contact **)realloc(contacts, (count + 2) * sizeof(Contact *));
    if (!newArray) {
        printf("Memory reallocation error in insertContactAlphabetical\n");
        return contacts;
    }
    int i;
    for (i = 0; i < count; i++) {
        int cmp = strcmp(newContact->familyName, newArray[i]->familyName);
        if (cmp < 0 || (cmp == 0 && strcmp(newContact->firstName, newArray[i]->firstName) < 0)) break;
    }
    for (int j = count; j > i; j--) newArray[j] = newArray[j - 1];
    newArray[i] = newContact;
    newArray[count + 1] = NULL;
    printf("Contact was successfully added in alphabetical order\n");
    return newArray;
}

// Removes contact by index
Contact **removeContactByIndex(Contact **contacts) {
    if (!contacts) {
        printf("Error: value of addressBook received in removeContactByIndex was NULL\n");
        return NULL;
    }
    int count = countContacts(contacts);
    if (count == 0) {
        printf("Error: Index out of range in removeContactByIndex\n");
        return contacts;
    }
    int index;
    printf("Removing a contact by index\nEnter index to remove (0 based): ");
    if (scanf("%d", &index) != 1) {
        printf("Error: Value of index supplied could not be read.\n");
        while (getchar() != '\n');
        return contacts;
    }
    while (getchar() != '\n');
    if (index < 0 || index >= count) {
        printf("Error: Index out of range in removeContactByIndex\n");
        return contacts;
    }
    freeContact(contacts[index]);
    for (int i = index; i < count; i++) contacts[i] = contacts[i + 1];
    Contact **newArray = (Contact **)realloc(contacts, (count) * sizeof(Contact *));
    if (!newArray) {
        printf("Error: Memory reallocation failed in removeContactByIndex\n");
        return contacts;
    }
    if (count - 1 == 0) newArray[0] = NULL;
    printf("Contact removed successfully by removeContactByIndex\n");
    return newArray;
}

// Removes contact by full name
int removeContactByFullName(Contact ***contacts) {
    if (!contacts || !*contacts) {
        printf("Error: value of contacts received in removeContactByFullName was NULL\n");
        return 0;
    }
    char firstName[256], familyName[256];
    printf("Enter first name: ");
    fgets(firstName, sizeof(firstName), stdin);
    firstName[strcspn(firstName, "\n")] = 0;
    printf("Enter family name: ");
    fgets(familyName, sizeof(familyName), stdin);
    familyName[strcspn(familyName, "\n")] = 0;

    int count = countContacts(*contacts);
    for (int i = 0; i < count; i++) {
        if (strcmp((*contacts)[i]->firstName, firstName) == 0 &&
            strcmp((*contacts)[i]->familyName, familyName) == 0) {
            freeContact((*contacts)[i]);
            for (int j = i; j < count; j++) (*contacts)[j] = (*contacts)[j + 1];
            Contact **newArray = (Contact **)realloc(*contacts, (count) * sizeof(Contact *));
            if (!newArray) {
                printf("Error: Memory reallocation failed in removeContactByFullName\n");
                return 1;
            }
            *contacts = newArray;
            if (count - 1 == 0) (*contacts)[0] = NULL;
            printf("Contact '%s %s' removed successfully\n", firstName, familyName);
            return 1;
        }
    }
    printf("Contact '%s %s' not found\n", firstName, familyName);
    return 2;
}

// Lists all contacts
void listContacts(Contact **contacts) {
    int count = countContacts(contacts);
    if (count == 0) {
        printf("No contacts available.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("%d. %s %s\n", i + 1, contacts[i]->firstName, contacts[i]->familyName);
        printf("Phone: %lld\n", contacts[i]->phoneNum);
        printf("Address: %s\n", contacts[i]->address);
        printf("Age: %d\n", contacts[i]->age);
    }
}

// Saves the contacts to file (input format)
void saveContactsToFile(Contact **contacts, char *filename) {
    if (!filename) {
        printf("Error: filename formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }
    if (!contacts) {
        printf("Error: addressBook formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: file not opened in saveContactsToFile\n");
        return;
    }
    int count = countContacts(contacts);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n%s\n%s\n%lld\n%d\n", contacts[i]->firstName, contacts[i]->familyName,
                contacts[i]->address, contacts[i]->phoneNum, contacts[i]->age);
    }
    fclose(file);
}

// Print contacts to file (human-readable)
void printContactsToFile(Contact **contacts, char *filename) {
    if (!filename) {
        printf("Error: filename formal parameter passed value NULL in printContactsToFile\n");
        return;
    }
    if (!contacts) {
        printf("Error: addressBook formal parameter passed value NULL in printContactsToFile\n");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: file not opened in printContactsToFile\n");
        return;
    }
    int count = countContacts(contacts);
    fprintf(file, "Address Book Report\n\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d. %s %s\n", i + 1, contacts[i]->firstName, contacts[i]->familyName);
        fprintf(file, "Phone: %lld\n", contacts[i]->phoneNum);
        fprintf(file, "Address: %s\n", contacts[i]->address);
        fprintf(file, "Age: %d\n\n", contacts[i]->age);
    }
    fprintf(file, "Total Contacts: %d\n", count);
    fclose(file);
}

// Load contacts from file (replace existing)
Contact **loadContactsFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: file not opened in loadContactsFromFile\n");
        return (Contact **)malloc(sizeof(Contact *));
    }
    Contact **newContacts = (Contact **)malloc(sizeof(Contact *));
    newContacts[0] = NULL;
    char buffer[256];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        Contact *contact = (Contact *)malloc(sizeof(Contact));
        buffer[strcspn(buffer, "\n")] = 0;
        contact->firstName = strdup(buffer);
        fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n")] = 0;
        contact->familyName = strdup(buffer);
        fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n")] = 0;
        contact->address = strdup(buffer);
        fscanf(file, "%lld\n", &contact->phoneNum);
        fscanf(file, "%d\n", &contact->age);
        newContacts = (Contact **)realloc(newContacts, (count + 2) * sizeof(Contact *));
        newContacts[count++] = contact;
        newContacts[count] = NULL;
    }
    fclose(file);
    return newContacts;
}

// Appends contacts from file
Contact **appendContactsFromFile(Contact **contacts, char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: file not opened in appendContactsFromFile\n");
        return contacts;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        Contact *contact = (Contact *)malloc(sizeof(Contact));
        buffer[strcspn(buffer, "\n")] = 0;
        contact->firstName = strdup(buffer);
        fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n")] = 0;
        contact->familyName = strdup(buffer);
        fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n")] = 0;
        contact->address = strdup(buffer);
        fscanf(file, "%lld\n", &contact->phoneNum);
        fscanf(file, "%d\n", &contact->age);
        if (!isDuplicate(contacts, contact)) {
            contacts = appendContact(contacts, contact);
        } else {
            freeContact(contact);
        }
    }
    fclose(file);
    return contacts;
}

// Merges contacts from file
Contact **mergeContactsFromFile(Contact **contacts, char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: file not opened in mergeContactsFromFile\n");
        return contacts;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        Contact *contact = (Contact *)malloc(sizeof(Contact));
        buffer[strcspn(buffer, "\n")] = 0;
        contact->firstName = strdup(buffer);
        fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n")] = 0;
        contact->familyName = strdup(buffer);
        fgets(buffer, sizeof(buffer), file); buffer[strcspn(buffer, "\n")] = 0;
        contact->address = strdup(buffer);
        fscanf(file, "%lld\n", &contact->phoneNum);
        fscanf(file, "%d\n", &contact->age);
        if (!isDuplicate(contacts, contact)) {
            contacts = insertContactAlphabetical(contacts, contact);
        } else {
            freeContact(contact);
        }
    }
    fclose(file);
    return contacts;
}

// Edits contact by index
Contact *editContact(Contact **contacts, int index) {
    int count = countContacts(contacts);
    if (index < 0 || index >= count) return NULL;
    Contact *contact = contacts[index];
    int choice;
    char buffer[256];
    while (1) {
        printf("1. Edit First Name\n2. Edit Last Name\n3. Edit Address\n4. Edit Phone Number\n5. Edit Age\n6. Cancel\n");
        printf("Select an option: ");
        scanf("%d", &choice); 
        while (getchar() != '\n');
        switch (choice) {
            case 1:
                printf("Enter new first name: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                free(contact->firstName);
                contact->firstName = strdup(buffer);
                break;
            case 2:
                printf("Enter new family name: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                free(contact->familyName);
                contact->familyName = strdup(buffer);
                break;
            case 3:
                printf("Enter new address: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                free(contact->address);
                contact->address = strdup(buffer);
                break;
            case 4:
                printf("Enter new 10-digit phone number: ");
                scanf("%lld", &contact->phoneNum);
                while (getchar() != '\n');
                break;
            case 5:
                printf("Enter new age: ");
                scanf("%d", &contact->age);
                while (getchar() != '\n');
                break;
            case 6:
                return contact;
            default:
                printf("Invalid option.\n");
        }
    }
    return contact;
}


int main() {
    Contact **addressBook = (Contact **)malloc(sizeof(Contact *));
    addressBook[0] = NULL;
    int choice;
    char filename[256];

    while (1) {
        printf("\nAddress Book Menu\n");
        printf("1. Append Contact\n");
        printf("2. Insert Contact in Alphabetical Order\n");
        printf("3. Remove Contact by Index\n");
        printf("4. Remove Contact by Full Name\n");
        printf("5. Find and Edit Contact\n");
        printf("6. List Contacts\n");
        printf("7. Print Contacts to File with the format of an input file\n");
        printf("8. Print Contacts to File (Human Readable)\n");
        printf("9. Load Contacts from File Replacing Existing Contacts\n");
        printf("10. Append Contacts from File\n");
        printf("11. Merge Contacts from File\n");
        printf("12. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: {
                Contact *newContact = readNewContact();
                if (newContact) addressBook = appendContact(addressBook, newContact);
                break;
            }
            case 2: {
                Contact *newContact = readNewContact();
                if (newContact) addressBook = insertContactAlphabetical(addressBook, newContact);
                break;
            }
            case 3:
                addressBook = removeContactByIndex(addressBook);
                break;
            case 4:
                removeContactByFullName(&addressBook);
                break;
            case 5: {
                int index;
                printf("Enter index to edit (0-based): ");
                scanf("%d", &index);
                while (getchar() != '\n');
                editContact(addressBook, index);
                break;
            }
            case 6:
                listContacts(addressBook);
                break;
            case 7:
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                saveContactsToFile(addressBook, filename);
                break;
            case 8:
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                printContactsToFile(addressBook, filename);
                break;
            case 9: {
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                int count = countContacts(addressBook);
                for (int i = 0; i < count; i++) freeContact(addressBook[i]);
                free(addressBook);
                addressBook = loadContactsFromFile(filename);
                break;
            }
            case 10:
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                addressBook = appendContactsFromFile(addressBook, filename);
                break;
            case 11:
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                addressBook = mergeContactsFromFile(addressBook, filename);
                break;
            case 12: {
                int count = countContacts(addressBook);
                for (int i = 0; i < count; i++) freeContact(addressBook[i]);
                free(addressBook);
                return 0;
            }
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}