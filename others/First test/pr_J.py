import math

data = input().split()

M = int(data[0])
L = int(data[1])

T = []
S = []
K = []

index = 2
for i in range(L):
    T.append(int(data[index]))
    S.append(int(data[index + 1]))
    K.append(int(data[index + 2]))
    index += 3

savings = 0
current_salary = M

for year in range(L):
    annual_income = current_salary * 12
    
    tax = math.ceil(annual_income * T[year] / 100)
    
    income_after_tax = annual_income - tax
    
    expenses = math.ceil(annual_income * S[year] / 100)
    
    if income_after_tax >= expenses:
        savings += income_after_tax - expenses
    else:
        savings -= expenses - income_after_tax
    
    current_salary = math.ceil(current_salary * (100 + K[year]) / 100)

print(savings)

