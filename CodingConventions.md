# Coding conventions
Most of the coding conventions used in this project are implicit. However, to clear up any misunderstandings about inconsistency, here are some general rules.

## Documentation style pending
I'mma trying my best! Just keeping the code as clean and neatly documented as possible.

## C++11 iterators preferred
Exception is when indices are required. In that case, it's up to my jurisdiction, but an integer iterator will be preferred if the index is needed and the actual item is accessed only one or two times.

## C++11 in-class initialisers allowed  
It's far too easy to forget to initialise variables, and this format is highly readable and convenient for multiple constructors. The only disadvantage is lack of backward compatibility with C++98, which is a non-issue in this project.

## Inlines used for short functions
Expect short-and-sweet functions to be inlined and larger ones to be declared in the cpp file.

## Inline function definitions are separate from their declaration
This generally makes C++ classes easier to read and document.

### Except in Math functions
Rationale for the math exception is that most math functions are inline, and the header is very large. As a programmer it's easier and faster to write and update math functions where they are. This may change in the future noting the aformentioned benefits of separate definitions.