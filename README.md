# SPacker
[![Build Status](https://travis-ci.org/JosephRyanPeterson/SPacker.svg?branch=master)](https://travis-ci.org/JosephRyanPeterson/SPacker)
![Code Climate](https://img.shields.io/codeclimate/github/JosephRyanPeterson/SPacker.svg)

Pack a set of spheres into an object.

# Compiling
Clone repository:

    git clone https://github.com/JosephRyanPeterson/SPacker.git

Move to the **src** directory and edit **Makefile** to use your specified compiler. Then merely execute *make*:

    make

# Input File
The format of the input file is a raw text file:

    shapeType<tab>param1value<tab>param2value...
    name1<tab>count1<tab>radius1
    name2<tab>count2<tab>radius2
	...
    
Currently, the only shape type is "spherical" which takes a single parameter for the radius. Names can be any string literal.

# Usage

    spacker -i [input file] -o [output file] [OPTIONAL: -h -a [absolute tolerance] -o [relative tolerance]]

