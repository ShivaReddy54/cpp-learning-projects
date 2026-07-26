# Design Patterns

Design patterns are **reusable solutions** to commonly occurring problems in software design.

> First popularized by the **Gang of Four (GoF)** in their 1994 book *Design Patterns: Elements of Reusable Object-Oriented Software*.

---

# Types of Design Patterns

There are **3 main categories** containing **23 classic GoF design patterns**.

- **Creational Patterns (5)** → How objects are created
- **Structural Patterns (7)** → How objects are organized
- **Behavioral Patterns (11)** → How objects communicate

---

# 1. Creational Patterns (5)

**Focus:** How objects are **created**

These patterns abstract the object creation process and hide instantiation complexity.

| # | Pattern | One-Line Idea |
|---|---------|---------------|
| 1 | **Singleton** | Ensure only **one** instance of a class exists |
| 2 | **Factory Method** | Delegate object creation to subclasses |
| 3 | **Abstract Factory** | Create families of related objects |
| 4 | **Builder** | Construct complex objects step by step |
| 5 | **Prototype** | Clone existing objects instead of creating new ones |

## Quick Decision Guide

| Requirement | Pattern |
|-------------|---------|
| Need only one instance? | **Singleton** |
| Need one product with extensibility? | **Factory Method** |
| Need a family of related products? | **Abstract Factory** |
| Need step-by-step construction? | **Builder** |
| Need to clone expensive objects? | **Prototype** |

---

# 2. Structural Patterns (7)

**Focus:** How objects are **structured and composed**

These patterns define relationships between classes and objects to build larger structures.

| # | Pattern | One-Line Idea |
|---|---------|---------------|
| 1 | **Adapter** | Make incompatible interfaces work together |
| 2 | **Bridge** | Separate abstraction from implementation |
| 3 | **Composite** | Treat individual and group objects uniformly |
| 4 | **Decorator** | Add behavior dynamically |
| 5 | **Facade** | Provide a simplified interface to a complex subsystem |
| 6 | **Flyweight** | Share common state to reduce memory usage |
| 7 | **Proxy** | Control access to another object |

## Quick Decision Guide

| Requirement | Pattern |
|-------------|---------|
| Incompatible interfaces? | **Adapter** |
| Abstraction tightly coupled with implementation? | **Bridge** |
| Need a part-whole hierarchy? | **Composite** |
| Need dynamic functionality? | **Decorator** |
| Need a simple interface to a complex subsystem? | **Facade** |
| Too many similar objects consuming memory? | **Flyweight** |
| Need access control, caching, or lazy loading? | **Proxy** |

---

# 3. Behavioral Patterns (11)

**Focus:** How objects **communicate and share responsibilities**

These patterns manage interactions, communication, and algorithms between objects.

| # | Pattern | One-Line Idea |
|---|---------|---------------|
| 1 | **Chain of Responsibility** | Pass requests through multiple handlers |
| 2 | **Command** | Encapsulate a request as an object |
| 3 | **Interpreter** | Define a grammar and interpret expressions |
| 4 | **Iterator** | Traverse collections uniformly |
| 5 | **Mediator** | Centralize communication between objects |
| 6 | **Memento** | Save and restore an object's state |
| 7 | **Observer** | Notify dependent objects of state changes |
| 8 | **State** | Change behavior based on internal state |
| 9 | **Strategy** | Swap algorithms at runtime |
| 10 | **Template Method** | Define an algorithm skeleton and let subclasses customize steps |
| 11 | **Visitor** | Add operations without modifying existing classes |

## Quick Decision Guide

| Requirement | Pattern |
|-------------|---------|
| Multiple handlers should try a request? | **Chain of Responsibility** |
| Need undo/redo or queued operations? | **Command** |
| Need to evaluate a custom language? | **Interpreter** |
| Need uniform collection traversal? | **Iterator** |
| Too much object-to-object communication? | **Mediator** |
| Need history or snapshots? | **Memento** |
| Need automatic notifications? | **Observer** |
| Behavior depends on current state? | **State** |
| Need interchangeable algorithms? | **Strategy** |
| Common algorithm with customizable steps? | **Template Method** |
| Need new operations without modifying classes? | **Visitor** |

---

# Similar Patterns — How They Differ

| Comparison | Pattern A | Pattern B |
|------------|-----------|-----------|
| **Strategy vs State** | Client chooses the algorithm | Object changes behavior based on its internal state |
| **Command vs Strategy** | Encapsulates an action (undoable) | Encapsulates an algorithm (swappable) |
| **Observer vs Mediator** | One-to-many notifications | One-to-one coordination through a mediator |
| **Memento vs Command** | Saves an object's state | Saves operations for undo/redo |
| **Template Method vs Strategy** | Inheritance-based | Composition-based |
| **Chain of Responsibility vs Strategy** | Multiple handlers process sequentially | Client selects one algorithm |
| **Visitor vs Strategy** | Add new operations | Swap algorithms |

---

# Summary of All 23 GoF Design Patterns

## Creational Patterns (5)

| Pattern | Purpose |
|---------|---------|
| Singleton | One instance with global access |
| Factory Method | Subclass decides what to create |
| Abstract Factory | Create families of related objects |
| Builder | Step-by-step object construction |
| Prototype | Clone existing objects |

---

## Structural Patterns (7)

| Pattern | Purpose |
|---------|---------|
| Adapter | Convert incompatible interfaces |
| Bridge | Decouple abstraction from implementation |
| Composite | Uniform treatment of tree structures |
| Decorator | Dynamically add responsibilities |
| Facade | Simplify complex subsystems |
| Flyweight | Reduce memory by sharing state |
| Proxy | Control access to another object |

---

## Behavioral Patterns (11)

| Pattern | Purpose |
|---------|---------|
| Chain of Responsibility | Pass requests through handlers |
| Command | Encapsulate requests |
| Interpreter | Interpret language grammar |
| Iterator | Traverse collections |
| Mediator | Centralize communication |
| Memento | Save and restore state |
| Observer | Notify dependents |
| State | Behavior changes with state |
| Strategy | Runtime algorithm selection |
| Template Method | Algorithm skeleton |
| Visitor | Add operations without modifying classes |

---

# Complete List

```
Design Patterns
│
├── Creational (5)
│   ├── Singleton
│   ├── Factory Method
│   ├── Abstract Factory
│   ├── Builder
│   └── Prototype
│
├── Structural (7)
│   ├── Adapter
│   ├── Bridge
│   ├── Composite
│   ├── Decorator
│   ├── Facade
│   ├── Flyweight
│   └── Proxy
│
└── Behavioral (11)
    ├── Chain of Responsibility
    ├── Command
    ├── Interpreter
    ├── Iterator
    ├── Mediator
    ├── Memento
    ├── Observer
    ├── State
    ├── Strategy
    ├── Template Method
    └── Visitor
```