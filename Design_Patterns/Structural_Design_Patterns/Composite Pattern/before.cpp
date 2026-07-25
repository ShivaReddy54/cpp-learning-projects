/*

Intent: Compose objects into tree structures to represent part-whole hierarchies.
    Composite lets clients treat individual objects and compositions uniformly.

EX: A file system. A folder can contain files (leaf) or other folders (composite).
    When you calculate total size, you treat files and folders the same way — "give me your size."
    A folder recursively asks its children, a file just returns its own size.
    You don't care whether you're dealing with a file or a folder.

*/

#include <iostream>
#include <string>
#include <vector>

// ===== Leaf objects (files) =====
class File{
private:
    std::string name;
    int size;

public:
    File(const std::string& n, int s) : name(n), size(s) {}

    std::string getName(){ return name; }
    int getSize() { return size; }

    void display(){
        std::cout << "File: " << name << " (" << size << " KB)" << std::endl;
    }
};

// ===== Composite objects (folders) =====
class Folder{
private:
    std::string name;
    std::vector<File*> files;      // Can only hold Files
    std::vector<Folder*> subFolders; // Can hold sub-folders

public:
    Folder(const std::string& n) : name(n) {}

    void addFile(File* file) { files.push_back(file); }
    void addSubFolder(Folder* folder) { subFolders.push_back(folder); }

    // PROBLEM: Client must treat Files and Folders DIFFERENTLY!
    int getTotalSize(){
        int total = 0;

        // Handle files differently
        for(File* file : files) total += file->getSize();

        // Handle folders differently
        for(Folder* folder : subFolders) total += folder->getTotalSize();  // Recursive

        return total;
    }

    void display(){
        std::cout << "Folder: " << name << " [" << getTotalSize() << " KB]" << std::endl;
        
        // Handle files differently
        for(File* file : files) file->display();

        // Handle folders differently
        for(Folder* folder : subFolders) folder->display();
    }
};

// ===== CLIENT CODE =====
int main() {
    File* file1 = new File("resume.doc", 120);
    File* file2 = new File("photo.jpg", 500);
    File* file3 = new File("notes.txt", 30);
    File* file4 = new File("budget.xlsx", 200);

    Folder* documents = new Folder("Documents");
    documents->addFile(file1);
    documents->addFile(file3);

    Folder* pictures = new Folder("Pictures");
    pictures->addFile(file2);

    Folder* root = new Folder("Home");
    root->addFile(file4);
    root->addSubFolder(documents);
    root->addSubFolder(pictures);

    // PROBLEM: Client must know if something is a File or a Folder
    // Can't treat them with a common interface!
    // Must use different methods: file.getSize() vs folder.getTotalSize()

    root->display();
    std::cout << "Total size: " << root->getTotalSize() << " KB" << std::endl;

    // PROBLEM: Can we add a folder inside a file? NO! But nothing prevents
    // the conceptual need for mixed structures.
    // PROBLEM: Every new operation (search, delete, move) needs different
    // handling for File vs Folder — lots of if-else/type checks!

    delete file1; delete file2; delete file3; delete file4;
    delete documents; delete pictures; delete root;

}

/*
===============================================================================
                  Problems Without the Composite Pattern
===============================================================================

Client Code

if (item is File)
    └──► getSize()

if (item is Folder)
    └──► getTotalSize()


Every time a new operation is added:

if (item is File)
    └──► searchInFile()

if (item is Folder)
    └──► searchInFolder()


The client must constantly check the object's actual type before deciding
which function to call.


Problems:

❌ No Uniform Treatment
   Files and folders cannot be treated through a common interface.

❌ Type Checking Everywhere
   The client repeatedly relies on if-else statements, type checks,
   or dynamic_cast to determine the object's concrete type.

❌ No Common Interface
   Different object types expose different APIs, making generic
   client code difficult to write.

❌ Branching for Every New Operation
   Each new operation introduces additional conditional logic,
   increasing code complexity.

❌ Difficult to Extend
   Adding another leaf type (e.g., Shortcut, Image, Video) requires
   modifying existing client code everywhere.

❌ Violates Open/Closed Principle
   Existing code must be changed whenever new object types are added.

===============================================================================
*/