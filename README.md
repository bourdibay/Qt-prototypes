Qt-prototypes
=============

Prototypes made with Qt framework for the development of an IDE.

Each directory corresponds to a prototype.
I realized them in the framework of my end-study project called Widev. It is a collaborative IDE made with Qt, and using the LLVM technologies such as libClang and libLLDB.
Each prototype has a test file (usually testWindow.h) which provides a basic on how to use the features embedded in the prototype.
TextEditor and TreeProto are the most important tests I made.
Scintilla may be useful to those who want to use it, and are not satisfied enough with the features provided by QScintilla.

How to build
============
mkdir build; cmake ../; make

On Windows, I provide a script "gen_VS12_x64.bat" script which generates a Visual Studio 2013 solution.
The prototypes should compile with VS2012 too (tested at the beginning of the project, before I moved to VS2013).

Quick description of each prototype
===================================

ListProto: This is my own implementation of a ListModel for a QListView. It has been designed to manage only one type of data (basically a structure), given as a template to the class.
For each type of data, you have to subclass this class and implement the methods data() and setData().

ModelSharing: A basic test on how a single model can be used for two different views.

Scintilla: At the first version of Widev, we used QScintilla for the text editor. For the second version we change it to our own editor (see TextEditor) because of some performance issues.
I wrapped most of features provided by Scintilla, in order to have as many features as possible usable.

SideAreaManagement: This is one of the tests I made to deal with side areas in a basic IDE. There are two horizontal areas (left and right) and a vertical one (bottom).

TextEditor: My own implementation of the text editor, heavily optimized for our own utilization.
I get inspired by https://github.com/aseba-community/aseba/tree/master/clients/studio for the implementation of the breakpoints margin.

TreeProto: My own implementation of a TreeModel for a QTreeView. It supports internal drag'n drop.
