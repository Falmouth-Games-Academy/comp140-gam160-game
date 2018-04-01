# Coding pains
Here lies a historical archive of coding pains, the programmer's equivalent of constipation (assuming writing code was the programmer's equivalent of diarrhea). Conceptual and technical issues I encountered during this journey.  

## Iterators and object deletions
I'm just innocently iterating through the game object list, when a function I call on an objects decides to spawn more objects. This is totally fine, except now my iterator is dead as the object list was reallocated.  

Possible fixes:  
* Use a linked list for the objects?
* Use a pending new object list for new objects?

Current gum-glued 'solution': Avoid iterators when handling the object list