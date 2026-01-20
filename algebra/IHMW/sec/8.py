from sympy.geometry import Point3D, Plane, Line3D

# 1
A = Point3D(-4, 1, 0) 

# 2
P1 = Point3D(5, 1, 5)
P2 = Point3D(4, -9, -3)
P3 = Point3D(10, 7, -10)
sigma = Plane(P1, P2, P3)

# 3
dist = sigma.distance(A)
print("Dist. between A and sigma equals to ", dist)

# 4
M1 = Point3D(5, 5, -10)
M2 = Point3D(6, -8, -2)
l = Line3D(M1, M2)

# 5
intersection = sigma.intersection(l)
print("The point of intersection of l and sigma is ", intersection[0])

# 6
proj_A_l = l.projection(A)
print("Projection of A on l is ", proj_A_l)

# 7
proj_A_sigma = sigma.projection(A)
print("Projection of A on sigma is ", proj_A_sigma)

# 8
print("Sigma equation is ", sigma.equation())

# 9
print("Sigma normal vector is ", sigma.normal_vector)

# 10
sigma1 = Plane(A, normal_vector=sigma.normal_vector)
print("Sigma1 is ", sigma1.equation())
