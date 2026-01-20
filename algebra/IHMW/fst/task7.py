import numpy

zero_matrix = numpy.zeros((7, 5))

diag_matrix = numpy.diag([1, 2, 3, 4, 5, 6, 7])

trace_diag = numpy.trace(diag_matrix)

trace_inverse = numpy.linalg.inv(diag_matrix)

matrix_X = numpy.random.rand(7, 4)

submatrix_X = matrix_X[2:4, 1:3]
det_submatrix_X = numpy.linalg.det(submatrix_X)

mult = numpy.dot(numpy.transpose(matrix_X), matrix_X)

print(mult)
