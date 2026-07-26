/*

Intent: Capture and store an object's internal state so that the object can be restored to this state later, without violating encapsulation.

EX: A text editor with Undo. Every time you type, the editor saves a snapshot (memento) of the document state.
    Press Undo → the editor restores the previous memento.
    The document doesn't know how snapshots are stored — encapsulation preserved.

*/

#include <iostream>
#include <string>
#include <vector>

// ===== Text Editor — no undo capability =====
class TextEditor{
private:
    std::string content;
    int cursorPosition;
    std::string font;

public:
    TextEditor() : content(""), cursorPosition(0), font("Arial") {}

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
        std::cout << "Deleted " << length << " chars | Content: '" << content << "' | Cursor: " << cursorPosition << std::endl;
    }

    void display(){
        std::cout << "--- Editor State ---" << std::endl;
        std::cout << "Content: '" << content << "'" << std::endl;
        std::cout << "Cursor: " << cursorPosition << std::endl;
        std::cout << "Font: " << font << std::endl;
    }
};

int main() {
    TextEditor editor;

    editor.type("Hello");
    editor.type(" World");
    editor.setFont("Courier");

    // PROBLEM: I want to undo " World" — but HOW?
    // No saved states! No undo mechanism!
    // I'd have to manually reconstruct the previous state:
    //   - Remember the content was "Hello"
    //   - Remember cursor was at 5
    //   - Remember font was Arial (or was it Courier?)

    // PROBLEM: Exposing all fields for manual state management
    // violates encapsulation — client shouldn't know internals!

    // PROBLEM: Can't save/restore state without breaking encapsulation

    editor.display();

}

/*
===============================================================================
                    Problems Without the Memento Pattern
===============================================================================

Editor State Changes

    editor.type("Hello")
        → State:
            Text   : "Hello"
            Cursor : 5
            Font   : Arial

    editor.type(" World")
        → State:
            Text   : "Hello World"
            Cursor : 11
            Font   : Arial

    editor.setFont("Courier")
        → State:
            Text   : "Hello World"
            Cursor : 11
            Font   : Courier


Now suppose the user wants to undo...

    UNDO "setFont"
        → Previous state required... but it was never saved!

    UNDO " World"
        → Need the editor state before typing... but it's gone!


Problems:

❌ No Undo / Redo Support
   Once the current state changes, previous states are lost forever.

❌ No Saved Snapshots
   There is no history of states that can be restored later.

❌ Manual State Reconstruction
   The client must manually remember and rebuild every field,
   violating encapsulation and becoming error-prone.

❌ No Multi-Level Undo
   Without storing multiple snapshots, only the latest state (if any)
   can be restored.

❌ No Selective Restoration
   Restoring previous versions of the object's state becomes difficult
   because no historical snapshots exist.

===============================================================================
*/