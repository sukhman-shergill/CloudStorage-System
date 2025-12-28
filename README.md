# Cloud Storage System with Data Deduplication

A C++ implementation of a cloud storage system featuring data deduplication to optimize storage efficiency. This project demonstrates core concepts of distributed storage systems used by services like Dropbox and Google Drive.

## Overview

This system implements a multi-user cloud storage platform with the following capabilities:

- User authentication and account management
- File upload, download, and deletion operations
- Intelligent data deduplication using chunk-level hashing
- Storage quota tracking and management (1MB per user)
- Real-time deduplication statistics

The key innovation here is the deduplication mechanism. When multiple files contain identical data, the system stores that data only once, significantly reducing storage requirements. This is achieved by splitting files into fixed-size chunks and using hash-based duplicate detection.

## Implementation Details

### Deduplication Algorithm

The deduplication system works as follows:

1. **Chunking**: Files are divided into fixed 50-byte chunks
2. **Hash Generation**: Each chunk is processed through a polynomial rolling hash function
3. **Duplicate Detection**: The system maintains a hash table to identify existing chunks
4. **Reference Counting**: Multiple files can reference the same chunk through a reference counter
5. **Storage Optimization**: Only unique chunks are physically stored

**Example**: Uploading two identical 1KB files results in only 1KB of actual storage usage instead of 2KB, achieving 50% space efficiency.

### Technical Architecture

- **Language**: C++ (C++14 standard)
- **Core Data Structures**:
  - `map<string, string>` - Chunk storage mapping (Hash → Content)
  - `map<string, int>` - Reference counting for garbage collection
  - `vector<Chunk>` - File chunk metadata
  - `unordered_map` - User management and file indexing
- **Design Pattern**: Object-oriented architecture with three main classes:
  - `StorageSystem` - Handles deduplication logic and chunk management
  - `User` - Manages user credentials and storage quotas
  - `CloudStorageSystem` - Coordinates file operations and user sessions

### Hash Function

Implemented a simple polynomial rolling hash:

```cpp
hash = (hash * 31 + character) % 1000000007
```

While not cryptographically secure, it provides sufficient collision resistance for this application's scope and demonstrates the hashing concept effectively.

## Build Instructions

Compile using g++ with C++14 support:

```bash
g++ -std=c++14 main.cpp CloudStorage.cpp -o CloudStorage.exe
```

Run the executable:

```bash
./CloudStorage.exe
```

## Usage

The system provides an interactive menu with the following options:

1. **User Registration** - Create new user accounts
2. **Login** - Authenticate existing users
3. **Upload File** - Store files with automatic deduplication
4. **Download File** - Retrieve and reconstruct files from chunks
5. **Delete File** - Remove files and update reference counts
6. **List Files** - View all uploaded files with metadata
7. **Storage Info** - Check quota usage and available space
8. **Deduplication Stats** - View system-wide deduplication metrics
9. **Demo Mode** - Automated demonstration of deduplication (uploads same content twice)

## Demonstration Output

Running the deduplication demo (option 9):

```
Uploading 'demo_file_1.txt'...
File uploaded successfully!
Split into 4 chunks.

Now uploading 'demo_file_2.txt' with SAME content...
  [Deduplication] Duplicate found! Saving space.
  [Deduplication] Duplicate found! Saving space.
  [Deduplication] Duplicate found! Saving space.
  [Deduplication] Duplicate found! Saving space.
File uploaded successfully!

========== Deduplication Statistics ==========
Total Chunks Referenced: 8
Unique Chunks Stored: 4
Space Saved: 200 bytes
Deduplication Efficiency: 50%
```

This demonstrates the system successfully identifying and reusing existing chunks, achieving 50% storage reduction.

## Project Structure

```
├── CloudStorage.h      # Class declarations and interfaces
├── CloudStorage.cpp    # Core implementation (400+ lines)
└── main.cpp           # Menu-driven user interface
```

## Key Learnings

Through this project, I gained practical experience with:

- Real-world application of hash tables and their collision handling
- Memory-efficient storage techniques used in production systems
- Reference counting mechanisms (similar to smart pointers)
- Trade-offs between space complexity and time complexity
- STL container selection and performance characteristics
- Object-oriented design principles in systems programming

## Future Enhancements

Potential improvements for this project:

- Implement variable-size chunking (content-defined chunking)
- Add encryption for secure storage
- Implement compression before storage
- Add multi-threading for concurrent uploads
- Replace simple hash with SHA-256 for production use
- Add persistent storage (currently in-memory only)

## Author

Sukhman Singh B.Tech Computer Science (2nd Year) Developed as a Systems Engineering exploration.
Course Project - Data Structures & Object-Oriented Programming

---

_This project was developed as a learning exercise to understand how modern cloud storage systems optimize storage through deduplication._
