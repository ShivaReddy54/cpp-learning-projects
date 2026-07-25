#include <iostream>
#include <string>
#include <vector>

// ===== STEP 1: Component — common interface for all objects =====
class FileSystemComponent{
public:
    virtual ~FileSystemComponent() = default;
    virtual std::string getName() = 0;
    virtual int getSize() = 0;
    virtual void display(int indent = 0) = 0;

    // Operations for Composite (default: leaf can't add/remove)
    virtual void add(FileSystemComponent* component) {
        std::cout << "Cannot add to a leaf" << std::endl;
    }
    virtual void remove(FileSystemComponent* component) {
        std::cout << "Cannot remove from a leaf" << std::endl;
    }
    virtual bool isComposite() { return false; }
};

// ===== STEP 2: Leaf — has no children =====
class File : public FileSystemComponent{
private:
    std::string name;
    int size;

public:
    File(const std::string& n, int s) : name(n), size(s) {}

    std::string getName() override { return name; }
    int getSize() override { return size; }

    void display(int indent = 0) override{
        std::string padding(indent, ' ');
        std::cout << padding << "File: " << name << " (" << size << " KB)" << std::endl;
    }

    // Leaf defaults: can't add, can't remove, not composite
    bool isComposite() override { return false; }
};

// ===== STEP 3: Composite — can hold children (leaves or other composites) =====
class Folder : public FileSystemComponent{
private:
    std::string name;
    std::vector<FileSystemComponent*> children;

public:
    Folder(const std::string& n) : name(n) {}

    std::string getName() override{ return name; }

    // Composite calculates size by SUMMING all children
    // Works whether children are Files or other Folders!
    int getSize() override{
        int total = 0;
        for(FileSystemComponent* child : children) total += child->getSize();  // Same call for leaf or composite!
        return total;
    }

    void add(FileSystemComponent* component) override {
        children.push_back(component);
    }

    void remove(FileSystemComponent* component) override {
        // Find and remove
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (*it == component) {
                children.erase(it);
                break;
            }
        }
    }

    bool isComposite() override { return true; }

    void display(int indent = 0) override{
        std::string padding(indent, ' ');
        std::cout << padding << "Folder: " << name << " [" << getSize() << " KB]" << std::endl;
        for (FileSystemComponent* child : children) {
            child->display(indent + 2);  // Same call for leaf or composite!
        }
    }
};

// ===== CLIENT CODE =====
int main() {
    // ===== Build tree structure =====
    FileSystemComponent* file1 = new File("resume.doc", 120);
    FileSystemComponent* file2 = new File("photo.jpg", 500);
    FileSystemComponent* file3 = new File("notes.txt", 30);
    FileSystemComponent* file4 = new File("budget.xlsx", 200);
    FileSystemComponent* file5 = new File("report.pdf", 350);

    FileSystemComponent* documents = new Folder("Documents");
    documents->add(file1);
    documents->add(file3);

    FileSystemComponent* pictures = new Folder("Pictures");
    pictures->add(file2);

    FileSystemComponent* work = new Folder("Work");
    work->add(file4);
    work->add(file5);

    FileSystemComponent* root = new Folder("Home");
    root->add(documents);
    root->add(pictures);
    root->add(work);

    // ===== Client treats ALL components UNIFORMLY =====
    std::cout << "=== Display Tree ===" << std::endl;
    root->display();  // One call handles the entire tree!

    std::cout << std::endl;

    // ===== Same getSize() call works for both leaf and composite =====
    std::cout << "=== Sizes (uniform interface!) ===" << std::endl;
    std::cout << "File size: " << file1->getName() << " = " << file1->getSize() << " KB" << std::endl;
    std::cout << "Folder size: " << documents->getName() << " = " << documents->getSize() << " KB" << std::endl;
    std::cout << "Root size: " << root->getName() << " = " << root->getSize() << " KB" << std::endl;

    std::cout << std::endl;

    // ===== Can treat any component the same way =====
    std::cout << "=== Is Composite? ===" << std::endl;
    std::cout << "File1 is composite? " << (file1->isComposite() ? "Yes" : "No") << std::endl;
    std::cout << "Documents is composite? " << (documents->isComposite() ? "Yes" : "No") << std::endl;

    std::cout << std::endl;

    // ===== Remove a subfolder — still works uniformly =====
    std::cout << "=== After removing Pictures ===" << std::endl;
    root->remove(pictures);
    root->display();
    std::cout << "New root size: " << root->getSize() << " KB" << std::endl;

    // Cleanup (simplified — in real code use smart pointers)
    delete root;     // Should recursively delete children

}


/*
===============================================================================
                   Composite Pattern: Before vs After
===============================================================================

BEFORE (Without Composite)

The client must determine the object's type before performing operations.

if (item is File)
    item.getSize();
else if (item is Folder)
    item.getTotalSize();

Every operation requires similar branching logic:

if (item is File)
    item.display();
else if (item is Folder)
    item.displayFolder();


Problems:
  ❌ Type checking required everywhere.
  ❌ Different interfaces for different object types.
  ❌ Client is tightly coupled to concrete classes.
  ❌ Hard to extend when new component types are added.


===============================================================================

AFTER (Using Composite)

Every object exposes the same interface.

item.getSize();       // Works for both File and Folder
item.display();       // Works for both File and Folder
item.getName();       // Works for both File and Folder
item.isComposite();   // Works for both File and Folder

The client never needs to know whether it is working with a
single object (Leaf) or a group of objects (Composite).


Result:
  ✅ No type checking required.
  ✅ Uniform interface for all components.
  ✅ Client code becomes simpler.
  ✅ Easy to extend by adding new component types.

===============================================================================
*/