Purpose of this library:

Object Oriented Programming allocates memory that is not efficient when processing large amounts of data

Data Oriented Design is much more cache friendly when running a program that iterates through large amounts of data, but does not have a kind syntax for programmers in the same way OOP does.

Fix:

Combine them both!

Develop a library that allows for closely similar syntax to OOP, but allocates memory in a data oriented design.


We're going to strictly brainstorm on design first.

class declaration:

class Person : soa::obj{

public/private:
    string name
    string dob
    int age
    string hairColor
    string skinColor
    bool hasGlasses
}


Player* player = new Player();

//or 

Player player;


Able to use instance variables through pointer access 

player->name



