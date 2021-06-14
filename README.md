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

Get usage with example command:

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

## Data generation

You can generate files with quadratic equation coefficients via **task_generator.py** script:

```python
if __name__ == "__main__":
	generate_tasks(num=10000, with_garbage=True, with_big_values=True)
```

After launching it you will find tasks.txt with specified number of quadratic equations (**num**), with or withour garbage lexemes (**with_garbage**), with or without very long integer coeffs (**with_big_values**).

## Long arithmetics

Consider really huge coeffs:

```commandline
-3179500447746179553711436123703232494548907607983239711710427957328960772952539124971478241832955824
-9.935787352823903026375102790095971371944822173524796551603572603117977310965454368186563787536200084e+198
4.171730474139252760573050847560572575713343306443815654037860875839790492853719003905582697559330137e+297)
```

When square_solver is built without boost multiprecision, results are not correct:

```commandline
(-3.17950044774618e+99 -9.9357873528239e+198 4.17173047413925e+297) => (inf, -inf)
```

When square_solver is built with boost multiprecision, results are fine:

```commandline
(-3179500447746179553711436123703232494548907607983239711710427957328960772952539124971478241832955824 -9.935787352823903026375102790095971371944822173524796551603572603117977310965454368186563787536200084e+198 4.171730474139252760573050847560572575713343306443815654037860875839790492853719003905582697559330137e+297) => (374893910256706927403708359464029944062082332672825961307558481277663716064943364012661188757984783, -3499846246673889851805989356239824449363692003813160599600218377588657824243943969513607292674175744)
```
