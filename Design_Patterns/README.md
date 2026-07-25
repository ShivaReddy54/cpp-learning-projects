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