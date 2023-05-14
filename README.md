# Homework 4 - Web Client

#### Daraban Albert-Timotei

### Commands

* REGISTER - reads credentials and sends a register request
* LOGIN - reads credentials and sends login request to the server, if a user is already connect he will be logged out
* ENTER_LIBRARY - sends library access request
* GET_BOOKS - sends request for all books details, if the request is good the details will be printed in JSON pretty print format
* GET_BOOK - reads book id and sends request for a book details, if the request is good the details will be printed in JSON pretty print format
* ADD_BOOK - reads book details and sends a request to add the book
* DELETE_BOOK - reads book id and sends a request to delete book
* LOGOUT - if logged in sends logout request to server
* EXIT - if logged in sends logout request to server and closes program

### Structure

* CLIENT connects the following parts of the program:
	* INPUT_PARSER - reads commands and any other input from the keyboard and parses them
	* SERVER_COM - communicates with the server and uses:
		* HELPERS - ensures the correct sending and receiving of data to the server and uses:
			* BUFFER - provides a resizable buffer
		* REQUESTS - creates request buffers

### JSON Parsing

For JSON parsing, we chose https://github.com/nlohmann/json because it is very intuitive to use.