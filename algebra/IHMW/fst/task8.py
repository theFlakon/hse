import numpy

def to_cycles(permutation: numpy.ndarray) -> list[list[int]]:
    new_perm = permutation.copy()

    for idx in range(new_perm.size):
        new_perm[idx] -= 1

    is_added = numpy.zeros(new_perm.size, dtype=bool)
    cycles = []

    for idx in range(new_perm.size):
        if is_added[idx]:
            continue

        curr_cycle = []
        loop_idx = idx

        while not is_added[loop_idx]:
            is_added[loop_idx] = True
            curr_cycle.append(int(loop_idx))  
            loop_idx = int(new_perm[loop_idx])

        if len(curr_cycle) > 1:
            cycles.append([el + 1 for el in curr_cycle])

    return cycles

def calc_ord(permutation: numpy.ndarray) -> int:
    perm_size = permutation.size

    indentity_perm = numpy.arange(1, perm_size + 1)

    current = numpy.copy(permutation)
    order = 1

    while not numpy.array_equal(current, indentity_perm):
        next_current = numpy.zeros_like(current)

        for idx in range(perm_size):
            next_current[idx] = current[permutation[idx] - 1]

        current = next_current
        order += 1

    return order

def raise_the_degree(permutation: numpy.ndarray, degree: int) -> numpy.ndarray:
    perm_len = permutation.size

    if degree == 0:
        return numpy.arange(1, perm_len + 1)
    
    if degree < 0:
        inverse_perm = numpy.zeros_like(permutation)

        for idx in range(perm_len):
            inverse_perm[permutation[idx] - 1] = idx + 1

        base_perm = inverse_perm
        abs_degree = -degree

    else:
        base_perm = permutation.copy()
        abs_degree = degree

    result = base_perm.copy()
    
    for _ in range(abs_degree - 1):
        next_result = numpy.array([result[el - 1] for el in base_perm])
        result = next_result

    return result

my_perm = [5, 3, 8, 9, 4, 7, 6, 2, 1]
degree = -797

numpy_arr = numpy.array(my_perm)

cycles = to_cycles(numpy_arr)
ord = calc_ord(numpy_arr)
raised_perm = raise_the_degree(numpy_arr, degree)

print(cycles)
print(ord)
print(raised_perm)
