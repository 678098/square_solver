import random


def generate_value() -> int:
	return random.randint(-1000, 1000)


def generate_value_big() -> int:
	return random.randint(int(-1e100), int(1e100))


def generate_garbage() -> str:
	garbage_symbols = 'abcdefghijklmnopqrstuvwxyz'
	chars = [random.choice(garbage_symbols) for _ in range(random.randint(4, 10))] + list(str(generate_value()))
	random.shuffle(chars)
	return ''.join(chars)


def write(file, coeff: int) -> None:
	file.write(str(coeff) + ' ')


def write_with_garbage(file, coeff: int) -> None:
	file.write(str(coeff) + ' ')
	if random.uniform(0.0, 1.0) < 0.3:
		file.write(generate_garbage() + ' ')


def generate_tasks(num: int = 10000, with_garbage: bool = True, with_big_values: bool = True) -> None:
	gen = generate_value_big if with_big_values else generate_value
	wr = write_with_garbage if with_garbage else write

	coeffs = []
	for i in range(num):
		x1 = gen()
		x2 = gen()
		a = gen()
		b = - a * (x1 + x2)
		c = a * x1 * x2

		coeffs += [a, b, c]

	with open('task.txt', 'w') as f:
		for coeff in coeffs:
			wr(f, coeff)


if __name__ == "__main__":
	generate_tasks(num=10000, with_garbage=True, with_big_values=False)
