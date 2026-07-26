#include <iostream>
#include <string>
#include <stack>

// ===== STEP 1: Memento — stores snapshot of state (immutable!) =====
class EditorMemento{
private:
    std::string content;
    int cursorPosition;
    std::string font;

    // Memento is immutable — only the originator can create it
    friend class TextEditor;  // Only TextEditor can access private data

    EditorMemento(const std::string& c, int cursor, const std::string& f)
        : content(c), cursorPosition(cursor), font(f) {}

public:
    // No public setters — memento is read-only!
    // Only getters for the originator (TextEditor) to restore state
};

// ===== STEP 2: Originator — creates mementos and restores from them =====
class TextEditor{
private:
    std::string content;
    int cursorPosition;
    std::string font;

public:
    TextEditor() : content(""), cursorPosition(0), font("Arial") {}

    // Save current state into a memento
    EditorMemento* save(){
        std::cout << "Saving state: '" << content << "', cursor: " 
                  << cursorPosition << ", font: " << font << std::endl;
        return new EditorMemento(content, cursorPosition, font);
    }

    // Restore state from a memento
    void restore(EditorMemento* memento){
        // Only TextEditor can access memento's private fields (friend class)
        this->content = memento->content;
        this->cursorPosition = memento->cursorPosition;
        this->font = memento->font;
        std::cout << "Restored state: '" << content << "', cursor: " 
                  << cursorPosition << ", font: " << font << std::endl;
    }

    void type(const std::string& text){
        content.insert(cursorPosition, text);
        cursorPosition += text.length();
        std::cout << "Typed: '" << text << "' | Content: '" << content << "' | Cursor: " << cursorPosition << std::endl;
    }

    void setFont(const std::string& f){
        font = f;
        std::cout << "Font set to: " << font << std::endl;
    }

    void deleteText(int length){
        if(cursorPosition >= length){
            content.erase(cursorPosition - length, length);
            cursorPosition -= length;
        }
        std::cout << "Deleted " << length << " chars | Content: '" << content << "'" << std::endl;
    }

    void display(){
        std::cout << "--- Editor State ---" << std::endl;
        std::cout << "Content: '" << content << "'" << std::endl;
        std::cout << "Cursor: " << cursorPosition << std::endl;
        std::cout << "Font: " << font << std::endl;
    }
};

// ===== STEP 3: Caretaker — manages mementos (history) =====
class EditorHistory {
private:
    std::stack<EditorMemento*> history;  // Stack for undo (LIFO)

public:
    void push(EditorMemento* memento) {
        history.push(memento);
    }

    EditorMemento* pop(){
        if(history.empty()){
            std::cout << "History: No more states to undo!" << std::endl;
            return nullptr;
        }
        EditorMemento* memento = history.top();
        history.pop();
        return memento;
    }

    bool hasHistory(){
        return !history.empty();
    }

    ~EditorHistory(){
        while(!history.empty()){
            delete history.top();
            history.pop();
        }
    }
};

// ===== CLIENT CODE =====
int main() {
    TextEditor editor;
    EditorHistory history;  // Caretaker manages mementos

    // ===== Initial state =====
    editor.display();

    std::cout << std::endl;

    // ===== Type "Hello" and SAVE =====
    history.push(editor.save());  // Save snapshot BEFORE next action
    editor.type("Hello");
    editor.display();

    std::cout << std::endl;

    // ===== Type " World" and SAVE =====
    history.push(editor.save());
    editor.type(" World");
    editor.display();

    std::cout << std::endl;

    // ===== Change font and SAVE =====
    history.push(editor.save());
    editor.setFont("Courier");
    editor.display();

    std::cout << std::endl;

    // ===== UNDO: Undo font change =====
    std::cout << "=== UNDO 1: Undo font change ===" << std::endl;
    editor.restore(history.pop());
    editor.display();

    std::cout << std::endl;

    // ===== UNDO: Undo " World" =====
    std::cout << "=== UNDO 2: Undo typing ' World' ===" << std::endl;
    editor.restore(history.pop());
    editor.display();

    std::cout << std::endl;

    // ===== UNDO: Undo "Hello" =====
    std::cout << "=== UNDO 3: Undo typing 'Hello' ===" << std::endl;
    editor.restore(history.pop());
    editor.display();

    std::cout << std::endl;

    // ===== No more undo =====
    std::cout << "=== UNDO 4: No more history ===" << std::endl;
    editor.restore(history.pop());

    // Memento pattern preserves encapsulation:
    // - Client (caretaker) only sees EditorMemento pointers
    // - Client can't read or modify memento contents
    // - Only TextEditor (originator) can access memento state (friend class)

}


/*
===============================================================================
                     Memento Pattern: Before vs After
===============================================================================

BEFORE (Without Memento)

    ❌ No Undo functionality
    ❌ No saved history of object states
    ❌ Previous states must be reconstructed manually
    ❌ Encapsulation is broken because external code must inspect
       and recreate the object's internal state


===============================================================================

AFTER (Using Memento)

                    EditorHistory (Caretaker)
                              │
                              │
                    stack<EditorMemento*>
                              │
                ┌─────────────┼─────────────┐
                │             │             │
                ▼             ▼             ▼
        Memento #1      Memento #2     Memento #3
      ---------------- ---------------- ----------------
      Text   : ""      Text   : Hello  Text   : Hello World
      Cursor : 0       Cursor : 5      Cursor : 11
      Font   : Arial   Font   : Arial  Font   : Arial
      ---------------- ---------------- ----------------


Undo Operation

    history.pop()
          │
          ▼
    editor.restore(previousMemento)

The editor immediately returns to its previous state.


===============================================================================

Encapsulation is Preserved

    • Memento stores the complete snapshot of the editor.
    • Memento's data members remain private.
    • Only the Originator (Editor) can access or restore them
      (typically using a friend relationship).
    • The Caretaker (EditorHistory) only stores snapshots and
      never reads or modifies their contents.


Result:

  ✅ Multi-level Undo support.
  ✅ Object snapshots stored in a history stack.
  ✅ Full encapsulation preserved.
  ✅ Caretaker manages history without knowing internal details.

===============================================================================
*/