# What Are Design Patterns?

Design patterns are reusable solutions to commonly occurring problems in software design.

> First popularized by the **"Gang of Four" (GoF)** in their 1994 book.

---

# Types of Design Patterns

There are **3 main categories** containing **23 classic patterns**.

## 1. Creational Patterns (5 Patterns)

**Focus:** How objects are **CREATED**

They abstract the instantiation process, hiding the complexity of object creation.

| # | Pattern | One-Line Idea |
|---|---------|---------------|
| 1 | **Singleton** | Ensure only **ONE** instance of a class exists |
| 2 | **Factory Method** | Delegate object creation to subclasses |
| 3 | **Abstract Factory** | Create families of related objects |
| 4 | **Builder** | Step-by-step construction of complex objects |
| 5 | **Prototype** | Clone existing objects instead of creating from scratch |


### Quick Decision Guide:
─────────────────────
Need only 1 instance?          → Singleton
Need 1 product, extensible?    → Factory Method
Need family of products?       → Abstract Factory
Need step-by-step building?    → Builder
Need to clone expensive objs?  → Prototype

---

## 2. Structural Patterns (7 Patterns)

**Focus:** How objects are **COMPOSED / STRUCTURED** together

They deal with relationships between entities—how classes and objects are combined to form larger structures.

| # | Pattern | One-Line Idea |
|---|---------|---------------|
| 1 | **Adapter** | Make incompatible interfaces work together |
| 2 | **Bridge** | Separate abstraction from implementation |
| 3 | **Composite** | Treat individual and group of objects uniformly |
| 4 | **Decorator** | Add behavior to objects dynamically |
| 5 | **Facade** | Provide a simplified interface to a complex subsystem |
| 6 | **Flyweight** | Share common state across many objects to save memory |
| 7 | **Proxy** | A placeholder/control object for another object |

### Quick Decision Guide:
─────────────────────
Incompatible interfaces?          → Adapter
Abstraction & impl coupled?       → Bridge
Part-whole hierarchy?             → Composite
Need dynamic features?            → Decorator
Complex subsystem, need simple?   → Facade
Too many similar objects?         → Flyweight
Need access control/lazy load?    → Proxy

---

## 3. Behavioral Patterns (11 Patterns)

**Focus:** How objects **COMMUNICATE** and distribute responsibility

They deal with algorithms and the assignment of responsibilities between objects.

| # | Pattern | One-Line Idea |
|---|---------|---------------|
| 1 | **Chain of Responsibility** | Pass request along a chain of handlers |
| 2 | **Command** | Encapsulate a request as an object |
| 3 | **Interpreter** | Define a grammar and interpret sentences |
| 4 | **Iterator** | Sequentially access elements of a collection |
| 5 | **Mediator** | Centralize communication between objects |
| 6 | **Memento** | Capture and restore an object's state |
| 7 | **Observer** | Notify dependent objects of state changes |
| 8 | **State** | Change behavior when internal state changes |
| 9 | **Strategy** | Swap algorithms at runtime |
| 10 | **Template Method** | Define the skeleton of an algorithm, letting subclasses fill in the steps |
| 11 | **Visitor** | Add operations to objects without modifying them |

### ### Quick Decision Guide:
─────────────────────

Need multiple handlers to try a request?      → Chain of Responsibility
Need undo/redo or queue commands?             → Command
Need to evaluate a custom language/grammar?   → Interpreter
Need to traverse a collection uniformly?      → Iterator
Too much object-to-object communication?      → Mediator
Need undo/history or snapshots?               → Memento
Need automatic notifications on changes?      → Observer
Behavior depends on current state?            → State
Need interchangeable algorithms?              → Strategy
Common algorithm with customizable steps?     → Template Method
Need new operations without changing classes? → Visitor

---

┌─────────────────────────────────────────────────────────────────────┐
│              SIMILAR PATTERNS — HOW THEY DIFFER                     │
├──────────────────┬────────────────────┬─────────────────────────────┤
│ Comparison       │ Pattern A          │ Pattern B                   │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Strategy vs State│ Client chooses     │ Object changes itself       │
│                  │ algorithm manually │ based on internal state     │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Command vs       │ Encapsulates       │ Encapsulates                │
│   Strategy       │ action (undoable)  │ algorithm (swappable)       │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Observer vs      │ One-to-many        │ One-to-one mediator         │
│   Mediator       │ notification       │ coordinates interactions    │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Memento vs       │ Saves full state   │ Strategy saved as command   │
│   Command        │ snapshot           │ for undo                    │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Template Method  │ Inheritance-based  │ Composition-based           │
│   vs Strategy    │ (subclass decides) │ (client plugs in strategy)  │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Chain of Resp.   │ Multiple handlers  │ Single handler chosen       │
│   vs Strategy    │ in sequence        │ by client                   │
├──────────────────┼────────────────────┼─────────────────────────────┤
│ Visitor vs       │ Add operations     │ Add element types           │
│   Strategy       │ to existing types  │ with new algorithm variants │
└──────────────────┴────────────────────┴─────────────────────────────┘


## Summary — All 23 Design Patterns
┌────────────────────────────────────────────────────────────────────┐
│                ALL 23 GoF DESIGN PATTERNS                          │
├────────────────────┬───────────────────────────────────────────────┤
│                    │                                               │
│  CREATIONAL (5)    │  HOW to CREATE objects                        │
│  ├── Singleton     │  One instance, global access                  │
│  ├── Factory Method│  Subclass decides which to create             │
│  ├── Abstract Fact │  Create families of related objects           │
│  ├── Builder       │  Step-by-step complex construction            │
│  └── Prototype     │  Clone existing instead of new                │
│                    │                                               │
│  STRUCTURAL (7)    │  HOW to STRUCTURE/COMPOSE objects             │
│  ├── Adapter       │  Convert incompatible interface               │
│  ├── Bridge        │  Decouple abstraction from implementation     │
│  ├── Composite     │  Treat leaf and composite uniformly           │
│  ├── Decorator     │  Add behavior dynamically via wrapping        │
│  ├── Facade        │  Simple interface to complex subsystem        │
│  ├── Flyweight     │  Share intrinsic state to save memory         │
│  └── Proxy         │  Placeholder to control access                │
│                    │                                               │
│  BEHAVIORAL (11)   │  HOW objects COMMUNICATE                      │
│  ├── Chain of Resp │  Pass request along handler chain             │
│  ├── Command       │  Encapsulate request as object                │
│  ├── Interpreter   │  Grammar + interpret as expression tree       │
│  ├── Iterator      │  Traverse collection uniformly                │
│  ├── Mediator      │  Centralize communication between objects     │
│  ├── Memento       │  Capture/restore state for undo               │
│  ├── Observer      │  Notify dependents on state change            │
│  ├── State         │  Change behavior when state changes           │
│  ├── Strategy      │  Swap algorithms at runtime                   │
│  ├── Template Meth │  Algorithm skeleton, subclasses fill steps    │
│  └── Visitor       │  Add operations without modifying classes     │
│                    │                                               │
└────────────────────┴───────────────────────────────────────────────┘