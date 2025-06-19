#!/usr/bin/env python3

import math
import sys

class ContinuedFraction:
    def __init__(self, root):
        self.root = root
        self.approx = math.isqrt(root)
        self.alpha = [0, 1, 1]
        self.convergents = [[0, 1], [1, 0]]

    def quotient(self):
        a, b, c = self.alpha
        return (a + b * self.approx) // c

    def next_alpha(self):
        a, b, c = self.alpha
        r = self.quotient() * c - a
        alpha = [r * c, b * c, b**2 * self.root - r**2]
        d = math.gcd(*alpha)
        return [x // d for x in alpha]

    def next_convergent(self):
        return [prev + self.quotient() * curr for prev, curr in zip(*self.convergents)]

    def advance(self):
        self.convergents = [*self.convergents, self.next_convergent()][1:]
        self.alpha = self.next_alpha()

    def run(self):
        while self.alpha != [self.approx, 1, 1]:
            self.advance()
        return self

class QuadraticInteger:
    def __init__(self, real, imag, root):
        self.real = real
        self.imag = imag
        self.root = root

    def mult(self, other):
        real = self.real * other.real + self.imag * other.imag * self.root
        imag = self.real * other.imag + self.imag * other.real
        return QuadraticInteger(real, imag, self.root)

    def power(self, exponent):
        if exponent == 0:
            return QuadraticInteger(1, 0, self.root)
        if exponent == 1:
            return self
        
        half_power = self.power(exponent // 2)
        if exponent % 2 == 0:
            return half_power.mult(half_power)
        return self.mult(half_power.mult(half_power))

class CattleProblem:
    def __init__(self):
        self.part_one = 50389082
        self.root = 4729494

    def first_solution(self):
        return ContinuedFraction(self.root).run().convergents[-1]

    def y_value(self):
        return QuadraticInteger(*self.first_solution(), self.root).power(2329).imag

    def multiplier(self):
        return (self.y_value() ** 2 * 957) // 18628

    def part_two(self):
        return str(self.part_one * self.multiplier())

sys.set_int_max_str_digits(210000)
SOLUTION = CattleProblem().part_two()
NUMBER_OF_DIGITS = len(SOLUTION)

print(f"{SOLUTION}\n\nsolution: {SOLUTION[:25]}...{SOLUTION[-25:]}\n\nnumber of digits: {NUMBER_OF_DIGITS}")

