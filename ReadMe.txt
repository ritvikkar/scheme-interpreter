README

This is a Scheme interpreter in C.

All the required functions in this project work to the best of our knowledge.

Additional Features
	- Interactive Module
		The interactive module is implemented runs until EOF but
			- doesn't support history and editing
			- exits on incorrect Scheme input
		The global environment is preserved between calls to interpreter so:
			> (define x 3)
			> x
		will return 3
	- Garbage Collector
		A structure for the garbage collector is implemented
			Due to the lack of time, we haven't been able to fully debug it, therefore
			it is not a part of the actual interpreter. The source code is present
			in the repository.
	- Load
		Our load function works as a general purpose function that processes a scheme file
		The terminal command is : ./interpreter file.ss
		Once all the command line arguments are interpreted, the interpreter enters interactive mode

	- Library Functions
		lists.ss
			Implemented. Handles all the built-in functions except
				member
				cons*
				memp
				remq
				remp
				filter
				find
				assp
		math.ss
			Implemented. Handles the following the built-in functions
				>=
				>
				=
				zero?
				positive?
				negative?
				square
				abs
