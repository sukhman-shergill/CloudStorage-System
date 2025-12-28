#include "CloudStorage.h"

using namespace std;

// Function to display main menu
void displayMenu()
{
  cout << "\n========== Secure Cloud Storage System ==========\n";
  cout << "1. Register\n";
  cout << "2. Login\n";
  cout << "3. Upload File\n";
  cout << "4. Download File\n";
  cout << "5. Delete File\n";
  cout << "6. List Files\n";
  cout << "7. Storage Info\n";
  cout << "8. Deduplication Stats\n";
  cout << "9. Demo: Upload Same File Twice\n";
  cout << "10. Logout\n";
  cout << "11. Exit\n";
  cout << "================================================\n";
  cout << "Enter your choice: ";
}

int main()
{
  CloudStorageSystem cloudSystem;
  int choice;
  string username, password, fileName, fileContent;

  cout << "========================================\n";
  cout << "Welcome to Secure Cloud Storage System!\n";
  cout << "   With Data Deduplication Technology\n";
  cout << "========================================\n";

  while (true)
  {
    displayMenu();
    cin >> choice;
    cin.ignore(); // Clear input buffer

    switch (choice)
    {
    case 1:
    {
      // Register new user
      cout << "Enter username: ";
      getline(cin, username);
      cout << "Enter password: ";
      getline(cin, password);
      cloudSystem.registerUser(username, password);
      break;
    }

    case 2:
    {
      // Login existing user
      cout << "Enter username: ";
      getline(cin, username);
      cout << "Enter password: ";
      getline(cin, password);
      cloudSystem.loginUser(username, password);
      break;
    }

    case 3:
    {
      // Upload file
      cout << "Enter file name: ";
      getline(cin, fileName);
      cout << "Enter file content: ";
      getline(cin, fileContent);
      cloudSystem.uploadFile(fileName, fileContent);
      break;
    }

    case 4:
    {
      // Download file
      cout << "Enter file name to download: ";
      getline(cin, fileName);
      string downloadedContent;
      if (cloudSystem.downloadFile(fileName, downloadedContent))
      {
        cout << "\nFile Content:\n";
        cout << "----------------------------------------\n";
        cout << downloadedContent << "\n";
        cout << "----------------------------------------\n";
      }
      break;
    }

    case 5:
    {
      // Delete file
      cout << "Enter file name to delete: ";
      getline(cin, fileName);
      cloudSystem.deleteFile(fileName);
      break;
    }

    case 6:
    {
      // List all files
      vector<FileInfo> files = cloudSystem.listFiles();
      if (files.empty())
      {
        cout << "No files found!\n";
      }
      else
      {
        cout << "\n========== Your Files ==========\n";
        for (int i = 0; i < files.size(); i++)
        {
          cout << (i + 1) << ". File Name: " << files[i].fileName << "\n";
          cout << "   Size: " << files[i].fileSize << " bytes\n";
          cout << "   Chunks: " << files[i].chunks.size() << "\n";
          cout << "   Upload Date: " << files[i].uploadDate << "\n";
          cout << "   ----------------------------\n";
        }
        cout << "================================\n";
      }
      break;
    }

    case 7:
    {
      // Show storage info
      cloudSystem.showStorageInfo();
      break;
    }

    case 8:
    {
      // Show deduplication stats
      cloudSystem.showDeduplicationStats();
      break;
    }

    case 9:
    {
      // Demo: Upload same file twice to show deduplication
      cout << "\n========== Deduplication Demo ==========\n";
      cout << "This will upload the same file content twice to demonstrate deduplication.\n\n";

      string demoContent = "This is a sample file content for testing deduplication! "
                           "Data deduplication helps save storage space by storing "
                           "identical chunks only once. This is very useful in cloud storage!";

      cout << "Uploading 'demo_file_1.txt'...\n";
      cloudSystem.uploadFile("demo_file_1.txt", demoContent);

      cout << "\nNow uploading 'demo_file_2.txt' with SAME content...\n";
      cloudSystem.uploadFile("demo_file_2.txt", demoContent);

      cout << "\n>> Notice: 'Duplicate found! Saving space.' messages show deduplication in action!\n";
      cloudSystem.showDeduplicationStats();
      break;
    }

    case 10:
    {
      // Logout
      cloudSystem.logoutUser();
      break;
    }

    case 11:
    {
      // Exit program
      cout << "\n========================================\n";
      cout << "Thank you for using Secure Cloud Storage!\n";
      cout << "========================================\n";
      return 0;
    }

    default:
      cout << "Invalid choice! Please try again.\n";
    }
  }

  return 0;
}
