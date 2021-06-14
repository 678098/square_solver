# square_solver

Console application for fast batch solving square equations in long integer arithmetics.

## Building

Make sure that **libboost-dev** is installed if you want to build with boost multiprecision types:

```
git clone https://github.com/678098/square_solver.git
cd square_solver
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_BOOST_MULTIPRECISION=ON ../
make
```

If you don't want to use boost multiprecision and want to use double type instead, just configure with command:

```
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_BOOST_MULTIPRECISION=OFF ../
```

## Launching

Get usage with example commands:

```
./square_solver --help
```

Which returns:

```buildoutcfg
Usage: square_solver [PARAMETERS] [COEFFICIENTS: a1 b1 c1 ... aN [bN [cN]]]
Solving multiple square equations a*x^2 + b*x + c = 0 with given
COEFFICIENTS a b c. Missing trailing coefficients b, c are
considered to be 0. Non-numeric data in arguments is filtered out.

	-h, --help        print usage
	-s, --silent      don't print found solutions
	-p, --perf        measure request processing time
	-i, --stdin       interactive mode: get coefficients from stdin

Examples:
	./square_solver 3 4 5 0 4 -4 11 22 123 45 42 42 67 1001
	./square_solver 1 -2 -3 0 4 -4 11 22
	./square_solver 1 2 1 1 4 1 1 -3 2 0 1 -1 0 0 1 0 0 0
	./square_solver --interactive
	./square_solver --silent --perf `cat task.txt`
	cat task.txt | ./square_solver -i
```

Sample launch:

```commandline
$ ./square_solver 1 2 1 1 4 1 1 -3 2 0 1 -1 0 0 1 0 0 0
(1 2 1) => (-1, -1)
(1 4 1) => (-3.73205080756888, -0.267949192431123)
(1 -3 2) => (1, 2)
(0 1 -1) => (1)
(0 0 1) => no roots
(0 0 0) => any number
```
