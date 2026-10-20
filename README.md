# Car Rental Management System in C

A simple but complete car rental management system written in C with CSV file persistence.

## Features

- **Car Management**: Add, list, and remove cars from inventory
- **Rental Management**: Rent cars to customers, return cars
- **Billing System**: Automatic cost calculation based on daily rate and rental duration
- **CSV Persistence**: All data saved to `cars.csv` and `rentals.csv` files
- **Data Loading**: Automatically loads existing data on startup

## Files

- `car_rental.h` - Header file with structures and function declarations
- `car_rental.c` - Main implementation
- `cars.csv` - Car inventory data (auto-generated)
- `rentals.csv` - Rental history data (auto-generated)

## Compilation

```bash
gcc car_rental.c -o car_rental.exe
```

## Usage

Run the program:
```bash
./car_rental.exe
```

### Menu Options

1. **Add Car** - Add a new car to inventory (make, model, year, license plate, daily rate)
2. **List Cars** - View all cars with availability status
3. **Remove Car** - Remove a car (only if available)
4. **Rent Car** - Rent an available car to a customer
5. **Return Car** - Return a rented car and generate bill
6. **List All Rentals** - View complete rental history
7. **View Active Rentals** - See currently rented cars
8. **Save Data to CSV** - Manually save data
9. **Exit** - Save and quit

## CSV File Format

### cars.csv
```csv
id,make,model,year,license_plate,daily_rate,is_available
1,Toyota,Camry,2023,ABC-1234,50.00,1
```

### rentals.csv
```csv
id,car_id,customer_name,customer_phone,rent_date,return_date,total_cost,is_active
1,1,John Doe,555-1234,2026-10-20 17:34:30,2026-10-20 18:34:30,50.00,0
```

## Billing Logic

- Minimum charge: 1 day
- Calculated based on actual rental duration (rounded up to nearest day)
- Formula: `days_rented * daily_rate`

## Extending the System

You can easily add features by modifying the structures in `car_rental.h`:

- Add customer database
- Add car categories (Sedan, SUV, Truck)
- Add discount codes
- Add payment tracking
- Add maintenance scheduling

## Data Persistence

Data is automatically:
- **Loaded** on program startup from CSV files
- **Saved** after every operation (add, rent, return, remove)
- **Saved** on exit

This means your data survives program restarts!