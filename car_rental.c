#include "car_rental.h"

// Global arrays
Car cars[MAX_CARS];
Rental rentals[MAX_RENTALS];
int car_count = 0;
int rental_count = 0;
int next_car_id = 1;
int next_rental_id = 1;

// =============================================
// SYSTEM INITIALIZATION
// =============================================

void init_system() {
    load_cars_from_csv();
    load_rentals_from_csv();
    printf("Car Rental Management System initialized.\n");
    printf("Loaded %d cars and %d rentals from CSV files.\n\n", car_count, rental_count);
}

// =============================================
// CSV FILE OPERATIONS - CARS
// =============================================

void save_cars_to_csv() {
    FILE* file = fopen(CSV_CARS, "w");
    if (!file) {
        printf("Error: Could not open %s for writing.\n", CSV_CARS);
        return;
    }

    // Write header
    fprintf(file, "id,make,model,year,license_plate,daily_rate,is_available\n");

    // Write each car
    for (int i = 0; i < car_count; i++) {
        fprintf(file, "%d,%s,%s,%d,%s,%.2f,%d\n",
                cars[i].id,
                cars[i].make,
                cars[i].model,
                cars[i].year,
                cars[i].license_plate,
                cars[i].daily_rate,
                cars[i].is_available);
    }

    fclose(file);
    printf("Cars saved to %s successfully.\n", CSV_CARS);
}

void load_cars_from_csv() {
    FILE* file = fopen(CSV_CARS, "r");
    if (!file) {
        printf("No existing cars.csv file found. Starting fresh.\n");
        return;
    }

    char line[512];
    // Skip header line
    fgets(line, sizeof(line), file);

    car_count = 0;
    next_car_id = 1;

    while (fgets(line, sizeof(line), file) && car_count < MAX_CARS) {
        Car car;
        // Parse CSV line: id,make,model,year,license_plate,daily_rate,is_available
        int parsed = sscanf(line, "%d,%99[^,],%99[^,],%d,%99[^,],%f,%d",
                           &car.id, car.make, car.model, &car.year,
                           car.license_plate, &car.daily_rate, &car.is_available);

        if (parsed == 7) {
            cars[car_count++] = car;
            if (car.id >= next_car_id) {
                next_car_id = car.id + 1;
            }
        }
    }

    fclose(file);
}

// CSV FILE OPERATIONS - RENTALS

void save_rentals_to_csv() {
    FILE* file = fopen(CSV_RENTALS, "w");
    if (!file) {
        printf("Error: Could not open %s for writing.\n", CSV_RENTALS);
        return;
    }

    // Write header
    fprintf(file, "id,car_id,customer_name,customer_phone,rent_date,return_date,total_cost,is_active\n");

    for (int i = 0; i < rental_count; i++) {
        // Convert time_t to string for CSV
        char rent_date_str[32], return_date_str[32];
        struct tm* rent_tm = localtime(&rentals[i].rent_date);
        struct tm* return_tm = localtime(&rentals[i].return_date);
        strftime(rent_date_str, sizeof(rent_date_str), "%Y-%m-%d %H:%M:%S", rent_tm);
        strftime(return_date_str, sizeof(return_date_str), "%Y-%m-%d %H:%M:%S", return_tm);

        fprintf(file, "%d,%d,%s,%s,%s,%s,%.2f,%d\n",
                rentals[i].id,
                rentals[i].car_id,
                rentals[i].customer_name,
                rentals[i].customer_phone,
                rent_date_str,
                return_date_str,
                rentals[i].total_cost,
                rentals[i].is_active);
    }

    fclose(file);
    printf("Rentals saved to %s successfully.\n", CSV_RENTALS);
}

void load_rentals_from_csv() {
    FILE* file = fopen(CSV_RENTALS, "r");
    if (!file) {
        printf("No existing rentals.csv file found. Starting fresh.\n");
        return;
    }

    char line[512];
    // Skip header line
    fgets(line, sizeof(line), file);

    rental_count = 0;
    next_rental_id = 1;

    while (fgets(line, sizeof(line), file) && rental_count < MAX_RENTALS) {
        Rental rental;
        char rent_date_str[32], return_date_str[32];

        // Parse CSV line
        int parsed = sscanf(line, "%d,%d,%99[^,],%99[^,],%31[^,],%31[^,],%f,%d",
                           &rental.id, &rental.car_id,
                           rental.customer_name, rental.customer_phone,
                           rent_date_str, return_date_str,
                           &rental.total_cost, &rental.is_active);

        if (parsed == 8) {
            // Parse date strings back to time_t
            struct tm rent_tm = {0}, return_tm = {0};
            sscanf(rent_date_str, "%d-%d-%d %d:%d:%d",
                   &rent_tm.tm_year, &rent_tm.tm_mon, &rent_tm.tm_mday,
                   &rent_tm.tm_hour, &rent_tm.tm_min, &rent_tm.tm_sec);
            rent_tm.tm_year -= 1900;
            rent_tm.tm_mon -= 1;
            rental.rent_date = mktime(&rent_tm);

            sscanf(return_date_str, "%d-%d-%d %d:%d:%d",
                   &return_tm.tm_year, &return_tm.tm_mon, &return_tm.tm_mday,
                   &return_tm.tm_hour, &return_tm.tm_min, &return_tm.tm_sec);
            return_tm.tm_year -= 1900;
            return_tm.tm_mon -= 1;
            rental.return_date = mktime(&return_tm);

            rentals[rental_count++] = rental;
            if (rental.id >= next_rental_id) {
                next_rental_id = rental.id + 1;
            }
        }
    }

    fclose(file);
}

// =============================================
// CAR MANAGEMENT
// =============================================

void add_car() {
    if (car_count >= MAX_CARS) {
        printf("Error: Maximum number of cars (%d) reached.\n", MAX_CARS);
        return;
    }

    print_separator();
    printf("ADD NEW CAR\n");
    print_separator();

    Car car;
    car.id = next_car_id++;

    get_string_input("Enter car make (e.g., Toyota): ", car.make, MAX_STRING);
    get_string_input("Enter car model (e.g., Camry): ", car.model, MAX_STRING);
    car.year = get_int_input("Enter year: ");
    get_string_input("Enter license plate: ", car.license_plate, MAX_STRING);
    car.daily_rate = get_float_input("Enter daily rental rate: $");
    car.is_available = 1;

    cars[car_count++] = car;
    save_cars_to_csv();

    printf("\nCar added successfully! (ID: %d)\n", car.id);
}

void list_cars() {
    print_separator();
    printf("CAR INVENTORY (%d cars)\n", car_count);
    print_separator();

    if (car_count == 0) {
        printf("No cars in inventory.\n");
        return;
    }

    printf("%-4s %-15s %-15s %-6s %-15s %-10s %s\n",
           "ID", "Make", "Model", "Year", "License Plate", "Daily Rate", "Status");
    print_separator();

    for (int i = 0; i < car_count; i++) {
        printf("%-4d %-15s %-15s %-6d %-15s $%-9.2f %s\n",
               cars[i].id,
               cars[i].make,
               cars[i].model,
               cars[i].year,
               cars[i].license_plate,
               cars[i].daily_rate,
               cars[i].is_available ? "Available" : "Rented");
    }
}

void remove_car() {
    list_cars();
    if (car_count == 0) return;

    int id = get_int_input("\nEnter car ID to remove (0 to cancel): ");
    if (id == 0) return;

    Car* car = find_car_by_id(id);
    if (!car) {
        printf("Car with ID %d not found.\n", id);
        return;
    }

    if (!car->is_available) {
        printf("Cannot remove car - it is currently rented.\n");
        return;
    }

    // Shift remaining cars
    for (int i = 0; i < car_count; i++) {
        if (cars[i].id == id) {
            for (int j = i; j < car_count - 1; j++) {
                cars[j] = cars[j + 1];
            }
            car_count--;
            save_cars_to_csv();
            printf("Car removed successfully.\n");
            return;
        }
    }
}

Car* find_car_by_id(int id) {
    for (int i = 0; i < car_count; i++) {
        if (cars[i].id == id) {
            return &cars[i];
        }
    }
    return NULL;
}

Car* find_available_car() {
    for (int i = 0; i < car_count; i++) {
        if (cars[i].is_available) {
            return &cars[i];
        }
    }
    return NULL;
}

// =============================================
// RENTAL MANAGEMENT
// =============================================

void rent_car() {
    print_separator();
    printf("RENT A CAR\n");
    print_separator();

    // Show available cars
    int available_count = 0;
    for (int i = 0; i < car_count; i++) {
        if (cars[i].is_available) {
            available_count++;
        }
    }

    if (available_count == 0) {
        printf("No cars available for rent.\n");
        return;
    }

    printf("Available cars:\n");
    printf("%-4s %-15s %-15s %-6s %-15s %s\n",
           "ID", "Make", "Model", "Year", "License Plate", "Daily Rate");
    print_separator();

    for (int i = 0; i < car_count; i++) {
        if (cars[i].is_available) {
            printf("%-4d %-15s %-15s %-6d %-15s $%.2f\n",
                   cars[i].id, cars[i].make, cars[i].model,
                   cars[i].year, cars[i].license_plate, cars[i].daily_rate);
        }
    }

    int car_id = get_int_input("\nEnter car ID to rent (0 to cancel): ");
    if (car_id == 0) return;

    Car* car = find_car_by_id(car_id);
    if (!car) {
        printf("Car with ID %d not found.\n", car_id);
        return;
    }

    if (!car->is_available) {
        printf("Car is not available for rent.\n");
        return;
    }

    if (rental_count >= MAX_RENTALS) {
        printf("Error: Maximum rentals reached.\n");
        return;
    }

    Rental rental;
    rental.id = next_rental_id++;
    rental.car_id = car_id;
    get_string_input("Enter customer name: ", rental.customer_name, MAX_STRING);
    get_string_input("Enter customer phone: ", rental.customer_phone, MAX_STRING);
    rental.rent_date = time(NULL);
    rental.return_date = 0;
    rental.total_cost = 0;
    rental.is_active = 1;

    // Mark car as rented
    car->is_available = 0;

    rentals[rental_count++] = rental;
    save_cars_to_csv();
    save_rentals_to_csv();

    printf("\nCar rented successfully!\n");
    printf("Rental ID: %d\n", rental.id);
    printf("Car: %s %s (%s)\n", car->make, car->model, car->license_plate);
    printf("Daily rate: $%.2f\n", car->daily_rate);
    printf("Rented to: %s (%s)\n", rental.customer_name, rental.customer_phone);
}

void return_car() {
    print_separator();
    printf("RETURN A CAR\n");
    print_separator();

    // Show active rentals
    int active_count = 0;
    for (int i = 0; i < rental_count; i++) {
        if (rentals[i].is_active) {
            active_count++;
        }
    }

    if (active_count == 0) {
        printf("No active rentals.\n");
        return;
    }

    printf("Active rentals:\n");
    printf("%-4s %-6s %-20s %-15s %s\n",
           "Rental ID", "Car ID", "Customer", "Phone", "Rent Date");
    print_separator();

    for (int i = 0; i < rental_count; i++) {
        if (rentals[i].is_active) {
            char date_str[32];
            struct tm* tm = localtime(&rentals[i].rent_date);
            strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M", tm);

            Car* car = find_car_by_id(rentals[i].car_id);
            printf("%-4d %-6d %-20s %-15s %s\n",
                   rentals[i].id, rentals[i].car_id,
                   rentals[i].customer_name, rentals[i].customer_phone, date_str);
        }
    }

    int rental_id = get_int_input("\nEnter rental ID to return (0 to cancel): ");
    if (rental_id == 0) return;

    Rental* rental = NULL;
    for (int i = 0; i < rental_count; i++) {
        if (rentals[i].id == rental_id && rentals[i].is_active) {
            rental = &rentals[i];
            break;
        }
    }

    if (!rental) {
        printf("Active rental with ID %d not found.\n", rental_id);
        return;
    }

    // Calculate cost
    rental->return_date = time(NULL);
    Car* car = find_car_by_id(rental->car_id);
    if (car) {
        rental->total_cost = calculate_rental_cost(car, rental->rent_date, rental->return_date);
        car->is_available = 1;
    }
    rental->is_active = 0;

    save_cars_to_csv();
    save_rentals_to_csv();

    generate_bill(rental);
}

void list_rentals() {
    print_separator();
    printf("ALL RENTALS (%d total)\n", rental_count);
    print_separator();

    if (rental_count == 0) {
        printf("No rentals recorded.\n");
        return;
    }

    printf("%-4s %-6s %-20s %-15s %-20s %-20s %-10s %s\n",
           "ID", "Car", "Customer", "Phone", "Rent Date", "Return Date", "Cost", "Status");
    print_separator();

    for (int i = 0; i < rental_count; i++) {
        char rent_str[32], return_str[32];
        struct tm* tm;

        tm = localtime(&rentals[i].rent_date);
        strftime(rent_str, sizeof(rent_str), "%Y-%m-%d %H:%M", tm);

        if (rentals[i].return_date > 0) {
            tm = localtime(&rentals[i].return_date);
            strftime(return_str, sizeof(return_str), "%Y-%m-%d %H:%M", tm);
        } else {
            strcpy(return_str, "Active");
        }

        Car* car = find_car_by_id(rentals[i].car_id);
        char car_info[32];
        if (car) {
            sprintf(car_info, "%s %s", car->make, car->model);
        } else {
            strcpy(car_info, "Unknown");
        }

        printf("%-4d %-6d %-20s %-15s %-20s %-20s $%-9.2f %s\n",
               rentals[i].id, rentals[i].car_id,
               rentals[i].customer_name, rentals[i].customer_phone,
               rent_str, return_str, rentals[i].total_cost,
               rentals[i].is_active ? "Active" : "Completed");
    }
}

Rental* find_active_rental_by_car(int car_id) {
    for (int i = 0; i < rental_count; i++) {
        if (rentals[i].car_id == car_id && rentals[i].is_active) {
            return &rentals[i];
        }
    }
    return NULL;
}

// =============================================
// BILLING
// =============================================

float calculate_rental_cost(Car* car, time_t rent_date, time_t return_date) {
    double seconds = difftime(return_date, rent_date);
    double days = seconds / (24 * 60 * 60);

    // Round up to nearest day (minimum 1 day)
    int billable_days = (int)ceil(days);
    if (billable_days < 1) billable_days = 1;

    return billable_days * car->daily_rate;
}

void generate_bill(Rental* rental) {
    Car* car = find_car_by_id(rental->car_id);

    print_separator();
    printf("RENTAL BILL - Rental ID: %d\n", rental->id);
    print_separator();

    if (car) {
        printf("Vehicle: %s %s (%d) - %s\n", car->make, car->model, car->year, car->license_plate);
        printf("Daily Rate: $%.2f\n", car->daily_rate);
    }

    printf("Customer: %s\n", rental->customer_name);
    printf("Phone: %s\n", rental->customer_phone);

    char rent_str[32], return_str[32];
    struct tm* tm = localtime(&rental->rent_date);
    strftime(rent_str, sizeof(rent_str), "%Y-%m-%d %H:%M", tm);

    tm = localtime(&rental->return_date);
    strftime(return_str, sizeof(return_str), "%Y-%m-%d %H:%M", tm);

    printf("Rental Period: %s to %s\n", rent_str, return_str);

    double seconds = difftime(rental->return_date, rental->rent_date);
    double days = seconds / (24 * 60 * 60);
    int billable_days = (int)ceil(days);
    if (billable_days < 1) billable_days = 1;

    printf("Days Rented: %d (billed as %d day%s)\n", (int)days, billable_days, billable_days != 1 ? "s" : "");
    print_separator();
    printf("TOTAL DUE: $%.2f\n", rental->total_cost);
    print_separator();
}

// =============================================
// UTILITY FUNCTIONS
// =============================================

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int_input(const char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clear_input_buffer();
            return value;
        }
        clear_input_buffer();
        printf("Invalid input. Please enter a number.\n");
    }
}

float get_float_input(const char* prompt) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1) {
            clear_input_buffer();
            return value;
        }
        clear_input_buffer();
        printf("Invalid input. Please enter a number.\n");
    }
}

void get_string_input(const char* prompt, char* buffer, int size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            // Remove trailing newline
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            if (strlen(buffer) > 0) {
                return;
            }
        }
        printf("Input cannot be empty. Please try again.\n");
    }
}

void print_separator() {
    printf("------------------------------------------------------------\n");
}

// =============================================
// MAIN MENU
// =============================================

void show_menu() {
    print_separator();
    printf("CAR RENTAL MANAGEMENT SYSTEM\n");
    print_separator();
    printf("1. Add Car\n");
    printf("2. List Cars\n");
    printf("3. Remove Car\n");
    printf("4. Rent Car\n");
    printf("5. Return Car\n");
    printf("6. List All Rentals\n");
    printf("7. View Active Rentals\n");
    printf("8. Save Data to CSV\n");
    printf("9. Exit\n");
    print_separator();
}

int main() {
    init_system();

    int choice;
    do {
        show_menu();
        choice = get_int_input("Enter your choice: ");

        switch (choice) {
            case 1: add_car(); break;
            case 2: list_cars(); break;
            case 3: remove_car(); break;
            case 4: rent_car(); break;
            case 5: return_car(); break;
            case 6: list_rentals(); break;
            case 7:
                print_separator();
                printf("ACTIVE RENTALS\n");
                print_separator();
                for (int i = 0; i < rental_count; i++) {
                    if (rentals[i].is_active) {
                        Car* car = find_car_by_id(rentals[i].car_id);
                        char date_str[32];
                        struct tm* tm = localtime(&rentals[i].rent_date);
                        strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M", tm);
                        printf("Rental ID: %d | Car: %s %s | Customer: %s | Since: %s\n",
                               rentals[i].id, car ? car->make : "Unknown", car ? car->model : "",
                               rentals[i].customer_name, date_str);
                    }
                }
                break;
            case 8:
                save_cars_to_csv();
                save_rentals_to_csv();
                printf("All data saved to CSV files.\n");
                break;
            case 9:
                save_cars_to_csv();
                save_rentals_to_csv();
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("\n");
    } while (choice != 9);

    return 0;
}