#include "CloudStorage.h"

// ========== StorageSystem Class Implementation ==========

StorageSystem::StorageSystem(int chunkSz)
{
  chunkSize = chunkSz;
}

// Simple hash function - adds ASCII values and multiplies by position
string StorageSystem::simpleHash(string content)
{
  long long hash = 0;
  for (int i = 0; i < content.length(); i++)
  {
    hash = (hash * 31 + content[i]) % 1000000007; // Simple polynomial rolling hash
  }
  return to_string(hash);
}

// Split file content into chunks
vector<Chunk> StorageSystem::splitIntoChunks(string content)
{
  vector<Chunk> chunks;
  int totalSize = content.length();
  int index = 0;

  // Split content into fixed-size chunks
  for (int i = 0; i < totalSize; i += chunkSize)
  {
    int currentChunkSize = min(chunkSize, totalSize - i);
    string chunkContent = content.substr(i, currentChunkSize);

    // Create chunk with hash
    Chunk newChunk;
    newChunk.chunkHash = simpleHash(chunkContent);
    newChunk.chunkIndex = index++;
    newChunk.chunkSize = currentChunkSize;

    // Store chunk in storage system
    storeChunk(newChunk.chunkHash, chunkContent);
    chunks.push_back(newChunk);
  }

  return chunks;
}

// Store chunk in storage if it doesn't exist
bool StorageSystem::storeChunk(string chunkHash, string chunkContent)
{
  // Check if chunk already exists (deduplication!)
  if (storage.find(chunkHash) != storage.end())
  {
    // Chunk already exists, just increment reference count
    refCount[chunkHash]++;
    cout << "  [Deduplication] Duplicate chunk found! Saving space.\n";
    return false; // Not a new chunk
  }

  // New chunk - store it
  storage[chunkHash] = chunkContent;
  refCount[chunkHash] = 1;
  return true; // New chunk stored
}

// Get chunk content by hash
string StorageSystem::getChunkContent(string chunkHash)
{
  if (storage.find(chunkHash) != storage.end())
  {
    return storage[chunkHash];
  }
  return ""; // Chunk not found
}

// Delete chunk and manage reference count
void StorageSystem::deleteChunk(string chunkHash)
{
  if (storage.find(chunkHash) == storage.end())
  {
    return; // Chunk doesn't exist
  }

  // Decrease reference count
  refCount[chunkHash]--;

  // If no more references, delete the chunk
  if (refCount[chunkHash] <= 0)
  {
    storage.erase(chunkHash);
    refCount.erase(chunkHash);
  }
}

// Get total number of chunk references
int StorageSystem::getTotalChunks()
{
  int total = 0;
  for (auto &pair : refCount)
  {
    total += pair.second;
  }
  return total;
}

// Get number of unique chunks stored
int StorageSystem::getUniqueChunks()
{
  return storage.size();
}

// Calculate space saved by deduplication
long long StorageSystem::getSavedSpace()
{
  long long totalSpace = 0;
  long long actualSpace = 0;

  for (auto &pair : storage)
  {
    string chunkHash = pair.first;
    string chunkContent = pair.second;
    int refs = refCount[chunkHash];

    totalSpace += chunkContent.length() * refs; // Space without deduplication
    actualSpace += chunkContent.length();       // Actual space used
  }

  return totalSpace - actualSpace;
}

// ========== User Class Implementation ==========

User::User()
{
  username = "";
  password = "";
  storageUsed = 0;
  storageLimit = 1000000; // 1MB default
}

User::User(string uname, string pass, long long limit)
{
  username = uname;
  password = pass;
  storageUsed = 0;
  storageLimit = limit;
}

string User::getUsername()
{
  return username;
}

// Check if entered password matches stored password
bool User::verifyPassword(string pass)
{
  return password == pass;
}

long long User::getStorageUsed()
{
  return storageUsed;
}

long long User::getStorageLimit()
{
  return storageLimit;
}

// Update storage when file is uploaded or deleted
void User::updateStorage(long long size)
{
  storageUsed += size;
}

// Check if user has enough space for new file
bool User::hasSpaceFor(long long size)
{
  return (storageUsed + size) <= storageLimit;
}

// ========== CloudStorageSystem Class Implementation ==========

CloudStorageSystem::CloudStorageSystem()
{
  isLoggedIn = false;
  currentUser = "";
}

// Register a new user in the system
bool CloudStorageSystem::registerUser(string username, string password)
{
  // Check if username already exists
  if (users.find(username) != users.end())
  {
    cout << "Username already exists!\n";
    return false;
  }

  // Create new user with 1MB storage limit
  users[username] = User(username, password, 1000000);
  userFiles[username] = vector<FileInfo>(); // Initialize empty file list
  cout << "User registered successfully!\n";
  return true;
}

// Login existing user
bool CloudStorageSystem::loginUser(string username, string password)
{
  // Check if user exists
  if (users.find(username) == users.end())
  {
    cout << "User not found!\n";
    return false;
  }

  // Verify password
  if (!users[username].verifyPassword(password))
  {
    cout << "Incorrect password!\n";
    return false;
  }

  currentUser = username;
  isLoggedIn = true;
  cout << "Login successful! Welcome " << username << "!\n";
  return true;
}

// Logout current user
void CloudStorageSystem::logoutUser()
{
  if (isLoggedIn)
  {
    cout << "User " << currentUser << " logged out successfully!\n";
    currentUser = "";
    isLoggedIn = false;
  }
}

string CloudStorageSystem::getCurrentUser()
{
  return currentUser;
}

// Upload a file to cloud storage with deduplication
bool CloudStorageSystem::uploadFile(string fileName, string fileContent)
{
  // Check if user is logged in
  if (!isLoggedIn)
  {
    cout << "Please login first!\n";
    return false;
  }

  long long fileSize = fileContent.length();

  // Check if user has enough storage space
  if (!users[currentUser].hasSpaceFor(fileSize))
  {
    cout << "Not enough storage space!\n";
    return false;
  }

  // Check if file with same name already exists
  for (const auto &file : userFiles[currentUser])
  {
    if (file.fileName == fileName)
    {
      cout << "File with this name already exists!\n";
      return false;
    }
  }

  cout << "Uploading file '" << fileName << "'...\n";

  // Split file into chunks and store with deduplication
  vector<Chunk> chunks = storageSystem.splitIntoChunks(fileContent);

  // Create file info and add to user's file list
  FileInfo newFile;
  newFile.fileName = fileName;
  newFile.chunks = chunks; // Store chunks instead of content
  newFile.fileSize = fileSize;
  newFile.uploadDate = getCurrentDate();
  newFile.owner = currentUser;

  userFiles[currentUser].push_back(newFile);
  users[currentUser].updateStorage(fileSize);

  cout << "File '" << fileName << "' uploaded successfully!\n";
  cout << "Split into " << chunks.size() << " chunks.\n";
  return true;
}

// Download a file from cloud storage
bool CloudStorageSystem::downloadFile(string fileName, string &fileContent)
{
  // Check if user is logged in
  if (!isLoggedIn)
  {
    cout << "Please login first!\n";
    return false;
  }

  // Search for file in user's files
  for (const auto &file : userFiles[currentUser])
  {
    if (file.fileName == fileName)
    {
      // Reconstruct file from chunks
      fileContent = "";
      for (const auto &chunk : file.chunks)
      {
        fileContent += storageSystem.getChunkContent(chunk.chunkHash);
      }
      cout << "File '" << fileName << "' downloaded successfully!\n";
      return true;
    }
  }

  cout << "File not found!\n";
  return false;
}

// Delete a file from cloud storage
bool CloudStorageSystem::deleteFile(string fileName)
{
  // Check if user is logged in
  if (!isLoggedIn)
  {
    cout << "Please login first!\n";
    return false;
  }

  // Search and delete file
  vector<FileInfo> &files = userFiles[currentUser];
  for (int i = 0; i < files.size(); i++)
  {
    if (files[i].fileName == fileName)
    {
      long long fileSize = files[i].fileSize;

      // Delete all chunks of this file
      for (const auto &chunk : files[i].chunks)
      {
        storageSystem.deleteChunk(chunk.chunkHash);
      }

      files.erase(files.begin() + i);
      users[currentUser].updateStorage(-fileSize); // Reduce storage used
      cout << "File '" << fileName << "' deleted successfully!\n";
      return true;
    }
  }

  cout << "File not found!\n";
  return false;
}

// List all files of current user
vector<FileInfo> CloudStorageSystem::listFiles()
{
  if (!isLoggedIn)
  {
    cout << "Please login first!\n";
    return vector<FileInfo>();
  }

  return userFiles[currentUser];
}

// Show storage information for current user
void CloudStorageSystem::showStorageInfo()
{
  if (!isLoggedIn)
  {
    cout << "Please login first!\n";
    return;
  }

  long long used = users[currentUser].getStorageUsed();
  long long limit = users[currentUser].getStorageLimit();

  cout << "\n========== Storage Info ==========\n";
  cout << "User: " << currentUser << "\n";
  cout << "Storage Used: " << used << " bytes\n";
  cout << "Storage Limit: " << limit << " bytes\n";
  cout << "Available: " << (limit - used) << " bytes\n";
  cout << "Usage: " << (used * 100.0 / limit) << "%\n";
  cout << "==================================\n\n";
}

// Show deduplication statistics
void CloudStorageSystem::showDeduplicationStats()
{
  cout << "\n========== Deduplication Statistics ==========\n";
  cout << "Total Chunks Referenced: " << storageSystem.getTotalChunks() << "\n";
  cout << "Unique Chunks Stored: " << storageSystem.getUniqueChunks() << "\n";
  cout << "Space Saved: " << storageSystem.getSavedSpace() << " bytes\n";

  if (storageSystem.getTotalChunks() > 0)
  {
    double efficiency = (1.0 - (double)storageSystem.getUniqueChunks() / storageSystem.getTotalChunks()) * 100.0;
    cout << "Deduplication Efficiency: " << efficiency << "%\n";
  }

  cout << "==============================================\n\n";
}

// Get current date as string
string CloudStorageSystem::getCurrentDate()
{
  time_t now = time(0);
  char *dt = ctime(&now);
  string dateStr(dt);
  // Remove newline character at the end
  if (!dateStr.empty() && dateStr[dateStr.length() - 1] == '\n')
  {
    dateStr.erase(dateStr.length() - 1);
  }
  return dateStr;
}
