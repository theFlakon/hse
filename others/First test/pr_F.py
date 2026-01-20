import sys
data = sys.stdin.read().splitlines()

n = int(data[0])
submissions = []
for i in range(1, n + 1):
	parts = data[i].split()
	time_str = parts[0]
	problem = parts[1]
	verdict = parts[2]

hours, minutes = map(int, time_str.split(':'))
total_minutes = hours * 60 + minutes

submissions.append((problem, verdict, total_minutes))

solved = {}  
wrong_attempts = {}  
first_ok_time = {}  

for problem, verdict, time in submissions:
	if problem not in solved:
		solved[problem] = False
		wrong_attempts[problem] = 0
		first_ok_time[problem] = None

	if solved[problem]:
		continue

	if verdict == "OK":
		solved[problem] = True
		first_ok_time[problem] = time
	elif verdict != "CE": 
		wrong_attempts[problem] += 1

total_solved = 0
total_penalty = 0

for problem in solved:
	if solved[problem]:
		total_solved += 1
		penalty_time = first_ok_time[problem] + 20 * wrong_attempts[problem]
		total_penalty += penalty_time

print(total_solved, total_penalty)

