#include <iostream>
#include <vector>
#include <string>

// ===== STEP 1: Iterator interface =====
class Iterator{
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() = 0;
    virtual std::string next() = 0;
    virtual void reset() = 0;
};

// ===== STEP 2: Collection interface =====
class IterableCollection{
public:
    virtual ~IterableCollection() = default;
    virtual Iterator* createIterator() = 0;
    virtual Iterator* createReverseIterator() = 0;
};

// ===== STEP 3: Playlist + its Iterator =====
class Playlist : public IterableCollection{
private:
    std::vector<std::string> songs;

public:
    void addSong(const std::string& song) { songs.push_back(song); }
    std::string getSongAt(int index) { return songs[index]; }
    int getCount() { return songs.size(); }

    Iterator* createIterator() override;
    Iterator* createReverseIterator() override;
};

class PlaylistIterator : public Iterator{
private:
    Playlist* playlist;
    int currentIndex;

public:
    PlaylistIterator(Playlist* p) : playlist(p), currentIndex(0) {}

    bool hasNext() override{
        return currentIndex < playlist->getCount();
    }

    std::string next() override{
        return playlist->getSongAt(currentIndex++);
    }

    void reset() override{
        currentIndex = 0;
    }
};

class PlaylistReverseIterator : public Iterator{
private:
    Playlist* playlist;
    int currentIndex;

public:
    PlaylistReverseIterator(Playlist* p) : playlist(p), currentIndex(p->getCount() - 1) {}

    bool hasNext() override{
        return currentIndex >= 0;
    }

    std::string next() override{
        return playlist->getSongAt(currentIndex--);
    }

    void reset() override{
        currentIndex = playlist->getCount() - 1;
    }
};

Iterator* Playlist::createIterator() { return new PlaylistIterator(this); }
Iterator* Playlist::createReverseIterator() { return new PlaylistReverseIterator(this); }

// ===== STEP 4: Bookshelf + its Iterator =====
class Bookshelf : public IterableCollection{
private:
    std::string books[5];
    int count;

public:
    Bookshelf() : count(0) {}
    void addBook(const std::string& book) { books[count++] = book; }
    std::string getBookAt(int index) { return books[index]; }
    int getCount() { return count; }

    Iterator* createIterator() override;
    Iterator* createReverseIterator() override;
};

class BookshelfIterator : public Iterator{
private:
    Bookshelf* bookshelf;
    int currentIndex;

public:
    BookshelfIterator(Bookshelf* b) : bookshelf(b), currentIndex(0) {}

    bool hasNext() override{
        return currentIndex < bookshelf->getCount();
    }

    std::string next() override{
        return bookshelf->getBookAt(currentIndex++);
    }

    void reset() override{
        currentIndex = 0;
    }
};

class BookshelfReverseIterator : public Iterator{
private:
    Bookshelf* bookshelf;
    int currentIndex;

public:
    BookshelfReverseIterator(Bookshelf* b) : bookshelf(b), currentIndex(b->getCount() - 1) {}

    bool hasNext() override{
        return currentIndex >= 0;
    }

    std::string next() override{
        return bookshelf->getBookAt(currentIndex--);
    }

    void reset() override{
        currentIndex = bookshelf->getCount() - 1;
    }
};

Iterator* Bookshelf::createIterator() { return new BookshelfIterator(this); }
Iterator* Bookshelf::createReverseIterator() { return new BookshelfReverseIterator(this); }

// ===== CLIENT CODE — UNIFORM iteration for ALL collections! =====
void iterateCollection(Iterator* iterator, const std::string& label) {
    std::cout << "=== " << label << " ===" << std::endl;
    iterator->reset();
    while(iterator->hasNext()) std::cout << "  " << iterator->next() << std::endl;
}

int main() {
    Playlist playlist;
    playlist.addSong("Song A");
    playlist.addSong("Song B");
    playlist.addSong("Song C");

    Bookshelf bookshelf;
    bookshelf.addBook("Book 1");
    bookshelf.addBook("Book 2");
    bookshelf.addBook("Book 3");

    // ===== Forward iteration — SAME code for BOTH collections! =====
    std::cout << "--- Forward Iteration ---" << std::endl;
    Iterator* playlistIt = playlist.createIterator();
    Iterator* bookshelfIt = bookshelf.createIterator();

    iterateCollection(playlistIt, "Playlist (Forward)");
    iterateCollection(bookshelfIt, "Bookshelf (Forward)");

    // ===== Reverse iteration — SAME code, different iterator! =====
    std::cout << std::endl << "--- Reverse Iteration ---" << std::endl;
    Iterator* playlistReverseIt = playlist.createReverseIterator();
    Iterator* bookshelfReverseIt = bookshelf.createReverseIterator();

    iterateCollection(playlistReverseIt, "Playlist (Reverse)");
    iterateCollection(bookshelfReverseIt, "Bookshelf (Reverse)");

    // Client NEVER knows how data is stored internally!
    // Playlist uses vector, Bookshelf uses raw array
    // But iteration is IDENTICAL!

    delete playlistIt;
    delete bookshelfIt;
    delete playlistReverseIt;
    delete bookshelfReverseIt;

}

/*
===============================================================================
                    Iterator Pattern: Before vs After
===============================================================================

BEFORE (Without Iterator)

Playlist

    for (int i = 0; i < count; i++)
        songs[i];          // std::vector access

Bookshelf

    for (int i = 0; i < count; i++)
        books[i];          // Raw array access

Every collection requires its own traversal logic because the client knows
how the collection stores its elements.


Problems:
  ❌ Internal representation is exposed.
  ❌ Different iteration code for every collection.
  ❌ Client depends on collection implementation.
  ❌ Difficult to change internal data structures.
  ❌ No standard support for reverse or custom traversal.


===============================================================================

AFTER (Using Iterator)

Iterator* it = collection.createIterator();

while (it->hasNext()) {
    auto item = it->next();
    // Process item
}

Exactly the same traversal code works for every collection, regardless of
whether it uses an array, vector, linked list, tree, or any other structure.


Additional Possibilities

Forward Traversal
    collection.createIterator();

Reverse Traversal
    collection.createReverseIterator();

Filtered Traversal
    collection.createFilteredIterator();


Result:
  ✅ Internal representation remains hidden.
  ✅ One uniform interface (hasNext() / next()).
  ✅ Collection implementation can change without affecting clients.
  ✅ Easy to support reverse and custom traversal strategies.

===============================================================================
*/