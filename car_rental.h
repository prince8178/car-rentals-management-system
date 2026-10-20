#ifndef CAR_RENTAL_H
#define CAR_RENTAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Constants
#define MAX_CARS 100
#define MAX_RENTALS 200
#define MAX_STRING 100
#define CSV_CARS "cars.csv"
#define CSV_RENTALS "rentals.csv"

// Car structure
typedef struct {
    int id;
    char make[MAX_STRING];
    char model[MAX_STRING];
    int year;
    char license_plate[MAX_STRING];
    float daily_rate;
    int is_available;  // 1 = available, 0 = rented
} Car;

// Rental structure
typedef struct {
    int id;
    int car_id;
    char customer_name[MAX_STRING];
    char customer_phone[MAX_STRING];
    time_t rent_date;
    time_t return_date;
    float total_cost;
    int is_active;  // 1 = active rental, 0 = completed
} Rental;

// Function declarations
void init_system();
void save_cars_to_csv();
void load_cars_from_csv();
void save_rentals_to_csv();
void load_rentals_from_csv();

// Car management
void add_car();
void list_cars();
void remove_car();
Car* find_car_by_id(int id);
Car* find_available_car();

// Rental management
void rent_car();
void return_car();
void list_rentals();
Rental* find_active_rental_by_car(int car_id);

// Billing
float calculate_rental_cost(Car* car, time_t rent_date, time_t return_date);
void generate_bill(Rental* rental);

// Utility
void clear_input_buffer();
int get_int_input(const char* prompt);
float get_float_input(const char* prompt);
void get_string_input(const char* prompt, char* buffer, int size);
void print_separator();

#endif