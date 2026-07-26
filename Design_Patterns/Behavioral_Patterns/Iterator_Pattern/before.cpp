/*

Intent: Provide a way to access the elements of an aggregate object sequentially without exposing its underlying representation.

EX: A TV remote with channel buttons. You press next/previous to cycle through channels. 
    You don't know how the TV stores channels internally (array? linked list? hashmap?) — you just iterate through them one by one.

*/

#include <iostream>
#include <vector>
#include <string>

// ===== Different collections with DIFFERENT internal structures =====
class Playlist{
private:
    std::vector<std::string> songs;  // Uses vector (array)

public:
    void addSong(const std::string& song) { songs.push_back(song); }
    std::vector<std::string>& getSongs() { return songs; }  // Exposes internals!
    int getCount() { return songs.size(); }
};

class Bookshelf{
private:
    std::string books[5];  // Uses raw array
    int count;

public:
    Bookshelf() : count(0) {}
    void addBook(const std::string& book) { books[count++] = book; }
    std::string* getBooks() { return books; }  // Exposes internals!
    int getCount() { return count; }
};

// ===== PROBLEM: Client must know EACH collection's internal structure =====
int main() {
    Playlist playlist;
    playlist.addSong("Song A");
    playlist.addSong("Song B");
    playlist.addSong("Song C");

    Bookshelf bookshelf;
    bookshelf.addBook("Book 1");
    bookshelf.addBook("Book 2");
    bookshelf.addBook("Book 3");

    // Iterate Playlist — must know it uses vector
    std::cout << "=== Playlist (vector iteration) ===" << std::endl;
    std::vector<std::string>& songs = playlist.getSongs();
    for(int i = 0; i < playlist.getCount(); i++) std::cout << "  " << songs[i] << std::endl;

    // Iterate Bookshelf — must know it uses raw array
    std::cout << "=== Bookshelf (array iteration) ===" << std::endl;
    std::string* books = bookshelf.getBooks();
    for(int i = 0; i < bookshelf.getCount(); i++) std::cout << "  " << books[i] << std::endl;

    // PROBLEMS:
    // 1. Exposed internals — getSongs() returns internal vector reference!
    // 2. Different iteration code for each collection type
    // 3. Can't iterate uniformly — must know internal structure
    // 4. No common traversal interface
    // 5. If Playlist changes from vector to linked list, ALL client code breaks!

}

/*
===============================================================================
                   Problems Without the Iterator Pattern
===============================================================================

Different collections require different traversal code.

Playlist Iteration

    for (int i = 0; i < count; i++)
        songs[i];

Uses:
    • std::vector::operator[]


Bookshelf Iteration

    for (int i = 0; i < count; i++)
        books[i];

Uses:
    • Raw array indexing


Although both loops perform the same task, the client must know how each
collection stores its elements.


Problems:

❌ Exposes Internal Representation
   The client must know whether the collection uses an array, vector,
   linked list, tree, or another data structure.

❌ Different Iteration Logic
   Every collection requires its own traversal code, reducing code reuse.

❌ No Uniform Traversal Interface
   Clients cannot iterate over different collections in the same way.

❌ Fragile Client Code
   Changing the collection's internal implementation forces changes in
   every client that traverses it.

❌ Poor Reusability
   Generic algorithms cannot work with multiple collection types unless
   they know each collection's internal structure.

❌ Limited Traversal Control
   Supporting forward, backward, filtered, or custom traversal requires
   rewriting iteration logic for every collection.

===============================================================================
*/