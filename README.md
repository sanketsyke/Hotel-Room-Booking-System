# NAS Hotels-Room-Booking-System

## Overview
The **Hotel Room Booking System** is a C-based project designed to manage hotel operations such as room booking, guest management, staff authentication, and feedback collection. The system supports two main user roles: hotel guests and staff, providing a user-friendly command-line interface for each.

## Features
- **Room Booking:** Allows guests to book available rooms, view room details, and manage bookings.
- **Guest Management:** Handles guest data and maintains booking history.
- **Staff Interface:** Hotel staff can view all bookings, check room statuses, and manage operations with secure authentication.
- **Feedback System:** Guests can provide feedback on their experience.
- **Booking History:** Tracks current and past bookings using persistent data storage.
- **Authentication:** Staff login is required for management operations.

## File Structure
project/
|-- auth.c, auth.h # Staff authentication module
|-- booking.c, booking.h # Booking management
|-- booking_id_counter.dat # Counter for unique booking IDs
|-- bookings.dat # Data file for current bookings
|-- constants.h # Shared constants
|-- feedback.c, feedback.h # Feedback module
|-- main.c # Main program (entry point)
|-- nas_hotels.exe # Compiled executable (for Windows)
|-- past_bookings.dat # Data file for past bookings
|-- room.c, room.h # Room management


## How to Run

### Prerequisites
- C compiler (e.g., gcc)
- Command-line/terminal environment

### Compilation
gcc main.c booking.c room.c feedback.c auth.c -o nas_hotels

### Running the Program
./nas_hotels

## Usage
When executed, the system displays the main menu:

- **User:** Allows booking rooms, viewing bookings, and submitting feedback.
- **Hotel Staff:** Requires login; can view/manage bookings and room statuses.
- **Exit:** Ends the program and saves all changes.

### User Features:
- Search Rooms : User recieves a table of rooms of different types
- Book Room : User can book a room
- Checkout : User can checkout 
- View bookings (current & past) : User can view all his bookings
- Give feedback : User can provide feedback

### Hotel Staff Features:
- Checkout Room: 

## Data Persistence
- Booking and guest data are saved in `.dat` files for persistence across sessions.
- Feedback records and authentication credentials are managed within their respective modules.





