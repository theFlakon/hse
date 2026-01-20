data = input().split()
N = int(data[0])
a = int(data[1])
b = int(data[2])
total_cost = 0

for _ in range(N):
    street_data = list(map(int, input().split()))
    num_houses = street_data[0]
    houses = street_data[1:1+num_houses]
    
    for house in houses:
        binary_str = bin(house)[2:]
        for char in binary_str:
            if char == '0':
                total_cost += a
            else:
                total_cost += b
                
print(total_cost)
