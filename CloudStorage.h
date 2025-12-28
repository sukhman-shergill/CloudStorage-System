#ifndef CLOUDSTORAGE_H
#define CLOUDSTORAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <ctime>

using namespace std;

// Structure to store chunk information
struct Chunk
{
  string chunkHash; // Hash of chunk content
  int chunkIndex;   // Position in file
  int chunkSize;    // Size of this chunk
};

// Structure to store file information
struct FileInfo
{
  string fileName;
  vector<Chunk> chunks; // File is stored as list of chunks
  long long fileSize;
  string uploadDate;
  string owner;
};

// Class to handle user management
class User
{
private:
  string username;
  string password;
  long long storageUsed;
  long long storageLimit;

public:
  User();
  User(string uname, string pass, long long limit = 1000000); // 1MB default limit

  string getUsername();
  bool verifyPassword(string pass);
  long long getStorageUsed();
  long long getStorageLimit();
  void updateStorage(long long size);
  bool hasSpaceFor(long long size);
};

// Class to handle storage and deduplication
class StorageSystem
{
private:
  map<string, string> storage; // Hash -> Actual chunk content
  map<string, int> refCount;   // Hash -> Reference count (how many files use it)
  int chunkSize;               // Size of each chunk in bytes

public:
  StorageSystem(int chunkSz = 50); // Default chunk size 50 bytes

  // Core deduplication functions
  string simpleHash(string content);                      // Simple hash function
  vector<Chunk> splitIntoChunks(string content);          // Split file into chunks
  bool storeChunk(string chunkHash, string chunkContent); // Store chunk if not exists
  string getChunkContent(string chunkHash);               // Retrieve chunk content
  void deleteChunk(string chunkHash);                     // Delete chunk and manage ref count

  // Statistics
  int getTotalChunks();
  int getUniqueChunks();
  long long getSavedSpace();
};

// Main Cloud Storage System class
class CloudStorageSystem
{
private:
  unordered_map<string, User> users;                 // username -> User object
  unordered_map<string, vector<FileInfo>> userFiles; // username -> list of files
  string currentUser;
  bool isLoggedIn;
  StorageSystem storageSystem; // Deduplication storage system

public:
  CloudStorageSystem();

  // User management functions
  bool registerUser(string username, string password);
  bool loginUser(string username, string password);
  void logoutUser();
  string getCurrentUser();

  // File operations
  bool uploadFile(string fileName, string fileContent);
  bool downloadFile(string fileName, string &fileContent);
  bool deleteFile(string fileName);
  vector<FileInfo> listFiles();

  // Storage info
  void showStorageInfo();
  void showDeduplicationStats(); // Show deduplication statistics

  // Helper function to get current date
  string getCurrentDate();
};

#endif
